/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOIDENTIFIERTAG_H
#define GEOMODELKERNEL_GEOIDENTIFIERTAG_H

/**
 * @class GeoIdentifierTag
 * 
 * @brief This class gives an integral identifer to the following physical volume.
 *
 * Courtesy of Philipp Shieferdecker (LMU Munich).
 */

#include "GeoModelKernel/GeoGraphNode.h"

class GeoIdentifierTag : public GeoGraphNode
{
 public:
  //	Constructor for the identifier tag. Takes an unsigned int
  GeoIdentifierTag (int id);

  //	Executes a GeoNodeAction.
  virtual void exec (GeoNodeAction *action) const;
  
  //	Specifies the identifier of the next physical volume in
  //	the node tree.
  int getIdentifier () const;
  
 protected:
  virtual ~GeoIdentifierTag();

 private:
  GeoIdentifierTag(const GeoIdentifierTag &right);
  GeoIdentifierTag & operator=(const GeoIdentifierTag &right);
  
  int m_identifier;  
};

inline int GeoIdentifierTag::getIdentifier () const
{
  return m_identifier;
}

#endif
