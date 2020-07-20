/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOSIMPLEPOLYGONBREP_H
#define GEOMODELKERNEL_GEOSIMPLEPOLYGONBREP_H

/**
 * @class: GeoSimplePolygonBrep
 *
 * @brief This shape represents a BREP solid consisting
 *      of two equivalent polygonial faces perpendicular to Z axis.
 *      The polygones are described by array of (x,y) vertices,
 *      The solid is considered valid if the number of polygon vertices >=3 
 *
 *      Constructor parameter is a half length along Z axis
 */

#include <vector>
#include "GeoModelKernel/GeoShape.h"


class GeoSimplePolygonBrep : public GeoShape
{
 public:
  GeoSimplePolygonBrep(double dz);

  virtual double volume() const;

  virtual const std::string& type() const;
  virtual ShapeType typeID() const;

  void addVertex(double XVertex, double YVertex);
  unsigned int getNVertices() const;

  bool isValid () const;

  const double & getXVertex(unsigned int i) const;
  const double & getYVertex(unsigned int i) const;

  virtual void exec(GeoShapeAction *action) const;

  static const std::string& getClassType();
  static ShapeType getClassTypeID();

  const double& getDZ() const;

 protected:
  virtual ~GeoSimplePolygonBrep();

 private:
  GeoSimplePolygonBrep(const GeoSimplePolygonBrep &right);
  GeoSimplePolygonBrep & operator=(const GeoSimplePolygonBrep &right);

  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_dZ;
  std::vector<double> m_xVertices;
  std::vector<double> m_yVertices;
};

inline unsigned int GeoSimplePolygonBrep::getNVertices() const
{
  return m_xVertices.size();
}

inline bool GeoSimplePolygonBrep::isValid() const
{
  return getNVertices()>=3;
}

inline const double& GeoSimplePolygonBrep::getXVertex(unsigned int i) const
{
  return m_xVertices[i];
}

inline const double& GeoSimplePolygonBrep::getYVertex(unsigned int i) const
{
  return m_yVertices[i];
}

inline const std::string& GeoSimplePolygonBrep::getClassType()
{
  return s_classType;
}

inline ShapeType GeoSimplePolygonBrep::getClassTypeID()
{
  return s_classTypeID;
}

inline const double& GeoSimplePolygonBrep::getDZ() const
{
  return m_dZ;
}

#endif
