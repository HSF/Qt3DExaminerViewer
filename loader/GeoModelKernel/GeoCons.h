/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOCONS_H
#define GEOMODELKERNEL_GEOCONS_H

#include "GeoModelKernel/GeoShape.h"

class GeoCons : public GeoShape
{
 public:
  GeoCons (double RMin1, double RMin2, double RMax1, double RMax2, double DZ, double SPhi, double DPhi);

  //	Returns the volume of the shape, for mass inventory
  virtual double volume () const;

  //	Returns the CONS shape type, as a string.
  virtual const std::string & type () const;
  
  //	Returns the CONS shape type, as a coded integer.
  virtual ShapeType typeID () const;
  
  //	Executes a GeoShapeAction
  virtual void exec (GeoShapeAction *action) const;
  
  //	For type identification.
  static const std::string& getClassType ();
  
  //	For type identification.
  static ShapeType getClassTypeID ();
  
  //	Inside radius at -dZ
  const double& getRMin1 () const;
  
  //	Inside radius at +dZ
  const double& getRMin2 () const;
  
  //	Outside radius at -dZ
  const double& getRMax1 () const;
  
  //	Outside radius at +dZ
  const double& getRMax2 () const;
  
  //	Half length in Z direction.
  const double& getDZ () const;
  
  //	Starting angle of the segment in radians
  const double& getSPhi () const;
  
  //	Delta angle of the segment in radians.
  const double& getDPhi () const;
  
 protected:
  virtual ~GeoCons();
  
 private:
  GeoCons(const GeoCons &right);
  GeoCons & operator=(const GeoCons &right);
  
  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_rMin1;
  double m_rMin2;
  double m_rMax1;
  double m_rMax2;
  double m_dZ;
  double m_sPhi;
  double m_dPhi;
};

inline const std::string& GeoCons::getClassType ()
{
  return s_classType;
}

inline ShapeType GeoCons::getClassTypeID ()
{
  return s_classTypeID;
}

inline const double& GeoCons::getRMin1 () const
{
  return m_rMin1;
}

inline const double& GeoCons::getRMin2 () const
{
  return m_rMin2;
}

inline const double& GeoCons::getRMax1 () const
{
  return m_rMax1;
}

inline const double& GeoCons::getRMax2 () const
{
  return m_rMax2;
}

inline const double& GeoCons::getDZ () const
{
  return m_dZ;
}

inline const double& GeoCons::getSPhi () const
{
  return m_sPhi;
}

inline const double& GeoCons::getDPhi () const
{
  return m_dPhi;
}

#endif
