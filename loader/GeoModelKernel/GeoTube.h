/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOTUBE_H
#define GEOMODELKERNEL_GEOTUBE_H

#include "GeoModelKernel/GeoShape.h"

class GeoTube : public GeoShape
{
 public:
  GeoTube (double RMin, double RMax, double ZHalfLength);

  //	Returns the volume of the shape, for mass inventory
  virtual double volume () const;
  
  //	Returns the TUBE shape type, as a string.
  virtual const std::string & type () const;
  
  //	Returns the TUBE shape type, as a coded integer.
  virtual ShapeType typeID () const;
  
  //	Executes a GeoShapeAction
  virtual void exec (GeoShapeAction *action) const;
  
  //	For type identification.
  static const std::string& getClassType ();
  
  //	For type identification.
  static ShapeType getClassTypeID ();
  
  //	Minimum (inner) tube radius.
  const double& getRMin () const;
  
  //	Maximum (outer) tube radius.
  const double& getRMax () const;
  
  //	Tube half-length in the z direction.
  const double& getZHalfLength () const;
  
 protected:
  //## Destructor (generated)
  virtual ~GeoTube();
  
 private:
  GeoTube(const GeoTube &right);
  GeoTube & operator=(const GeoTube &right);

  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_rMin;
  double m_rMax;
  double m_zHalfLength;

};

inline const std::string& GeoTube::getClassType ()
{
  return s_classType;
}

inline ShapeType GeoTube::getClassTypeID ()
{
  return s_classTypeID;
}

inline const double& GeoTube::getRMin () const
{
  return m_rMin;
}

inline const double& GeoTube::getRMax () const
{
  return m_rMax;
}

inline const double& GeoTube::getZHalfLength () const
{
  return m_zHalfLength;
}

#endif
