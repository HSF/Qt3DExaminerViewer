/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOFULLPHYSVOL_H
#define GEOMODELKERNEL_GEOFULLPHYSVOL_H

#include "GeoModelKernel/GeoVFullPhysVol.h"
#include <vector>
#include <string>

class GeoLogVol;
class GeoVAlignmentStore;

/**
 * @class GeoFullPhysVol
 *
 * @brief This class represent a physical volume whose absolute
 * default and aligned positions are known and held in cache.
 *
 * These pieces should not be part of a subtree which is used as a
 * shared instance. For if it is, then of course the absolute position 
 * of the piece is an absurdity. The run time error will result as 
 * soon as the absolute position is requested.
 *
 * These elements may be declared to readout geometry. See the class 
 * ReadoutElement.
 */

class GeoFullPhysVol final : public GeoVFullPhysVol
{
 public:
  GeoFullPhysVol (const GeoLogVol* LogVol);

  /// Adds a Graph Node to the Geometry Graph.
  void add(GeoGraphNode* graphNode);

  /// Returns the number of child physical volumes.
  virtual unsigned int getNChildVols() const override;

  /// Returns the ith child volume
  virtual PVConstLink  getChildVol(unsigned int index) const override;

  /// Returns the transform to the ith volume.
  virtual GeoTrf::Transform3D getXToChildVol(unsigned int index, const GeoVAlignmentStore* store=nullptr) const override;
  
  /// Returns the default transform to the ith volume.
  virtual GeoTrf::Transform3D getDefXToChildVol(unsigned int index, const GeoVAlignmentStore* store=nullptr) const override;

  /// Executes a GeoNodeAction.
  virtual void exec(GeoNodeAction *action) const override;

  /// Returns the name of the child.
  virtual std::string getNameOfChildVol(unsigned int i) const override;

  /// Returns the id of the child.
  virtual Query<int> getIdOfChildVol(unsigned int i) const override;

  /// Returns the number of child physical volumes and Serial Transformers.
  virtual unsigned int getNChildVolAndST() const override;

  GeoFullPhysVol* clone(bool attached = true);
  const GeoFullPhysVol* cloneOrigin() const;
  /// The following method breaks consistency of cloned volumes!
  /// Use it only in Simulation jobs and
  /// don't call it until geometry has been completely translated to G4
  void clear(); // drop subtree
  virtual GeoTrf::Transform3D getX    (const GeoVAlignmentStore* store=nullptr) const override;
  virtual GeoTrf::Transform3D getDefX (const GeoVAlignmentStore* store=nullptr) const override;
  virtual unsigned int getNChildNodes() const override;
  virtual const GeoGraphNode * const *getChildNode (unsigned int i) const override;
  virtual const GeoGraphNode * const *findChildNode(const GeoGraphNode *n) const override;

  protected:
  virtual ~GeoFullPhysVol() override;

  private:
  GeoFullPhysVol(const GeoFullPhysVol &right);
  GeoFullPhysVol & operator=(const GeoFullPhysVol &right);

  /// Hold the list of children.
  std::vector<const GeoGraphNode *> m_daughters;
  
  const GeoFullPhysVol* m_cloneOrigin;
};

#endif
