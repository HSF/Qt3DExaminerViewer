/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOSHAPEUNION_H
#define GEOMODELKERNEL_GEOSHAPEUNION_H

#include "GeoModelKernel/GeoShape.h"

#ifndef _GeoShapePersistification_On_
  class Persistifier;
#endif

class GeoShapeUnion : public GeoShape
{
 public:
  GeoShapeUnion (const GeoShape* A, const GeoShape* B);

  //	Returns the volume of the shape, for mass inventory
  virtual double volume () const;

  //	Returns the OR shape type, as a string.
  virtual const std::string & type () const;

  //	Returns the OR shape type, as a coded integer.
  virtual ShapeType typeID () const;

  //	Returns the first operand being ORed
  const GeoShape* getOpA () const;

  //	Returns the second operand being ORed.
  const GeoShape* getOpB () const;

  //	Executes a GeoShapeAction
  virtual void exec (GeoShapeAction *action) const;

  //	For type identification.
  static const std::string& getClassType ();

  //	For type identification.
  static ShapeType getClassTypeID ();

 protected:
  virtual ~GeoShapeUnion();

 private:
  GeoShapeUnion(const GeoShapeUnion &right);
  GeoShapeUnion & operator=(const GeoShapeUnion &right);

  //	The first shape operand in the OR operation.
  const GeoShape* m_opA;

  //	The second shape operand in the OR operation.
  const GeoShape* m_opB;

  static const std::string s_classType;
  static const ShapeType s_classTypeID;

    //    For I/O only!
  GeoShapeUnion(){}
  friend Persistifier;

};

inline const std::string& GeoShapeUnion::getClassType ()
{
  return s_classType;
}

inline ShapeType GeoShapeUnion::getClassTypeID ()
{
  return s_classTypeID;
}

#endif
