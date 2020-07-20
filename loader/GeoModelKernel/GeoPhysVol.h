/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOPHYSVOL_H
#define GEOMODELKERNEL_GEOPHYSVOL_H

#include "GeoModelKernel/GeoVPhysVol.h"

/**
 * @class GeoPhysVol
 *
 * @brief A GeoPhysVol is a physical volume for normal recursive
 * access only. This means, normally, that it represents a
 * piece of detector that nobody cares about except for
 * tracing engines like GEANT. More specifically:
 *
 * The position of the physical volume is not kept with
 * the physical volume. It can easily be obtained by
 * traversing detector graph (ComputeTransformAction), but
 * it cannot be obtained in constant time. If you need to
 * know the absolute default and/or aligned position of the
 * detector, you need to use a different kind of physical
 * volume called a GeoRAPhysicalVolume, which has a higher
 * memory cost.
 */

#include <mutex>


class GeoPhysVol : public GeoVPhysVol
{
 public:
  GeoPhysVol(const GeoLogVol* LogVol);

  GeoPhysVol(const GeoPhysVol &right) = delete;
  GeoPhysVol & operator=(const GeoPhysVol &right) = delete;

  void add(GeoGraphNode* graphNode);

  /// Returns the number of child physical volumes.
  virtual unsigned int getNChildVols() const override final;

  /// Returns the ith child volume
  virtual PVConstLink  getChildVol(unsigned int index) const override final;

  /// Returns the transform to the ith volume.
  virtual GeoTrf::Transform3D getXToChildVol(unsigned int index
					      ,const GeoVAlignmentStore* store=nullptr) const override final;

  /// Returns the default transform to the ith volume.
  virtual GeoTrf::Transform3D getDefXToChildVol(unsigned int index
						 ,const GeoVAlignmentStore* store=nullptr) const override final;

  /// Executes a GeoNodeAction.
  virtual void exec(GeoNodeAction *action) const override final;

  /// Returns the name of the child.
  virtual std::string getNameOfChildVol(unsigned int i) const override final;

  /// Returns the id of the child.
  virtual Query<int> getIdOfChildVol(unsigned int i) const override final;

  /// Returns the number of child physical volumes and Serial Transformers.
  virtual unsigned int getNChildVolAndST() const override final;

  virtual GeoTrf::Transform3D getX    (const GeoVAlignmentStore* store=nullptr) const override final;
  virtual GeoTrf::Transform3D getDefX (const GeoVAlignmentStore* store=nullptr) const override final;
  virtual unsigned int getNChildNodes() const override final;
  virtual const GeoGraphNode * const *getChildNode (unsigned int i) const override final;
  virtual const GeoGraphNode * const *findChildNode(const GeoGraphNode *n) const override final;

 protected:
  virtual ~GeoPhysVol() override;

 private:
  std::vector<const GeoGraphNode*> m_daughters;
  mutable std::mutex m_muxVec;
};

#endif
