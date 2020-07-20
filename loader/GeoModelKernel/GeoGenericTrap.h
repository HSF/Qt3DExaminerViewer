/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOGENERICTRAP_H
#define GEOMODELKERNEL_GEOGENERICTRAP_H

#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include <vector>

typedef GeoTrf::Vector2D GeoTwoVector;
typedef std::vector<GeoTwoVector> GeoGenericTrapVertices;

class GeoGenericTrap : public GeoShape
{
 public:
  GeoGenericTrap(double ZHalfLength, const GeoGenericTrapVertices& Vertices);

  virtual double volume() const;

  virtual const std::string& type() const;
  virtual ShapeType typeID() const;

  static const std::string& getClassType();
  static ShapeType getClassTypeID();

  virtual void exec(GeoShapeAction *action) const;

  double getZHalfLength() const;
  const GeoGenericTrapVertices& getVertices() const;

 protected:
  virtual ~GeoGenericTrap();

 private:
  GeoGenericTrap(const GeoGenericTrap &right);
  GeoGenericTrap& operator=(const GeoGenericTrap &right);

  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  double m_zHalfLength;
  GeoGenericTrapVertices m_vertices;
};

inline const std::string& GeoGenericTrap::getClassType()
{
  return s_classType;
}

inline ShapeType GeoGenericTrap::getClassTypeID()
{
  return s_classTypeID;
}

#endif
