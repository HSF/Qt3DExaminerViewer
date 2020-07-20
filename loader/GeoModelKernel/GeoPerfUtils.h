/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOPERFUTILS_H
#define GEOMODELKERNEL_GEOPERFUTILS_H

/**
 * @class GeoPerfUtils
 *
 * @brief This is a collection of performance utilities used to 
 * benchmark the GeoModel.  Currently it includes a routine
 * to fetch the current memory consumption in kilobytes and
 * a routine to fetch the current CPU consumption in jiffies.
 *
 * Joe Boudreau March 2005.
 */

class GeoPerfUtils {

 public:

  // Get the current Memory Usage (kbytes);
  static int getMem();

  // Get the current CPU Usage (jiffies= 1/100th of a second):
  static int getCpu();

};

#endif
