/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOPOLYHEDRIZEACTION_H
#define GEOMODELKERNEL_GEOPOLYHEDRIZEACTION_H

/**
 * @class GeoPolyhedrizeAction
 *
 * @brief This action polyhedrizes a shape. This includes, for example, boolean shapes.
 * A polhyedral representation is useful when you want to:
 *   -visualize the shape
 *   -compute the shape's volume
 */


#include "GeoModelKernel/GeoShapeAction.h"
class GeoPolyhedron;

class GeoPolyhedrizeAction : public GeoShapeAction
{
 public:
  GeoPolyhedrizeAction();
  virtual ~GeoPolyhedrizeAction();

  //	Handles a shift shape.
  virtual void handleShift (const GeoShapeShift *shift);
  
  //	Handles a union shape.
  virtual void handleUnion (const GeoShapeUnion *unio);
  
  //	Handles an intersection shape.
  virtual void handleIntersection (const GeoShapeIntersection *isect);
  
  //	Handles a subtraction shape.
  virtual void handleSubtraction (const GeoShapeSubtraction *subtract);
  
  //	Handles a box shape.
  virtual void handleBox (const GeoBox *box);
  
  //	Handles a cons shape.
  virtual void handleCons (const GeoCons *cons);
  
  //	Handles a para shape.
  virtual void handlePara (const GeoPara *para);
  
  //	Handles a pcon shape.
  virtual void handlePcon (const GeoPcon *pcon);
  
  //	Handles a pgon shape.
  virtual void handlePgon (const GeoPgon *pgon);
  
  //	Handles a trap shape.
  virtual void handleTrap (const GeoTrap *trap);
  
  //	Handles a  trd shape.
  virtual void handleTrd (const GeoTrd *trd);
  
  //	Handles a tube shape.
  virtual void handleTube (const GeoTube *tube);
  
  //	Handles a tubs shape.
  virtual void handleTubs (const GeoTubs *tubs);
  
  //	Returns the polyhedral representation of a shape.
  const GeoPolyhedron * getPolyhedron () const;
  
  private:
  GeoPolyhedrizeAction(const GeoPolyhedrizeAction &right);
  GeoPolyhedrizeAction & operator=(const GeoPolyhedrizeAction &right);

  //	This polyhedral representation of the shape.
  GeoPolyhedron *m_polyhedron;
};


#endif
