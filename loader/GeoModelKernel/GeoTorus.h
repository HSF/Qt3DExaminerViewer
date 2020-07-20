/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOTORUS_H
#define GEOMODELKERNEL_GEOTORUS_H

#include "GeoModelKernel/GeoShape.h"


class GeoTorus : public GeoShape
{
 public:
  GeoTorus (double Rmin, double Rmax, double Rtor, double SPhi, double DPhi);

  virtual double volume () const;

  virtual const std::string & type () const;
  virtual ShapeType typeID () const;

  static const std::string& getClassType ();
  static ShapeType getClassTypeID ();

  virtual void exec (GeoShapeAction *action) const;

  const double& getRMin () const;
  const double& getRMax () const;
  const double& getRTor () const;
  const double& getSPhi () const;
  const double& getDPhi () const;

 protected:
  virtual ~GeoTorus();

 private:
  GeoTorus(const GeoTorus &right);
  GeoTorus & operator=(const GeoTorus &right);

  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_rMin;
  double m_rMax;
  double m_rTor;
  double m_sPhi;
  double m_dPhi;
};

inline const std::string& GeoTorus::getClassType ()
{
  return s_classType;
}

inline ShapeType GeoTorus::getClassTypeID ()
{
  return s_classTypeID;
}

inline const double& GeoTorus::getRMin () const
{
  return m_rMin;
}

inline const double& GeoTorus::getRMax () const
{
  return m_rMax;
}

inline const double& GeoTorus::getRTor () const
{
  return m_rTor;
}

inline const double& GeoTorus::getSPhi () const
{
  return m_sPhi;
}

inline const double& GeoTorus::getDPhi () const
{
  return m_dPhi;
}


#endif
