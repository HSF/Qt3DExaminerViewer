#ifndef MODOLFACTORY_H
#define MODOLFACTORY_H
#include "GeneralMeshModel.h"
#include <QEntity>

class ModelFactory
{
public:
    ModelFactory(Qt3DCore::QEntity *rootEntity);
    GeneralMeshModel **build3DText();
    GeneralMeshModel *buildTestVolume();
    GeneralMeshModel *buildLineOne();
    GeneralMeshModel *buildLineTwo();
    GeneralMeshModel *buildTetrahedra();

private:
    Qt3DCore::QEntity *m_rootEntity;
};

#endif // MODOLFACTORY_H
