/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOFACET_H
#define GEOMODELKERNEL_GEOFACET_H

/**
 * @class: GeoFacet
 *
 * @brief Base class for GeoModel Facets
 * Two implementations exist:
 *   1. GeoTriangularFacet     
 *   2. GeoQuadrangularFacet
 */

#include "GeoModelKernel/RCBase.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include <vector>

typedef GeoTrf::Vector3D GeoFacetVertex;

// ** Base class
class GeoFacet : public RCBase
{
 public:
  enum GeoFacetVertexType
  {
    ABSOLUTE,
    RELATIVE
  };

  inline size_t getNumberOfVertices() const;
  inline GeoFacetVertex getVertex(size_t) const;
  inline GeoFacetVertexType getVertexType() const;

 protected:
  GeoFacet()
    : m_nVertices(0),
      m_vertexType(ABSOLUTE) {};
  virtual ~GeoFacet(){};

  size_t m_nVertices;
  std::vector<GeoFacetVertex> m_vertices;
  GeoFacetVertexType m_vertexType;
};

// Triangular facet
class GeoTriangularFacet : public GeoFacet
{
 public:
  GeoTriangularFacet(GeoFacetVertex
		     ,GeoFacetVertex
		     ,GeoFacetVertex
		     ,GeoFacetVertexType);

  virtual ~GeoTriangularFacet();
};

// Quadrangular facet
class GeoQuadrangularFacet : public GeoFacet
{
 public:
  GeoQuadrangularFacet(GeoFacetVertex
		       ,GeoFacetVertex
		       ,GeoFacetVertex
		       ,GeoFacetVertex
		       ,GeoFacetVertexType);

  virtual ~GeoQuadrangularFacet();
};

// Inline methods
inline size_t GeoFacet::getNumberOfVertices() const
{
  return m_nVertices;
}

inline GeoFacetVertex GeoFacet::getVertex(size_t index) const
{
  return (index<m_nVertices ? m_vertices[index] : GeoFacetVertex(999999.,999999.,999999.));
}

inline GeoFacet::GeoFacetVertexType GeoFacet::getVertexType() const
{
  return m_vertexType;
}

#endif
