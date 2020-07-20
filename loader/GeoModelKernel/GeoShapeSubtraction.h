/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOSHAPESUBTRACTION_H
#define GEOMODELKERNEL_GEOSHAPESUBTRACTION_H

#include "GeoModelKernel/GeoShape.h"


#ifndef _GeoShapePersistification_On_
  class Persistifier;
#endif


class GeoShapeSubtraction : public GeoShape
{
 public:
  GeoShapeSubtraction (const GeoShape* A, const GeoShape* B);

  //	Returns the volume of the shape, for mass inventory
  virtual double volume () const;

  //	Returns the NOT shape type, as a string.
  virtual const std::string & type () const;

  //	Returns the NOT shape type, as a coded integer.
  virtual ShapeType typeID () const;

  //	Returns the first operand in the subtraction
  const GeoShape* getOpA () const;

  //	Returns the second operand in the subtraction
  const GeoShape* getOpB () const;

  //	Executes a GeoShapeAction
  virtual void exec (GeoShapeAction *action) const;

  //	For type identification.
  static const std::string& getClassType ();

  //	For type identification.
  static ShapeType getClassTypeID ();

 protected:
  virtual ~GeoShapeSubtraction();

 private:
  GeoShapeSubtraction(const GeoShapeSubtraction &right);
  GeoShapeSubtraction & operator=(const GeoShapeSubtraction &right);

  //	The shape operand in the Subtraction operation
  const GeoShape* m_opA;

  //	The shape operand in the Subtraction operation
  const GeoShape* m_opB;

  static const std::string s_classType;
  static const ShapeType s_classTypeID;

  //    For I/O only!
  GeoShapeSubtraction(){}
  friend Persistifier;

};

inline const std::string& GeoShapeSubtraction::getClassType ()
{
  return s_classType;
}

inline ShapeType GeoShapeSubtraction::getClassTypeID ()
{
  return s_classTypeID;
}

#endif
