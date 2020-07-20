/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOVSTORE_H
#define GEOMODELKERNEL_GEOVSTORE_H

/**
 * @class GeoVStore
 *
 * @brief Abstract base class for a generic GeoModel store
 * The interface allows to store pointers to two types of objects
 *   1. Full Physical Volumes
 *   2. Alignable Transforms
 *
 * A pointer to the store object can be passed to GeoVGeometryPlugin::create().
 * This allows for storing pointers to full physical volumes and alignable transforms
 * while the plugin is building raw detector geometries. The stored pointers can
 * be used later for constructing the readout geometry layer, and for applying
 * alignment corrections
 */

#include <any>

class GeoAlignableTransform;
class GeoVFullPhysVol;

class GeoVStore
{
 public:
  GeoVStore() {}
  virtual ~GeoVStore() {}

  virtual void storeAXF(GeoAlignableTransform* axf, std::any key) = 0;
  virtual void storeFPV(GeoVFullPhysVol* fpv, std::any key) = 0;

};


#endif
