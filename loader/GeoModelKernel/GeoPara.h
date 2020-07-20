/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOPARA_H
#define GEOMODELKERNEL_GEOPARA_H

#include "GeoModelKernel/GeoShape.h"

class GeoPara : public GeoShape
{
 public:
  //	Constructor for the BOX.
  GeoPara (double XHalfLength, double YHalfLength, double ZHalfLength, double Alpha, double Theta, double Phi);
  
  //	Returns the volume of the shape, for mass inventory
  virtual double volume () const;
  
  //	Returns the PARA shape type, as a string.
  virtual const std::string & type () const;
  
  //	Returns the PARA shape type, as a coded integer.
  virtual ShapeType typeID () const;
  
  //	Executes a GeoShapeAction
  virtual void exec (GeoShapeAction *action) const;
  
  //	For type identification.
  static const std::string& getClassType ();
  
  //	For type identification.
  static ShapeType getClassTypeID ();
  
  //	Half length in the x-direction.
  const double& getXHalfLength () const;
  
  //	Half-length in the y direction.
  const double& getYHalfLength () const;
  
  //	Half-length in the z direction.
  const double& getZHalfLength () const;
  
  //	Polar (theta) angle.
  const double& getTheta () const;
  
  //	The angle alpha...between the two sides of the top face
  //	of the parallelapiped.
  const double& getAlpha () const;
  
  //	Azimuthal (phi) angle.
  const double& getPhi () const;
  
 protected:
  virtual ~GeoPara();
  
 private:
  GeoPara(const GeoPara &right);
  GeoPara & operator=(const GeoPara &right);
  
  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_xHalfLength;
  double m_yHalfLength;
  double m_zHalfLength;
  double m_theta;
  double m_alpha;
  double m_phi;
};

inline const std::string& GeoPara::getClassType ()
{
  return s_classType;
}

inline ShapeType GeoPara::getClassTypeID ()
{
  return s_classTypeID;
}

inline const double& GeoPara::getXHalfLength () const
{
  return m_xHalfLength;
}

inline const double& GeoPara::getYHalfLength () const
{
  return m_yHalfLength;
}

inline const double& GeoPara::getZHalfLength () const
{
  return m_zHalfLength;
}

inline const double& GeoPara::getTheta () const
{
  return m_theta;
}

inline const double& GeoPara::getAlpha () const
{
  return m_alpha;
}

inline const double& GeoPara::getPhi () const
{
  return m_phi;
}

#endif
