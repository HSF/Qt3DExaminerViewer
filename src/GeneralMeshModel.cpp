#include "headers/GeneralMeshModel.h"

#include <QGuiApplication>

#include <QString>

#include <QtCore/QDebug>
#include <QtCore/QString>

#include <QtWidgets/QCommandLinkButton>

#include <Qt3DRender/QObjectPicker>


extern QCommandLinkButton *info;

GeneralMeshModel::GeneralMeshModel(Qt3DCore::QEntity *rootEntity, Qt3DRender::QGeometryRenderer *mesh)
    : m_rootEntity(rootEntity){

    // Build Mesh Entity
    m_meshEntity = new Qt3DCore::QEntity(m_rootEntity);

    // Mesh Transform
    Qt3DCore::QTransform *meshTransform = new Qt3DCore::QTransform();
    meshTransform->setScale(1.0f);
    meshTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 1.0f, 0.0f), 0.0f));
    meshTransform->setTranslation(QVector3D(0.0f, 0.0f, 0.0f));

    // Mesh material
    Qt3DExtras::QPhongMaterial *meshMaterial = new Qt3DExtras::QPhongMaterial();
    meshMaterial->setDiffuse(QColor(QRgb(0xbeb32b)));

    // Mesh picker
    Qt3DRender::QObjectPicker *picker = new Qt3DRender::QObjectPicker(m_meshEntity);
    picker->setEnabled(true);
    picker->setHoverEnabled(true);


    m_meshEntity->addComponent(mesh);
    m_meshEntity->addComponent(meshMaterial);
    m_meshEntity->addComponent(meshTransform);
    m_meshEntity->addComponent(picker);
    QObject::connect(picker, &Qt3DRender::QObjectPicker::clicked, this, &GeneralMeshModel::unpackSubMesh);
}

GeneralMeshModel::~GeneralMeshModel(){
}

void GeneralMeshModel::add_subModel(GeneralMeshModel *subModel){
    m_subModels.push_back(subModel);
}

void GeneralMeshModel::changeState(Qt3DRender::QPickEvent* event){
     Qt3DExtras::QPhongMaterial *material = (Qt3DExtras::QPhongMaterial*)(m_meshEntity->componentsOfType<Qt3DExtras::QPhongMaterial>()[0]);
     material->setDiffuse(QColor(255, 0, 0, 127));
}

void GeneralMeshModel::enablePick(bool enable){
    Qt3DRender::QObjectPicker *picker = (Qt3DRender::QObjectPicker*)(m_meshEntity->componentsOfType<Qt3DRender::QObjectPicker>()[0]);
    picker->setEnabled(enable);
}

void GeneralMeshModel::restoreState(bool checked){
    for(GeneralMeshModel *subModel:m_subModels){
        subModel->restoreState(checked);
    }
    Qt3DExtras::QPhongMaterial *material = (Qt3DExtras::QPhongMaterial*)(m_meshEntity->componentsOfType<Qt3DExtras::QPhongMaterial>()[0]);
    material->setDiffuse(QColor(QRgb(0xbeb32b)));
    showMesh(true);
}

void GeneralMeshModel::unpackSubMesh(Qt3DRender::QPickEvent* event){
    if(event->modifiers() == Qt::ControlModifier && event->button() == Qt3DRender::QPickEvent::LeftButton){
        showMesh(false);
        for(GeneralMeshModel *subModel:m_subModels){
            subModel->showMesh(true);
        }
        qInfo() << "control + picked" << endl;
    }
    qInfo() << "picked" << endl;
}

void GeneralMeshModel::showMesh(bool visible){
    m_meshEntity->setEnabled(visible);
    enablePick(visible);
}

void GeneralMeshModel::translateMesh(QVector3D translation){
    for(GeneralMeshModel *subModel:m_subModels){
        subModel->translateMesh(translation);
    }
    Qt3DCore::QTransform *transform = (Qt3DCore::QTransform*)(m_meshEntity->componentsOfType<Qt3DCore::QTransform>()[0]);
    transform->setTranslation(translation);
}

void GeneralMeshModel::scaleMesh(int magnitude){
    for(GeneralMeshModel *subModel:m_subModels){
        subModel->scaleMesh(magnitude);
    }
    float magnitudeF = (float)(magnitude);
    Qt3DCore::QTransform *transform = (Qt3DCore::QTransform*)(m_meshEntity->componentsOfType<Qt3DCore::QTransform>()[0]);
    transform->setScale(magnitudeF);
}





