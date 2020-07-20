/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOPCON_H
#define GEOMODELKERNEL_GEOPCON_H

/**
 * @class GeoPcon
 *
 * @brief This shape represents a polycone.
 * Specify the starting angle and delta angle (subtended angle)
 * of the polycone first, Then add at least two planes with the
 * addPlane( double zPlane, double rInner,  double rOuter) method.
 */

#include "GeoModelKernel/GeoShape.h"
#include <vector>

class GeoPcon : public GeoShape
{
 public:
  //	Constructor for the PCON.  Note that the constructor
  //	does not fully build this object.  The PCON is not valid
  //	until at least two polygon planes have been added.
  GeoPcon (double SPhi, double DPhi);

  //	Returns the volume of the shape, for mass inventory
  virtual double volume () const;
  
  //	Returns the PCON shape type, as a string.
  virtual const std::string & type () const;
  
  //	Returns the PCON shape type, as a coded integer.
  virtual ShapeType typeID () const;
  
  //	Add another plane to the polycone  A minimum of two
  //	planes are required to create a valid polycone.
  void addPlane (double ZPlane, double RMinPlane, double RMaxPlane);
  
  //	Returns the number of planes that have been created for
  //	the polycone.
  unsigned int getNPlanes () const;
  
  //	True if the polycone has at least two planes.  False
  //	otherwise.
  bool isValid () const;
  
  //	Get the Z Position of the specified plane.
  const double & getZPlane (unsigned int i) const;
  
  //	Get the RMin of the specified plane.
  const double & getRMinPlane (unsigned int i) const;
  
  //	Get the Z Position of the specified plane.
  const double & getRMaxPlane (unsigned int i) const;
  
  //	Executes a GeoShapeAction
  virtual void exec (GeoShapeAction *action) const;
  
  //	For type identification.
  static const std::string& getClassType ();
  
  //	For type identification.
  static ShapeType getClassTypeID ();
  
  //	Starting angle of the segment in radians.
  const double& getSPhi () const;
  
  //	Delta angle of the segment in radians.
  const double& getDPhi () const;

 protected:
  virtual ~GeoPcon();
  
 private:
  GeoPcon(const GeoPcon &right);
  GeoPcon & operator=(const GeoPcon &right);
  
  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_sPhi;
  double m_dPhi;

  //	Z Position of poly-cone planes.
  std::vector<double> m_zPlane;
  
  //	Minimum radius of poly-cone planes.
  std::vector<double> m_rMinPlane;

  //	Maximum radius of poly-cone planes.
  std::vector<double> m_rMaxPlane;
};

inline unsigned int GeoPcon::getNPlanes () const
{
  return m_zPlane.size ();
}

inline bool GeoPcon::isValid () const
{
  return getNPlanes () >= 2;
}

inline const double & GeoPcon::getZPlane (unsigned int i) const
{
  return m_zPlane[i];
}

inline const double & GeoPcon::getRMinPlane (unsigned int i) const
{
  return m_rMinPlane[i];
}

inline const double & GeoPcon::getRMaxPlane (unsigned int i) const
{
  return m_rMaxPlane[i];
}

inline const std::string& GeoPcon::getClassType ()
{
  return s_classType;
}

inline ShapeType GeoPcon::getClassTypeID ()
{
  return s_classTypeID;
}

inline const double& GeoPcon::getSPhi () const
{
  return m_sPhi;
}

inline const double& GeoPcon::getDPhi () const
{
  return m_dPhi;
}

#endif
