/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOSERIALDENOMINATOR_H
#define GEOMODELKERNEL_GEOSERIALDENOMINATOR_H

/**
 * @class GeoSerialDenominator
 * 
 * @brief This class generates a name for all physical volumes
 * following it.  The name is a prefix string followed by a
 * serial number that starts at zero.  The naming scheme
 * applies to all following sibling physical volumes.  The
 * naming is reset only when a GeoNameTag or another Geo
 * SerialDenominator is found.
 */

#include "GeoModelKernel/GeoGraphNode.h"
#include <string>

class GeoSerialDenominator : public GeoGraphNode
{
 public:
  GeoSerialDenominator (const std::string &BaseName);
  
  //	Executes a GeoNodeAction.
  virtual void exec (GeoNodeAction *action) const;
  
  //	Specifies the name of the next physical volume in the
  //	node tree.
  const std::string& getBaseName () const;

 protected:
  virtual ~GeoSerialDenominator();
  
 private:
  GeoSerialDenominator(const GeoSerialDenominator &right);
  GeoSerialDenominator & operator=(const GeoSerialDenominator &right);
  
  std::string m_baseName;
  
};

inline const std::string& GeoSerialDenominator::getBaseName () const
{
  return m_baseName;
}

#endif
