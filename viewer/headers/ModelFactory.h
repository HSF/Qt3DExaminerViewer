#ifndef MODOLFACTORY_H
#define MODOLFACTORY_H
#include "GeneralMeshModel.h"
#include <QEntity>
struct Pcon{
    double ZPlane;
    double RMinPlane;
    double RMaxPlane;
};

class ModelFactory
{
public:
    ModelFactory(Qt3DCore::QEntity *rootEntity);
    GeneralMeshModel **build3DText();
    GeneralMeshModel *buildTestVolume();
    GeneralMeshModel *buildCoordinatePlane();
    GeneralMeshModel *buildCoordinateLine();
    GeneralMeshModel *buildTetrahedra();
    GeneralMeshModel *buildTube(double rMin, double rMax, double zHalf);
    GeneralMeshModel *buildTubs(double rMin, double rMax, double zHalf, double SPhi, double DPhi);
    GeneralMeshModel *buildPcon(double SPhi, double DPhi, unsigned int nPlanes, Pcon *planes);
private:
    Qt3DCore::QEntity *m_rootEntity;
};

#endif // MODOLFACTORY_H
