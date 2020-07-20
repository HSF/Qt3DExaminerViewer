/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOVDETECTORMANAGER_H
#define GEOMODELKERNEL_GEOVDETECTORMANAGER_H

/**
 * @class GeoVDetectorManager
 *
 * @brief This class is the Manager for each detector. It has two
 *	required query mechanism:  one for "tree tops" (These
 *	are top level pieces of raw geometry; there may be one
 *	or more of these), and another one for detector
 *	elements, which represent readout units.
 *
 *	Subsystems engineers should override this.  They may in
 *	addition add their own query mechanisms to their
 *	interface.
 *	They may in fact add anything else they require to their
 *	interface.
 */

#include "GeoModelKernel/GeoVPhysVol.h"

class GeoVDetectorElement;

class GeoVDetectorManager 
{
 public:
  GeoVDetectorManager();
  virtual ~GeoVDetectorManager();

  //	Returns the number of Tree Tops (Top Level Physicsl
  //	Volumes)
  virtual unsigned int getNumTreeTops () const = 0;

  //	Access to the Tree Top
  virtual PVConstLink getTreeTop (unsigned int i) const = 0;
  
  const std::string & getName() const;
  void                setName(const std::string & name);

 private:
  GeoVDetectorManager(const GeoVDetectorManager &right);
  GeoVDetectorManager & operator=(const GeoVDetectorManager &right);

  std::string m_name;

};

#endif
