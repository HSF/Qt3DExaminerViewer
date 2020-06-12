#include "headers/MeshModel.h"

#include <QString>

#include <QtCore/QDebug>
#include <QtCore/QString>

#include <QtWidgets/QCommandLinkButton>

#include <Qt3DRender/QObjectPicker>


extern QCommandLinkButton *info;

MeshModel::MeshModel(Qt3DCore::QEntity *rootEntity, Qt3DRender::QMesh *mesh)
    : m_rootEntity(rootEntity){
    // Mesh Transform
    Qt3DCore::QTransform *meshTransform = new Qt3DCore::QTransform();
    meshTransform->setScale(0.006f);
    meshTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 1.0f, 0.0f), 0.0f));
    meshTransform->setTranslation(QVector3D(0.0f, 0.0f, 0.0f));

    // Mesh material
    Qt3DExtras::QPhongMaterial *meshMaterial = new Qt3DExtras::QPhongMaterial();
    meshMaterial->setDiffuse(QColor(QRgb(0xbeb32b)));

    // Mesh picker
    Qt3DRender::QObjectPicker *picker = new Qt3DRender::QObjectPicker();
    picker->setEnabled(true);
    picker->setHoverEnabled(true);

    // Build Mesh Entity
    m_meshEntity = new Qt3DCore::QEntity(m_rootEntity);
    m_meshEntity->addComponent(mesh);
    m_meshEntity->addComponent(meshMaterial);
    m_meshEntity->addComponent(meshTransform);
    /*m_meshEntity->addComponent(picker);
    QObject::connect(picker, &Qt3DRender::QObjectPicker::clicked, this, &MeshModel::changeState);
    QObject::connect(picker, &Qt3DRender::QObjectPicker::clicked, this,[mesh](){ info->setDescription(QString("This is ") + mesh->objectName() +
                                                                                                      QString("\nVertices: ") + mesh->property("Vertices").toString() +
                                                                                                      QString("\nEdges: ") + mesh->property("Edges").toString() +
                                                                                                      QString("\nFaces: ") + mesh->property("Faces").toString());});*/
}

MeshModel::~MeshModel(){
}

void MeshModel::add_subModel(MeshModel *subModel){
    m_subModels.push_back(subModel);
}

void MeshModel::changeState(Qt3DRender::QPickEvent* event){
     Qt3DExtras::QPhongMaterial *material = (Qt3DExtras::QPhongMaterial*)(m_meshEntity->componentsOfType<Qt3DExtras::QPhongMaterial>()[0]);
     material->setDiffuse(QColor(255, 0, 0, 127));

}

void MeshModel::enablePick(bool enable){
    Qt3DRender::QObjectPicker *picker = (Qt3DRender::QObjectPicker*)(m_meshEntity->componentsOfType<Qt3DRender::QObjectPicker>()[0]);
    picker->setEnabled(enable);
}

void MeshModel::restoreState(bool checked){
    for(MeshModel *subModel:m_subModels){
        subModel->restoreState(checked);
    }
    Qt3DExtras::QPhongMaterial *material = (Qt3DExtras::QPhongMaterial*)(m_meshEntity->componentsOfType<Qt3DExtras::QPhongMaterial>()[0]);
    material->setDiffuse(QColor(QRgb(0xbeb32b)));
}

void MeshModel::showMesh(bool visible){
    m_meshEntity->setEnabled(visible);
}

void MeshModel::translateMesh(QVector3D translation){
    for(MeshModel *subModel:m_subModels){
        subModel->translateMesh(translation);
    }
    Qt3DCore::QTransform *transform = (Qt3DCore::QTransform*)(m_meshEntity->componentsOfType<Qt3DCore::QTransform>()[0]);
    transform->setTranslation(translation);
}

void MeshModel::scaleMesh(int magnitude){
    for(MeshModel *subModel:m_subModels){
        subModel->scaleMesh(magnitude);
    }
    float magnitudeF = 0.001 + (float)(magnitude) * 0.01 / 100.0;
    Qt3DCore::QTransform *transform = (Qt3DCore::QTransform*)(m_meshEntity->componentsOfType<Qt3DCore::QTransform>()[0]);
    transform->setScale(magnitudeF);
}

void MeshModel::rotateMeshZ(int degree){
    for(MeshModel *subModel:m_subModels){
        subModel->rotateMeshZ(degree);
    }
    float degreeF = degree * 360.0 / 100.0;
    Qt3DCore::QTransform *transform = (Qt3DCore::QTransform*)(m_meshEntity->componentsOfType<Qt3DCore::QTransform>()[0]);
    transform->setRotationZ(degreeF);
}





