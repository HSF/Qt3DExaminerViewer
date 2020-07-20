/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOSHAPEINTERSECTION_H
#define GEOMODELKERNEL_GEOSHAPEINTERSECTION_H

#include "GeoModelKernel/GeoShape.h"


#ifndef _GeoShapePersistification_On_
  class Persistifier;
#endif


class GeoShapeIntersection : public GeoShape
{
 public:



  //	Constructor taking two shape operands.
  GeoShapeIntersection (const GeoShape* A, const GeoShape* B);

  //	Returns the volume of the shape, for mass inventory
  virtual double volume () const;

  //	Returns the AND shape type, as a string.
  virtual const std::string & type () const;

  //	Returns the AND shape type, as a coded integer.
  virtual ShapeType typeID () const;

  //	Returns the first operand being ANDed
  const GeoShape* getOpA () const;

  //	Returns the second operand being ANDed.
  const GeoShape* getOpB () const;

  //	Executes a GeoShapeAction
  virtual void exec (GeoShapeAction *action) const;

  //	For type identification.
  static const std::string& getClassType ();

  //	For type identification.
  static ShapeType getClassTypeID ();

 protected:
  virtual ~GeoShapeIntersection();

 private:
  GeoShapeIntersection(const GeoShapeIntersection &right);
  GeoShapeIntersection & operator=(const GeoShapeIntersection &right);

  //	The first shape operand in the AND operation.
  const GeoShape* m_opA;

  //	The second shape operand in the AND operation.
  const GeoShape* m_opB;

  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  //    For I/O only!
  GeoShapeIntersection(){}
  friend Persistifier;
};

inline const std::string& GeoShapeIntersection::getClassType ()
{
  return s_classType;
}

inline ShapeType GeoShapeIntersection::getClassTypeID ()
{
  return s_classTypeID;
}

#endif
