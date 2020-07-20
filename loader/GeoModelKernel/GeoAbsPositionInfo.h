/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_GEOABSPOSITIONINFO_H
#define GEOMODELKERNEL_GEOABSPOSITIONINFO_H

/**
 * @class GeoAbsPositionInfo
 * 
 * @brief This class represents anything whose default and aligned
 * postions are known and cached.
 */

#include <GeoModelKernel/GeoDefinitions.h>

class GeoAbsPositionInfo 
{
 public:
  GeoAbsPositionInfo();
  ~GeoAbsPositionInfo();

  //	Returns the default absolute transform.
  const GeoTrf::Transform3D * getAbsTransform () const;

  //	Returns the default absolute transform.
  const GeoTrf::Transform3D * getDefAbsTransform () const;

  
  //	Clears the absolute transform.
  void clearAbsTransform ();

  //	Clears the default absolute transform.
  void clearDefAbsTransform ();

  //	Sets the absolute transform.
  void setAbsTransform (const GeoTrf::Transform3D &  xform);

  //	Sets the absolute transform.
  void setDefAbsTransform (const GeoTrf::Transform3D &  xform);

 private:
  GeoAbsPositionInfo(const GeoAbsPositionInfo &right);
  GeoAbsPositionInfo & operator=(const GeoAbsPositionInfo &right);

  //	The absolute transform from the world coord down to this
  //	positioned object.
  GeoTrf::Transform3D *m_absTransform;
  
  //	The default absolute transform from the world coord down
  //	to this positioned object.
  GeoTrf::Transform3D *m_defAbsTransform;
};

inline const GeoTrf::Transform3D * GeoAbsPositionInfo::getAbsTransform () const
{
  return m_absTransform;
}

inline const GeoTrf::Transform3D * GeoAbsPositionInfo::getDefAbsTransform () const
{
  return m_defAbsTransform;
}

#endif
