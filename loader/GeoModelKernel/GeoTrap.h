/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOTRAP_H
#define GEOMODELKERNEL_GEOTRAP_H

/**
 * @class GeoTrap
 *
 * @brief This shape represents a very general trapezoid
 */

#include "GeoModelKernel/GeoShape.h"

class GeoTrap : public GeoShape
{
 public:
  GeoTrap (double ZHalfLength, double Theta, double Phi, double Dydzn, double Dxdyndzn, double Dxdypdzn, double Angleydzn, double Dydzp, double Dxdyndzp, double Dxdypdzp, double Angleydzp);

  //	Returns the volume of the shape, for mass inventory
  virtual double volume () const;
  
  //	Returns the TRAP shape type, as a string.
  virtual const std::string & type () const;
  
  //	Returns the TRAP shape type, as a coded integer.
  virtual ShapeType typeID () const;
  
  //	Executes a GeoShapeAction
  virtual void exec (GeoShapeAction *action) const;
  
  //	For type identification.
  static const std::string& getClassType ();
  
  //	For type identification.
  static ShapeType getClassTypeID ();
  
  //	Z half length.
  const double& getZHalfLength () const;
  
  //	Polar (theta) angle.
  const double& getTheta () const;
  
  //	Azimuthal (phi) angle.
  const double& getPhi () const;
  
  //	Y half length at -z.
  const double& getDydzn () const;
  
  //	X half length at -z, -y.
  const double& getDxdyndzn () const;
  
  //	X half length at -z, +y
  const double& getDxdypdzn () const;
  
  const double& getAngleydzn () const;
  
  //	Y half length at +z.
  const double& getDydzp () const;
  
  //	X half length at +z, -y
  const double& getDxdyndzp () const;
  
  //	X half length at +z, +y
  const double& getDxdypdzp () const;
  
  const double& getAngleydzp () const;
  
 protected:
  virtual ~GeoTrap();

 private:
  GeoTrap(const GeoTrap &right);
  GeoTrap & operator=(const GeoTrap &right);

  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_zHalfLength;
  double m_theta;
  double m_phi;
  double m_dydzn;
  double m_dxdyndzn;
  double m_dxdypdzn;
  double m_angleydzn;
  double m_dydzp;
  double m_dxdyndzp;
  double m_dxdypdzp;
  double m_angleydzp;

};

inline const std::string& GeoTrap::getClassType ()
{
  return s_classType;
}

inline ShapeType GeoTrap::getClassTypeID ()
{
  return s_classTypeID;
}

inline const double& GeoTrap::getZHalfLength () const
{
  return m_zHalfLength;
}

inline const double& GeoTrap::getTheta () const
{
  return m_theta;
}

inline const double& GeoTrap::getPhi () const
{
  return m_phi;
}

inline const double& GeoTrap::getDydzn () const
{
  return m_dydzn;
}

inline const double& GeoTrap::getDxdyndzn () const
{
  return m_dxdyndzn;
}

inline const double& GeoTrap::getDxdypdzn () const
{
  return m_dxdypdzn;
}

inline const double& GeoTrap::getAngleydzn () const
{
  return m_angleydzn;
}

inline const double& GeoTrap::getDydzp () const
{
  return m_dydzp;
}

inline const double& GeoTrap::getDxdyndzp () const
{
  return m_dxdyndzp;
}

inline const double& GeoTrap::getDxdypdzp () const
{
  return m_dxdypdzp;
}

inline const double& GeoTrap::getAngleydzp () const
{
  return m_angleydzp;
}

#endif
