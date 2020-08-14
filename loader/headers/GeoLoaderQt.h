#ifndef GEOLOADERQT_H
#define GEOLOADERQT_H

#include "viewer/headers/GeneralMeshModel.h"
#include "viewer/headers/ModelFactory.h"

// GeoModel includes
#include <GeoModelDBManager/GMDBManager.h>
#include <GeoModelRead/ReadGeoModel.h>
#include <GeoModelKernel/GeoPhysVol.h>
#include <GeoModelKernel/GeoFullPhysVol.h>
// GeoModel shapes
#include <GeoModelKernel/GeoBox.h>
#include <GeoModelKernel/GeoTube.h>
#include <GeoModelKernel/GeoTubs.h>
#include <GeoModelKernel/GeoPcon.h>

class GeoLoaderQt{
public:
	GeoLoaderQt(Qt3DCore::QEntity *rootEntity);
    GeneralMeshModel *loadFromDB(QString path);
private:
    GeoPhysVol* createTheWorld(GeoPhysVol* world);
    GMDBManager* checkPath(QString path);
    GeoPhysVol* introWorld(GMDBManager *db);
    const GeoVPhysVol* introChild(PVConstLink nodeLink);

	GeneralMeshModel *createBox( const GeoShape* shapeIn);
	GeneralMeshModel *createTube(const GeoShape* shapeIn);
	GeneralMeshModel *createTubs(const GeoShape* shapeIn);
    GeneralMeshModel *createPcon(const GeoShape* shapeIn);
    Qt3DCore::QEntity *m_rootEntity;
    ModelFactory *m_builder;
};


#endif
