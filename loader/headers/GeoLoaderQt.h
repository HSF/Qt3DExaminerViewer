#ifndef GEOLOADERQT_H
#define GEOLOADERQT_H
#include <viewer/headers/GeneralMeshModel.h>

class GeoLoaderQt{
public:
	GeoLoaderQt(Qt3DCore::QEntity *rootEntity);
	GeneralMeshModel *loadCreate(QString path);
private:
	GeoPhysVol* createTheWorld(GeoPhysVol* world)
	GMDBManager* checkPath(QString path);
	GeoPhysVol* introWorld(GMDBManager *db);
	GeoVPhysVol* introChild(PVConstLink nodeLink);

	GeneralMeshModel *createBox( const GeoShape* shapeIn);
	GeneralMeshModel *createTube(const GeoShape* shapeIn);
	GeneralMeshModel *createTubs(const GeoShape* shapeIn);
	GeneralMeshModel *createPcon(const GeoShape* shapeIn);
}


#endif