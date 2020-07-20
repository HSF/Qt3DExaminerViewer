/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOTUBS_H
#define GEOMODELKERNEL_GEOTUBS_H

#include "GeoModelKernel/GeoShape.h"

class GeoTubs : public GeoShape
{
 public:
  GeoTubs (double RMin, double RMax, double ZHalfLength, double SPhi, double DPhi);

  //	Returns the volume of the shape, for mass inventory
  virtual double volume () const;

  //	Returns the TUBS shape type, as a string.
  virtual const std::string & type () const;

  //	Returns the TUBS shape type, as a coded integer.
  virtual ShapeType typeID () const;

  //	Executes a GeoShapeAction
  virtual void exec (GeoShapeAction *action) const;

  //	For type identification.
  static const std::string& getClassType ();

  //	For type identification.
  static ShapeType getClassTypeID ();

  //	Minimum (inner) tube section radius.
  const double& getRMin () const;

  //	Maximum (outer) tube section radius.
  const double& getRMax () const;

  //	Tube section half-length in the z direction.
  const double& getZHalfLength () const;

  //	Starting angle of the tube section in radians.
  const double& getSPhi () const;

  //	Delta angle of the tube section in radians.
  const double& getDPhi () const;

 protected:
  virtual ~GeoTubs();

 private:
  GeoTubs(const GeoTubs &right);
  GeoTubs & operator=(const GeoTubs &right);

  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_rMin;
  double m_rMax;
  double m_zHalfLength;
  double m_sPhi;
  double m_dPhi;

};

inline const std::string& GeoTubs::getClassType ()
{
  return s_classType;
}

inline ShapeType GeoTubs::getClassTypeID ()
{
  return s_classTypeID;
}

inline const double& GeoTubs::getRMin () const
{
  return m_rMin;
}

inline const double& GeoTubs::getRMax () const
{
  return m_rMax;
}

inline const double& GeoTubs::getZHalfLength () const
{
  return m_zHalfLength;
}

inline const double& GeoTubs::getSPhi () const
{
  return m_sPhi;
}

inline const double& GeoTubs::getDPhi () const
{
  return m_dPhi;
}

#endif
