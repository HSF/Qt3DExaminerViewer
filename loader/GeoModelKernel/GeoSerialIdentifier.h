/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOSERIALIDENTIFIER_H
#define GEOMODELKERNEL_GEOSERIALIDENTIFIER_H

/**
 * @class GeoSerialIdentifier
 *
 * @brief Like a GeoSerialDenominator.  Identifies volumes, including virtual volumes.
 */

#include "GeoModelKernel/GeoGraphNode.h"

class GeoSerialIdentifier : public GeoGraphNode  
{
 public:
  GeoSerialIdentifier(int baseId);

  virtual void exec(GeoNodeAction *action) const;

  inline int getBaseId() const;
  
 protected:
  virtual ~GeoSerialIdentifier();

 private:
  GeoSerialIdentifier(const GeoSerialIdentifier &right);
  const GeoSerialIdentifier & operator=(const GeoSerialIdentifier &right);

  int  m_baseId;
};

inline int GeoSerialIdentifier::getBaseId() const
{
  return m_baseId;
}

#endif


