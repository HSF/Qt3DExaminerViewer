/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOCOUNTVOLACTION_H
#define GEOMODELKERNEL_GEOCOUNTVOLACTION_H

#include "GeoModelKernel/GeoNodeAction.h"

class GeoCountVolAction : public GeoNodeAction
{
 public:
  GeoCountVolAction ();
  virtual ~GeoCountVolAction();

  //	Handles a physical volume.
  virtual void handlePhysVol (const GeoPhysVol *);
  
  //	Handles a physical volume.
  virtual void handleFullPhysVol (const GeoFullPhysVol *);
  
  //	Handles a Serial Transformer
  virtual void handleSerialTransformer (const GeoSerialTransformer  *st);

  //	The volume count.
  unsigned int getCount () const;

 private:
  GeoCountVolAction(const GeoCountVolAction &right);
  GeoCountVolAction & operator=(const GeoCountVolAction &right);

  unsigned int m_count;
};

inline unsigned int GeoCountVolAction::getCount () const
{
  return m_count;
}

#endif
