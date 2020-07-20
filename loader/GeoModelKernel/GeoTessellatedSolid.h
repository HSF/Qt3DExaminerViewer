/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOTESSELLATEDSOLID_H
#define GEOMODELKERNEL_GEOTESSELLATEDSOLID_H

#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoFacet.h"

class GeoTessellatedSolid : public GeoShape
{
 public:
  GeoTessellatedSolid();

  virtual double volume() const;

  virtual const std::string& type() const;
  virtual ShapeType typeID() const;

  static const std::string& getClassType();
  static ShapeType getClassTypeID();

  virtual void exec(GeoShapeAction *action) const;

  void addFacet(GeoFacet*);
  GeoFacet* getFacet(size_t) const;
  size_t getNumberOfFacets() const;

  //!	True if the polygon has at least two facets. False otherwise.
  //  bool isValid () const; // TODO: implement, like, e.g., in GeoPgon

 protected:
  virtual ~GeoTessellatedSolid();

 private:
  GeoTessellatedSolid(const GeoTessellatedSolid &right);
  GeoTessellatedSolid& operator=(const GeoTessellatedSolid &right);

  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  std::vector<GeoFacet*> m_facets;
};

inline const std::string& GeoTessellatedSolid::getClassType()
{
  return s_classType;
}

inline ShapeType GeoTessellatedSolid::getClassTypeID()
{
  return s_classTypeID;
}

// TODO: implement, like, e.g., in GeoPgon
//inline bool GeoPgon::isValid () const
//{
//  return m_zPlane.size () >= 2;
//}

#endif
