/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOVFULLPHYSVOL_H
#define GEOMODELKERNEL_GEOVFULLPHYSVOL_H

#include "GeoModelKernel/GeoVPhysVol.h"
#include "GeoModelKernel/GeoAbsPositionInfo.h"

class GeoVAlignmentStore;

// To Do: consider dropping it from the class hierarchy?
class GeoVFullPhysVol : public GeoVPhysVol 
{
 public:
  GeoVFullPhysVol(const GeoLogVol* logVol);

  GeoVFullPhysVol(const GeoVFullPhysVol &right) = delete;
  GeoVFullPhysVol & operator=(const GeoVFullPhysVol &right) = delete;

  /// Returns the absolute transform of the volume.
  const GeoTrf::Transform3D& getAbsoluteTransform(GeoVAlignmentStore* store=nullptr) const;

  /// Clears the position information.  This can be used if
  /// the cache is determined to be invalid.  The usual client
  /// is one of the parents, upon changing its own position.
  /// There is little need for casual users to call this.
  void clearPositionInfo() const;

  /// Returns the default absolute transform of the volume.
  const GeoTrf::Transform3D& getDefAbsoluteTransform(GeoVAlignmentStore* store=nullptr) const;

  /// Returns the absolute name of this node.
  const std::string& getAbsoluteName();

  /// Returns the identification bits.
  unsigned int getId() const;

 protected:
  virtual ~GeoVFullPhysVol() override;

 private:
  /// The absolute name of this volume.
  std::string m_absName;

  /// An identifier. This is locally cached in a full physical volume.
  mutable Query<int> *m_id;

  /// Information on the where this volume is, by default and after alignment corrections.      
  mutable GeoAbsPositionInfo *m_absPosInfo;

};

#endif
