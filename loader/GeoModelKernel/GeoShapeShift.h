/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOSHAPESHIFT_H
#define GEOMODELKERNEL_GEOSHAPESHIFT_H

#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoDefinitions.h"


#ifndef _GeoShapePersistification_On_
  class Persistifier;
#endif


class GeoShapeShift : public GeoShape
{
 public:
  GeoShapeShift (const GeoShape* A, const GeoTrf::Transform3D &X);

  //	Returns the volume of the shape, for mass inventory
  virtual double volume () const;

  //	Returns the OR shape type, as a string.
  virtual const std::string & type () const;

  //	Returns the OR shape type, as a coded integer.
  virtual ShapeType typeID () const;

  //	Returns the first operand being ORed
  const GeoShape* getOp () const;

  //	Returns the shift of this shape.
  const GeoTrf::Transform3D & getX () const;

  //	Executes a GeoShapeAction
  virtual void exec (GeoShapeAction *action) const;

  //	For type identification.
  static const std::string& getClassType ();

  //	For type identification.
  static ShapeType getClassTypeID ();

 protected:
  virtual ~GeoShapeShift();

 private:
  GeoShapeShift(const GeoShapeShift &right);
  GeoShapeShift & operator=(const GeoShapeShift &right);

  //	The shape operand in the NOT operation.
  const GeoShape* m_op;

  //	Gives the amount by which the volume is shifted.
  GeoTrf::Transform3D m_shift;

  static const std::string s_classType;
  static const ShapeType s_classTypeID;

    //    For I/O only!
  GeoShapeShift(){}
  friend Persistifier;

};

inline const std::string& GeoShapeShift::getClassType ()
{
  return s_classType;
}

inline ShapeType GeoShapeShift::getClassTypeID ()
{
  return s_classTypeID;
}

#endif
