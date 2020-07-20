/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

/**
 * @class GeoClearAbsPosAction
 * 
 * @brief Clears absolute postion information from full physical
 *      volume nodes.  This action is initiated from a change in
 *      the alignment of an alignable transformation...although
 *      it may of course be used by other clients, too.
 */

#ifndef GEOMODELKERNEL_GEOCLEARABSPOSACTION_H
#define GEOMODELKERNEL_GEOCLEARABSPOSACTION_H

#include "GeoModelKernel/GeoNodeAction.h"

class GeoClearAbsPosAction : public GeoNodeAction
{
 public:
  GeoClearAbsPosAction();
  virtual ~GeoClearAbsPosAction();

  //	Handles a physical volume.
  virtual void handleFullPhysVol (const GeoFullPhysVol *vol);

 private:
  GeoClearAbsPosAction(const GeoClearAbsPosAction &right);
  GeoClearAbsPosAction & operator=(const GeoClearAbsPosAction &right);

};

#endif
