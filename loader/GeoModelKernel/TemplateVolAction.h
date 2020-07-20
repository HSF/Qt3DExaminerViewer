/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_TEMPLATEVOLACTION_H
#define GEOMODELKERNEL_TEMPLATEVOLACTION_H

/**
 * @class TemplateVolAction
 * 
 * @brief An example that does nothing, for people to copy and fill in
 */

#include "GeoModelKernel/GeoVolumeAction.h"

class TemplateVolAction : public GeoVolumeAction
{
 public:
  TemplateVolAction();
  ~TemplateVolAction();

  //	Callback method. Overriden by users.
  virtual void handleVPhysVol (const GeoVPhysVol *);
};

#endif
