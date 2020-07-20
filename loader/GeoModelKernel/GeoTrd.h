/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOTRD_H
#define GEOMODELKERNEL_GEOTRD_H

#include "GeoModelKernel/GeoShape.h"

class GeoTrd : public GeoShape
{
 public:
  GeoTrd (double XHalfLength1, double XHalfLength2, double YHalfLength1, double YHalfLength2, double ZHalfLength);

  //	Returns the volume of the shape, for mass inventory
  virtual double volume () const;
  
  //	Returns the TRD shape type, as a string.
  virtual const std::string & type () const;
  
  //	Returns the TRD shape type, as a coded integer.
  virtual ShapeType typeID () const;
  
  //	Executes a GeoShapeAction
  virtual void exec (GeoShapeAction *action) const;
  
  //	For type identification.
  static const std::string& getClassType ();
  
  //	For type identification.
  static ShapeType getClassTypeID ();
  
  //	Half length in the x-direction at -dz.
  const double& getXHalfLength1 () const;
  
  //	Half length in the x-direction at +dz
  const double& getXHalfLength2 () const;
  
  //	Half-length in the y direction at +dz.
  const double& getYHalfLength1 () const;
  
  //	Half-length in the y direction at -dz
  const double& getYHalfLength2 () const;
  
  //	Half-length in the z direction.
  const double& getZHalfLength () const;

 protected:
  virtual ~GeoTrd();

 private:
  GeoTrd(const GeoTrd &right);
  GeoTrd & operator=(const GeoTrd &right);
  
  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_xHalfLength1;
  double m_xHalfLength2;
  double m_yHalfLength1;
  double m_yHalfLength2;
  double m_zHalfLength;

};

inline const std::string& GeoTrd::getClassType ()
{
  return s_classType;
}

inline ShapeType GeoTrd::getClassTypeID ()
{
  return s_classTypeID;
}

inline const double& GeoTrd::getXHalfLength1 () const
{
  return m_xHalfLength1;
}

inline const double& GeoTrd::getXHalfLength2 () const
{
  return m_xHalfLength2;
}

inline const double& GeoTrd::getYHalfLength1 () const
{
  return m_yHalfLength1;
}

inline const double& GeoTrd::getYHalfLength2 () const
{
  return m_yHalfLength2;
}

inline const double& GeoTrd::getZHalfLength () const
{
  return m_zHalfLength;
}

#endif
