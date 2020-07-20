/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOPVCONSTLINK_H
#define GEOMODELKERNEL_GEOPVCONSTLINK_H

/**
 * @class GeoPVConstLink
 *
 * @brief Smart pointer to physical volume... constant
 */

class GeoVPhysVol;

class GeoPVConstLink 
{
 public:
  GeoPVConstLink();
  GeoPVConstLink(const GeoPVConstLink &right);
  GeoPVConstLink (const GeoVPhysVol  *addr);

  virtual ~GeoPVConstLink();

  int operator==(const GeoPVConstLink &right) const;
  int operator!=(const GeoPVConstLink &right) const;

  int operator<(const GeoPVConstLink &right) const;
  int operator>(const GeoPVConstLink &right) const;
  int operator<=(const GeoPVConstLink &right) const;
  int operator>=(const GeoPVConstLink &right) const;

  //	Dereference this smart pointer, like a regular pointer.
  virtual const GeoVPhysVol & operator * () const;

  //	Allow use of smartPointer->operation().....
  virtual const GeoVPhysVol * operator -> () const;

  operator bool () const;

  GeoPVConstLink& operator = (const GeoPVConstLink& right);

 protected:
  //	A pointer to the underlying volume
  GeoVPhysVol *  m_ptr;

};

#endif
