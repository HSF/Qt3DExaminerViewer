#ifndef MODOLFACTORY_H
#define MODOLFACTORY_H
#include "GeneralMeshModel.h"
#include <QEntity>
#include <GeoModelKernel/GeoTessellatedSolid.h>
struct Pcon{
    double ZPlane;
    double RMinPlane;
    double RMaxPlane;
};

class ModelFactory{

protected:
    ModelFactory(Qt3DCore::QEntity *rootEntity);
    static ModelFactory* m_singleton;
    Qt3DCore::QEntity *m_rootEntity;
    float m_maxSize=10;

public:
    /**
     * Singletons should not be cloneable.
     */
    ModelFactory(ModelFactory &other) = delete;
    /**
     * Singletons should not be assignable.
     */
    void operator=(const ModelFactory &) = delete;
    /**
     * This is the static method that controls the access to the singleton
     * instance. On the first run, it creates a singleton object and places it
     * into the static field. On subsequent runs, it returns the client existing
     * object stored in the static field.
     */
    static ModelFactory *GetInstance(Qt3DCore::QEntity *rootEntity);

    void setMaxSize(float size);
    float MaxSize();
    GeneralMeshModel **build3DText();
    GeneralMeshModel *buildTestVolume();
    GeneralMeshModel *buildCoordinatePlane();
    GeneralMeshModel *buildCoordinateLine();
    GeneralMeshModel *buildTetrahedra();
    GeneralMeshModel *buildBox(double xHalf, double yHalf, double zHalf);
    GeneralMeshModel *buildTube(double rMin, double rMax, double zHalf);
    GeneralMeshModel *buildTubs(double rMin, double rMax, double zHalf, double SPhi, double DPhi);
    GeneralMeshModel *buildPcon(double SPhi, double DPhi, unsigned int nPlanes, Pcon *planes);
    GeneralMeshModel *buildCons(double rMin1, double rMin2, double rMax1, double rMax2, double zHalf, double SPhi, double DPhi);
    GeneralMeshModel *buildTorus(double rMin, double rMax, double rTor, double SPhi, double DPhi);
    GeneralMeshModel *buildTessellatedSolid(size_t num, GeoFacet **faces);
};

#endif // MODOLFACTORY_H
