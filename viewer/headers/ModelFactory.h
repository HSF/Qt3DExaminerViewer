#ifndef MODOLFACTORY_H
#define MODOLFACTORY_H
#include "GeneralMeshModel.h"
#include <QEntity>
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
    GeneralMeshModel *buildTube(double rMin, double rMax, double zHalf);
    GeneralMeshModel *buildTubs(double rMin, double rMax, double zHalf, double SPhi, double DPhi);
    GeneralMeshModel *buildPcon(double SPhi, double DPhi, unsigned int nPlanes, Pcon *planes);
};

#endif // MODOLFACTORY_H
