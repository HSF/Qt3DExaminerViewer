/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOPGON_H
#define GEOMODELKERNEL_GEOPGON_H

#include "GeoModelKernel/GeoShape.h"
#include <vector>

class GeoPgon : public GeoShape
{
 public:
  //	Constructor for the PGON.  Note that the constructor
  //	does not fully build this object.  The PGON is not valid
  //	until at least two polygon planes have been added.
  GeoPgon (double SPhi, double DPhi, unsigned int NSides);

  //	Returns the volume of the shape, for mass inventory
  virtual double volume () const;
  
  //	Returns the PGON shape type, as a string.
  virtual const std::string & type () const;
  
  //	Returns the PGON shape type, as a coded integer.
  virtual ShapeType typeID () const;
  
  //	Add another plane to the polygon.  A minimum of two
  //	planes are required to create a valid polygon.
  void addPlane (double ZPlane, double RMinPlane, double RMaxPlane);
  
  //	Returns the number of planes that have been created for
  //	the polygon.
  unsigned int getNPlanes () const;
  
  //	True if the polygon has at least two planes.  False
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
  
  //	Number of sides in each polygonal segment.
  const unsigned int& getNSides () const;
  
 protected:
  virtual ~GeoPgon();
  
 private:
  GeoPgon(const GeoPgon &right);
  GeoPgon & operator=(const GeoPgon &right);
  
  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_sPhi;
  double m_dPhi;
  unsigned int m_nSides;

  //	Z Position of polygon planes.
  std::vector<double> m_zPlane;

  //	Minimum radius of polygon planes.
  std::vector<double> m_rMinPlane;

  //	Maximum radius of polygon planes.
  std::vector<double> m_rMaxPlane;
};

inline unsigned int GeoPgon::getNPlanes () const
{
  return m_zPlane.size ();
}

inline bool GeoPgon::isValid () const
{
  return m_zPlane.size () >= 2;
}

inline const double & GeoPgon::getZPlane (unsigned int i) const
{
  return m_zPlane[i];
}

inline const double & GeoPgon::getRMinPlane (unsigned int i) const
{
  return m_rMinPlane[i];
}

inline const double & GeoPgon::getRMaxPlane (unsigned int i) const
{
  return m_rMaxPlane[i];
}

inline const std::string& GeoPgon::getClassType ()
{
  return s_classType;
}

inline ShapeType GeoPgon::getClassTypeID ()
{
  return s_classTypeID;
}

inline const double& GeoPgon::getSPhi () const
{
  return m_sPhi;
}

inline const double& GeoPgon::getDPhi () const
{
  return m_dPhi;
}

inline const unsigned int& GeoPgon::getNSides () const
{
  return m_nSides;
}

#endif
