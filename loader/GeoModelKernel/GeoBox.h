/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOBOX_H
#define GEOMODELKERNEL_GEOBOX_H

#include "GeoModelKernel/GeoShape.h"

class GeoBox : public GeoShape
{
 public:
  //	Constructor for the BOX.
  GeoBox (double XHalfLength, double YHalfLength, double ZHalfLength);

  //	Returns the volume of the shape, for mass inventory
  virtual double volume () const;

  //	Returns the BOX shape type, as a string.
  virtual const std::string & type () const;

  //	Returns the BOX shape type, as a coded integer.
  virtual ShapeType typeID () const;

  //	 Executes a GeoShapeAction.
  virtual void exec (GeoShapeAction *action) const;

  //	For type identification.
  static const std::string& getClassType ();

  //	For type identification.,
  static ShapeType getClassTypeID ();

  //	Half length in the x-direction.
  const double& getXHalfLength () const;

  //	Half-length in the y direction.
  const double& getYHalfLength () const;

  //	Half-length in the z direction.
  const double& getZHalfLength () const;

 protected:
  virtual ~GeoBox();

  private:
  GeoBox(const GeoBox &right);
  GeoBox & operator=(const GeoBox &right);

  static const std::string s_classType;
  static const ShapeType s_classTypeID;
  double m_xHalfLength;
  double m_yHalfLength;
  double m_zHalfLength;
};

inline const std::string& GeoBox::getClassType ()
{
  return s_classType;
}

inline ShapeType GeoBox::getClassTypeID ()
{
  return s_classTypeID;
}

inline const double& GeoBox::getXHalfLength () const
{
  return m_xHalfLength;
}

inline const double& GeoBox::getYHalfLength () const
{
  return m_yHalfLength;
}

inline const double& GeoBox::getZHalfLength () const
{
  return m_zHalfLength;
}

#endif
