/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOVPHYSVOL_H
#define GEOMODELKERNEL_GEOVPHYSVOL_H

#include "GeoModelKernel/GeoPVLink.h"
#define SMARTLINKS
#ifdef  SMARTLINKS
typedef GeoPVLink PVLink;
typedef GeoPVConstLink PVConstLink;
#else
class GeoVPhysVol;
typedef GeoVPhysVol *PVLink;
typedef const GeoVPhysVol *PVConstLink;
#endif
#include "GeoModelKernel/GeoDefinitions.h"
#include "GeoModelKernel/Query.h"
#include <string>

#include "GeoModelKernel/GeoLogVol.h"
#include "GeoModelKernel/GeoGraphNode.h"

class GeoVolumeAction;
class GeoVAlignmentStore;

class GeoVPhysVol : public GeoGraphNode
{
 public:
  GeoVPhysVol(const GeoLogVol* LogVol);
  
  GeoVPhysVol(const GeoVPhysVol &right) = delete;
  GeoVPhysVol & operator=(const GeoVPhysVol &right) = delete;

  /// Returns true if the physical volume is accessed by more than one parent.
  /// Should check this before trusting the parent pointer.
  bool isShared() const;

  /// Returns the index of a specific daughter volume.  The Query class can be used
  /// just like an unsigned int, but it provides and isValid() method to determine
  /// whether it is valid and throws an exception if an invalid value is blithely used.
  Query<unsigned int> indexOf(PVConstLink daughter) const;

  /// Gets the parent, if the parent is unique, and otherwise returns a nullptr pointer.
  PVConstLink getParent() const;

  /// Returns the logical volume.
  const GeoLogVol* getLogVol() const;

  /// Returns the number of child physical volumes.
  virtual unsigned int getNChildVols() const = 0;

  /// Returns the ith child volume
  virtual PVConstLink  getChildVol(unsigned int index) const = 0;

  /// Returns the transform to the ith volume.
  virtual GeoTrf::Transform3D getXToChildVol(unsigned int index, const GeoVAlignmentStore* store=nullptr) const = 0;

  /// Returns the default transform to the ith volume.
  virtual GeoTrf::Transform3D getDefXToChildVol(unsigned int index, const GeoVAlignmentStore* store=nullptr) const = 0;

  /// Returns the name of the child.
  virtual std::string getNameOfChildVol(unsigned int i) const = 0;

  /// Applies an action to the volume. The volume executes the action and the applies
  /// it to the children, either from the top down or the bottom up, depending upon
  /// the configuration of the action.
  void apply(GeoVolumeAction* action) const;

  /// When a node is added to a parent in the graph, the node is always notified.
  /// What happens at that time is up to the node.  Most nodes do not need to do
  /// anything. Some--the alignable transforms in particular--need to take some
  /// actions, such as adding the parent volume to a list.
  virtual void dockTo(GeoVPhysVol* parent);

  /// Returns the id of the child.
  virtual Query<int> getIdOfChildVol(unsigned int i) const = 0;

  /// Returns the number of child physical volumes and Serial Transformers.
  virtual unsigned int getNChildVolAndST() const = 0;

  virtual GeoTrf::Transform3D getX    (const GeoVAlignmentStore* store=nullptr) const = 0;
  virtual GeoTrf::Transform3D getDefX (const GeoVAlignmentStore* store=nullptr) const = 0;
  virtual unsigned int getNChildNodes() const=0;
  virtual const GeoGraphNode * const *getChildNode (unsigned int i) const=0;
  virtual const GeoGraphNode * const *findChildNode(const GeoGraphNode *n) const=0;

 protected:
  virtual ~GeoVPhysVol();

 private:
  /// If one parent           ...pointer=PARENT;
  /// If no parent            ...pointer=nullptr.
  /// If more than one parent ...pointer=this;
  const GeoVPhysVol* m_parentPtr;
  
  const GeoLogVol *m_logVol;
};

inline bool GeoVPhysVol::isShared () const
{
  return m_parentPtr == this;
}

#endif
