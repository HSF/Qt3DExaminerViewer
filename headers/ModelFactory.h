#ifndef MODOLFACTORY_H
#define MODOLFACTORY_H
#include "headers/GeneralMeshModel.h"
#include <QEntity>

class ModelFactory
{
public:
    ModelFactory(Qt3DCore::QEntity *rootEntity);
    void build3DText();
    GeneralMeshModel *buildVolume();
private:
    Qt3DCore::QEntity *m_rootEntity;
};

#endif // MODOLFACTORY_H
