/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOPVLINK_H
#define GEOMODELKERNEL_GEOPVLINK_H

/**
 * @class GeoPVConstLink
 *
 * @brief Smart pointer to physical volume... non-constant
 */

#include "GeoModelKernel/GeoPVConstLink.h"

class GeoPVLink : public GeoPVConstLink
{
 public:
  GeoPVLink();
  GeoPVLink(const GeoPVLink &right);
  GeoPVLink (const GeoVPhysVol  *addr);

  virtual ~GeoPVLink();

  //	Dereference this smart pointer, like a regular pointer.
  virtual GeoVPhysVol & operator * ();
  
  //	Allow use of smartPointer->operation().....
  virtual GeoVPhysVol * operator -> ();
  
  //	Dereference this smart pointer, like a regular pointer.
  virtual const GeoVPhysVol & operator * () const;
  
  //	Allow use of smartPointer->operation().....
  virtual const GeoVPhysVol * operator -> () const;
  
  //	Assignment operator.
  GeoPVLink& operator = (const GeoPVLink& right);    
};


#endif
