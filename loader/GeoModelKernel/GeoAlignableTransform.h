/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOALIGNABLETRANSFORM_H
#define GEOMODELKERNEL_GEOALIGNABLETRANSFORM_H

#include "GeoModelKernel/GeoTransform.h"
#include <vector>

class GeoVAlignmentStore;

class GeoAlignableTransform final : public GeoTransform
{
 public:
  GeoAlignableTransform(const GeoTrf::Transform3D &transform);

  /// Gets the total transform, including the alignment correction
  virtual GeoTrf::Transform3D getTransform(const GeoVAlignmentStore* store=nullptr) const override;

  /// Sets an alignment delta
  void setDelta(const GeoTrf::Transform3D& delta, GeoVAlignmentStore* store=nullptr);

  /// Clears the alignment delta
  // To Do: This method will become obsolete once we migrate LAr to the Alignment Store.
  //        At that point thie method should be dropped
  void clearDelta(GeoVAlignmentStore* store=nullptr);

  /// When a node is added to a parent in the graph, the node
  /// is always notified.  What happens at that time is up to
  /// the node.  Most nodes do not need to do anything.
  /// Some--the alignable transforms in particular--need to
  /// take some actions, such as adding the parent volume to a list
  virtual void dockTo(GeoVPhysVol* parent) override;

 protected:
  virtual ~GeoAlignableTransform() override;

 private:
  GeoAlignableTransform(const GeoAlignableTransform &right);
  GeoAlignableTransform & operator=(const GeoAlignableTransform &right);

  // Pointer to an alignment correction.  Until some
  // alignment correction is set, this pointer is nullptr and
  // the memory is unallocated.
  GeoTrf::Transform3D* m_delta;

  // A list of parents who use this alignable target.  They
  // must all be notified when the alignment changes!
  std::vector<GeoGraphNode*>  m_parentList;
};

#endif
