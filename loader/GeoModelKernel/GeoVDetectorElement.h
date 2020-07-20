/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOVDETECTORELEMENT_H
#define GEOMODELKERNEL_GEOVDETECTORELEMENT_H

/**
 * @class GeoVDetectorElement
 *
 * @brief A GeoVDetectorElement is a piece of readout geometry.
 *	This is an abstract base class for readout objects which
 *	have a distinct association to a piece of physical
 *	geometry and whose position we track by specifying a
 *	Euclidean transformation w.r.t global coordinates. level
 *	that one can keep track of with Euclidean
 *	transformations.  Examples:
 *
 *	* silicon wafers (not strips or pixels!)
 *	* calorimeter modules (not cells!)
 */

#include "GeoModelKernel/GeoVFullPhysVol.h"
class Identifier;

class GeoVDetectorElement 
{
 public:
  GeoVDetectorElement (const GeoVFullPhysVol* fullPhysVol);
  virtual ~GeoVDetectorElement();

  //	Returns a pointer to a physical volume which is
  //	associated with this detector element.
  const GeoVFullPhysVol* getMaterialGeom () const;

 private:
  GeoVDetectorElement(const GeoVDetectorElement &right);
  GeoVDetectorElement & operator=(const GeoVDetectorElement &right);

 private:
  const GeoVFullPhysVol *m_materialGeom;

};

inline const GeoVFullPhysVol* GeoVDetectorElement::getMaterialGeom () const
{
  return m_materialGeom;
}


#endif
