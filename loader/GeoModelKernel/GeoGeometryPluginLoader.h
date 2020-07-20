/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOGEOMETRYPLUGINLOADER_H_
#define GEOGEOMETRYPLUGINLOADER_H_
#include <string> 
class GeoVGeometryPlugin;
class GeoGeometryPluginLoader 
{
 public:

  // Constructor:
  GeoGeometryPluginLoader();

  // Destructor:
  ~GeoGeometryPluginLoader();

  // load Geometry plugin
  GeoVGeometryPlugin *load(const std::string & path) const;
  
 private:
  
  GeoGeometryPluginLoader(const GeoGeometryPluginLoader &)=delete;
  GeoGeometryPluginLoader & operator=(const GeoGeometryPluginLoader &)=delete;

};

#endif
