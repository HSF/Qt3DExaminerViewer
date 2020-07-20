/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEONAMETAG_H
#define GEOMODELKERNEL_GEONAMETAG_H

/**
 * @class GeoNameTag
 *
 * @brief This class gives a name to the next physical volume.
 */

#include "GeoModelKernel/GeoGraphNode.h"
#include <string>

class GeoNameTag : public GeoGraphNode
{
 public:
  //	Constructor for the name tag.  Takes a base name.
  GeoNameTag (const std::string &BaseName);
  
  //	Executes a GeoNodeAction.
  virtual void exec (GeoNodeAction *action) const;

  //	Specifies the name of the next physical volume in the
  //	node tree.
  const std::string& getName () const;
  
 protected:
  virtual ~GeoNameTag();
  
 private:
  GeoNameTag(const GeoNameTag &right);
  GeoNameTag & operator=(const GeoNameTag &right);
  
  std::string m_name;
};

inline const std::string& GeoNameTag::getName () const
{
  return m_name;
}

#endif
