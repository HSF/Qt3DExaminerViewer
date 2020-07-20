/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOCUTVOLACTION_H
#define GEOMODELKERNEL_GEOCUTVOLACTION_H

/**
 * @class GeoCutVolAction
 *
 * @brief Geo Volume action providing a mechanism for Cutoff implementation  
 *
 * The cutoff is applied to the mother Physical Volume and then it is
 * propagated down to all of child volumes.
 *
 * GeoCutVolAction object navigates the input Physical Volume (PV) tree.
 * For each original PV in the tree it creates and a new PV with
 * the same characteristics - material, LV name, PV name, ID, transform
 * in parents coordinate frame - except for shape. The new PV hierarchy
 * is kept by the GeoCutVolAction internally and can be obtained after
 * the action is done by calling getPV() method.
 *
 * When the cut action is finished users have to use the new PV tree
 * obtained with getPV(). The original PV can be dereferenced and
 * the action object destroyed.
 *
 * In order to implement a cutoff, one needs to provide two input
 * arguments to the constructor of GeoCutVolAction
 *   1. A shape to be cut off the mother volume and all its children
 *   2. A transformation of the cutoff shape with respect to the 
 *      coordinate frame of the mother shape
 *
 * Here is an example of GeoCutVolAction usage:
 *
 * |--------------------------------------------------------------------------|
 * | ... First construct a physical volume *origPhysVol* and add all daughter |
 * |     volumes to it                                                        |
 * |                                                                          |
 * |   GeoCutVolAction action(shape,transform);                               |
 * |   pPhysVol->apply(&action);                                              |
 * |   GeoPhysVol* cutPhysVol = action.getPV();                               |
 * |   pPhysVol->unref();                                                     |
 * |                                                                          |
 * | ... Use *cutPhysVol* afterwards (atach to the upper level volumes)       |
 * |--------------------------------------------------------------------------|
 */

#include "GeoModelKernel/GeoVolumeAction.h"
#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoDefinitions.h"
#include <stack>

class GeoPhysVol;

class GeoCutVolAction : public GeoVolumeAction 
{
 public:
  // Constructor parameters:
  // Shape: the shape to be cut
  // Transformation: Shape transformation with respect to the mother Physical Volume
  GeoCutVolAction(const GeoShape& Shape,
		  const GeoTrf::Transform3D& Transform);

  ~GeoCutVolAction();

  virtual void handleVPhysVol (const GeoVPhysVol *);
  
  // Get the cutoff result
  GeoPhysVol* getPV();

 private: 
  GeoPhysVol*               m_physVol;

  const GeoShape&           m_shape;
  GeoTrf::Transform3D      m_transform;

  std::stack<GeoPhysVol*>*  m_copyStack;
};

#endif
