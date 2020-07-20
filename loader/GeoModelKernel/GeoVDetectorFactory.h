/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOVDETECTORFACTORY_H
#define GEOMODELKERNEL_GEOVDETECTORFACTORY_H

/**
 * @class: GeoVDetectorFactory
 *
 * @brief Abstact Base Class for a detector factory.  The detector
 *	factory builds the geometry and creates a GeoDetector
 *	Manager, which one may thereafter query for all raw
 *	geometry, and all readout geometry.
 */

#include "GeoModelKernel/GeoVPhysVol.h"

class GeoPhysVol;
class GeoVDetectorManager;

class GeoVDetectorFactory 
{
 public:
  GeoVDetectorFactory();
  virtual ~GeoVDetectorFactory();

  //	Create the system.
  virtual void create (GeoPhysVol* world) = 0;

  //	Access to the Detector Manager.  When implementing this
  //	method, please use the covariant return type mechanism.
  virtual const GeoVDetectorManager* getDetectorManager () const = 0;

 private:
  GeoVDetectorFactory(const GeoVDetectorFactory &right);
  GeoVDetectorFactory & operator=(const GeoVDetectorFactory &right);

};

#endif
