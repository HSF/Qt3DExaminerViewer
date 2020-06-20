#include "headers/GeneralMeshModel.h"
#include <QGuiApplication>
#include <QString>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtWidgets/QCommandLinkButton>
#include <Qt3DRender/QObjectPicker>

GeneralMeshModel::GeneralMeshModel(Qt3DCore::QEntity *rootEntity, Qt3DRender::QGeometryRenderer *mesh)
    : m_rootEntity(rootEntity), m_isSelectMode(true){

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
    m_picker = new Qt3DRender::QObjectPicker(m_meshEntity);
    m_picker->setEnabled(true);
    m_picker->setHoverEnabled(true);

    m_meshEntity->addComponent(mesh);
    m_meshEntity->addComponent(meshMaterial);
    m_meshEntity->addComponent(meshTransform);
    m_meshEntity->addComponent(m_picker);
    QObject::connect(m_picker, &Qt3DRender::QObjectPicker::clicked, this, &GeneralMeshModel::unpackSubMesh);
    QObject::connect(m_picker, &Qt3DRender::QObjectPicker::clicked, this, &GeneralMeshModel::changeState);
    QObject::connect(m_picker, &Qt3DRender::QObjectPicker::clicked, this,[mesh](){ info->setDescription(QString("This is ") + mesh->objectName());});
    QObject::connect(m_picker, &Qt3DRender::QObjectPicker::exited, this, [](){ info->setDescription(QString("move mouse inside a volume to see tips"));});
    QObject::connect(m_picker, &Qt3DRender::QObjectPicker::entered, this, [](){ info->setDescription(QString("1) Left click to select\n"
                                                                                                             "2) CMD/Ctrl + left click to unpack children\n"
                                                                                                             "3) Click \"revert original state\" "
                                                                                                             "button to restore"));});
}

GeneralMeshModel::~GeneralMeshModel(){
}

void GeneralMeshModel::add_subModel(GeneralMeshModel *subModel){
    m_subModels.push_back(subModel);
}

void GeneralMeshModel::changeState(Qt3DRender::QPickEvent* event){
    qInfo() << "clicked position: " << event->position();

    if(event->button() == Qt3DRender::QPickEvent::LeftButton){
        Qt3DExtras::QPhongMaterial *material = (Qt3DExtras::QPhongMaterial*)(m_meshEntity->componentsOfType<Qt3DExtras::QPhongMaterial>()[0]);
        material->setDiffuse(QColor(255, 0, 0, 127));
    }
}

void GeneralMeshModel::enablePickAll(bool enable){
    for(GeneralMeshModel *subModel:m_subModels){
        subModel->enablePickAll(enable);
    }
    enablePick(enable);
    m_isSelectMode=enable;
}

void GeneralMeshModel::enablePick(bool enable){
    if(enable){
        m_meshEntity->addComponent(m_picker);
    }
    else{
        m_meshEntity->removeComponent(m_picker);
    }
}

void GeneralMeshModel::restoreState(bool checked){
    for(GeneralMeshModel *subModel:m_subModels){
        subModel->restoreState(checked);
        subModel->showMesh(false);
    }
    Qt3DExtras::QPhongMaterial *material = (Qt3DExtras::QPhongMaterial*)(m_meshEntity->componentsOfType<Qt3DExtras::QPhongMaterial>()[0]);
    material->setDiffuse(QColor(QRgb(0xbeb32b)));
    showMesh(true);
    if(m_isSelectMode) enablePick(true);
}

void GeneralMeshModel::unpackSubMesh(Qt3DRender::QPickEvent* event){
    if(event->modifiers() == Qt::ControlModifier && event->button() == Qt3DRender::QPickEvent::LeftButton){
        if(m_subModels.size() == 0){
            //info->setDescription(QString("This volume has no children"));
            return;
        }
        showMesh(false);
        enablePick(false);
        for(GeneralMeshModel *subModel:m_subModels){
            subModel->showMesh(true);
            subModel->enablePick(true);
        }
    }
}

void GeneralMeshModel::showMesh(bool visible){
    m_meshEntity->setEnabled(visible);
}

void GeneralMeshModel::translateMesh(QVector3D translation){
    for(GeneralMeshModel *subModel:m_subModels){
        subModel->translateMesh(translation);
    }
    Qt3DCore::QTransform *transform = (Qt3DCore::QTransform*)(m_meshEntity->componentsOfType<Qt3DCore::QTransform>()[0]);
    transform->setTranslation(translation);
}

void GeneralMeshModel::rotateMesh(QQuaternion rotation){
    for(GeneralMeshModel *subModel:m_subModels){
        subModel->rotateMesh(rotation);
    }
    Qt3DCore::QTransform *transform = (Qt3DCore::QTransform*)(m_meshEntity->componentsOfType<Qt3DCore::QTransform>()[0]);
    transform->setRotation(rotation);
}

void GeneralMeshModel::scaleMesh(QVector3D magnitude){
    for(GeneralMeshModel *subModel:m_subModels){
        subModel->scaleMesh(magnitude);
    }
    Qt3DCore::QTransform *transform = (Qt3DCore::QTransform*)(m_meshEntity->componentsOfType<Qt3DCore::QTransform>()[0]);
    transform->setScale3D(magnitude);
}





