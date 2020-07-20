/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOSERIALTRANSFORMER_H
#define GEOMODELKERNEL_GEOSERIALTRANSFORMER_H

/**
 * @class GeoSerialTransformer
 * 
 * @brief This class takes a physical volume and places it
 * according to a transformation field, N times.
 */

#include "GeoModelKernel/GeoGraphNode.h"
#include "GeoModelKernel/GeoVPhysVol.h"
#include "GeoModelKernel/GeoXF.h"

class GeoSerialTransformer : public GeoGraphNode
{
 public:
  GeoSerialTransformer (const GeoVPhysVol *volume, const GeoXF::Function *func, unsigned int copies);

  //	Executes a GeoNodeAction.
  virtual void exec (GeoNodeAction *action) const;

  //	Returns the transformation field itself.
  const GeoXF::Function * getFunction () const;

  // Returns the volume:
  PVConstLink getVolume () const
  {
    return m_physVol;
  }

  // Returns the number of copies:
  unsigned int getNCopies () const
  {
    return m_nCopies;
  }

  // Returns the transformation to the ith copy:
  GeoTrf::Transform3D getTransform (int i) const
  {
    return (*m_function) (i);
  }

 protected:
  virtual ~GeoSerialTransformer();

 private:
  GeoSerialTransformer(const GeoSerialTransformer &right);
  GeoSerialTransformer & operator=(const GeoSerialTransformer &right);

  //	Number of copies of a physical volume to generate.
  unsigned int m_nCopies;

  //	Recipe for the transformation; specifically, a
  //	Transform-valued m_function of a single variable.
  const GeoXF::Function *m_function;

  //	The physical volume to be multiply placed.
  const GeoVPhysVol *m_physVol;
};

#endif
