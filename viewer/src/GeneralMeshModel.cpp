#include "../headers/GeneralMeshModel.h"
#include <QGuiApplication>
#include <QtCore/QDebug>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QLabel>
#include <Qt3DRender/qpickevent.h>

GeneralMeshModel::GeneralMeshModel(Qt3DCore::QEntity *rootEntity, Qt3DRender::QGeometryRenderer *mesh, Qt3DRender::QMaterial* mat)
    : m_mesh(mesh), m_parentModel(nullptr), m_isSelectMode(true), m_volume(nullptr)
{
    // Build Mesh Entity
    m_meshEntity = new Qt3DCore::QEntity(rootEntity);

    // Mesh Transform
    m_meshTransform = new Qt3DCore::QTransform();
    m_meshTransform->setScale(1.0f);
    m_meshTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 1.0f, 0.0f), 0.0f));
    m_meshTransform->setTranslation(QVector3D(0.0f, 0.0f, 0.0f));

    // Mesh material
    if(mat) { // use the material defined by the client
        m_meshMaterial = mat;
    } else {  // use a default material
        Qt3DExtras::QPhongMaterial* defaultMat = new Qt3DExtras::QPhongMaterial();
    	defaultMat->setDiffuse(QColor(QRgb(0xbeb32b)));
        m_meshMaterial = defaultMat;
    }
    
    // Mesh picker
    m_picker = new Qt3DRender::QObjectPicker;
    m_picker->setEnabled(true);
    m_picker->setHoverEnabled(true);

    QObject::connect(m_picker, &Qt3DRender::QObjectPicker::exited, this, [](){ info->setDescription(TIPS);});
    QObject::connect(m_picker, &Qt3DRender::QObjectPicker::clicked, [this](Qt3DRender::QPickEvent *event){
        if(event->modifiers() == Qt::ControlModifier && event->button() == Qt3DRender::QPickEvent::LeftButton)
            openVolume();
        else if(event->modifiers() == Qt::AltModifier && event->button() == Qt3DRender::QPickEvent::LeftButton)
            closeVolume();
        else if(event->button() == Qt3DRender::QPickEvent::LeftButton && event->modifiers() == Qt::NoModifier)
            getSelected();
        else {
            // change the focus of camera
            if(event->button() == Qt3DRender::QPickEvent::LeftButton && event->modifiers() == Qt::ShiftModifier){
                cameraWrapper->translateView(event->worldIntersection(), 0);
            }
            else if(event->button() == Qt3DRender::QPickEvent::RightButton && event->modifiers() == Qt::ShiftModifier){
                //camera->camera()->viewEntity(m_meshEntity);
                cameraWrapper->translateView(m_meshTransform->translation(), m_mesh->property("maxLength").toInt());
            }
        }
    });

    // When a GeneralMeshModel is used for world container, m_mesh is nullptr
    if(m_mesh != nullptr){
        // add components to the Entity
        m_meshEntity->addComponent(m_mesh);
        m_meshEntity->addComponent(m_meshMaterial);
        m_meshEntity->addComponent(m_meshTransform);
        m_meshEntity->addComponent(m_picker);
    }
}

GeneralMeshModel::~GeneralMeshModel(){
}

void GeneralMeshModel::setColor(QColor color){
    Qt3DExtras::QPhongMaterial* mat = dynamic_cast<Qt3DExtras::QPhongMaterial*>(m_meshMaterial);
    if(mat) {
    	mat->setDiffuse(color);
    }
};

void GeneralMeshModel::addSubModel(GeneralMeshModel *subModel){
    m_subModels.push_back(subModel);
    subModel->addParentModel(this);
}

int GeneralMeshModel::subModelCount(){
    return m_subModels.count();
}

GeneralMeshModel *GeneralMeshModel::getSubModel(int i){
    return m_subModels.at(i);
}

GeneralMeshModel *GeneralMeshModel::getParentModel(){
    return m_parentModel;
}

void GeneralMeshModel::addParentModel(GeneralMeshModel *parentModel){
    m_parentModel = parentModel;
}

void GeneralMeshModel::getSelected(){
    GeneralMeshModel *topParent = this;
    while(topParent->getParentModel() != nullptr)
        topParent = topParent->getParentModel();
    topParent->deselect();
    setColor(QColor(120, 0, 0, 127));
    QString message;
    if(m_volume != nullptr){
        const GeoMaterial *material = m_volume->getLogVol()->getMaterial();
        message = QString("This is Shape ") + m_mesh->objectName();
        message += QString("\nVolume: %1").arg(m_volume->getLogVol()->getShape()->volume());
        message += QString("\n\nObject Name: ") + QString::fromStdString(m_volume->getLogVol()->getName());
        message += QString("\n\nMaterial Name: ") + QString::fromStdString(material->getName());
        message += QString("\nMaterial Density: %1").arg(material->getDensity());
        unsigned int n = material->getNumElements();
        for(unsigned int i = 0; i < n; i++){
            message += QString("\nElement: ") + QString::fromStdString(material->getElement(i)->getName())
                    + QString(": %1\%").arg(100*material->getFraction(i));
        }
    } else message = QString("Please set volume for this Mesh Model");
    info->setDescription(message);
}

void GeneralMeshModel::deselect(){
    for(GeneralMeshModel *subModel:m_subModels){
        if(subModel != nullptr)
            subModel->deselect();
    }
    setColor(QColor(QRgb(0xbeb32b)));
}

void GeneralMeshModel::setPickMode(bool enable){
    enablePick(enable);
    m_isSelectMode=enable;
}

void GeneralMeshModel::enablePick(bool enable){
    if(enable)
        m_meshEntity->addComponent(m_picker);
    else
        m_meshEntity->removeComponent(m_picker);
}

void GeneralMeshModel::restoreState(bool checked){
    for(GeneralMeshModel *subModel:m_subModels){
        if(subModel != nullptr){
            subModel->restoreState(checked);
            subModel->showMesh(true);
        }
    }
    setColor(QColor(QRgb(0xbeb32b)));
}

void GeneralMeshModel::openVolume(){
    if(m_subModels.size() == 0){
        info->setDescription(QString::fromStdString(m_volume->getLogVol()->getName()) + QString(" has no children"));
        return;
    }
    showMesh(false);
    for(GeneralMeshModel *subModel:m_subModels){
        subModel->showMesh(true);
    }
}

void GeneralMeshModel::closeVolume(){
    if(m_parentModel == nullptr || m_parentModel->objectName()=="world"){
        info->setDescription(QString::fromStdString(m_volume->getLogVol()->getName()) + QString(" has no parent"));
        return;
    }
    m_parentModel->showMesh(true);
    for(GeneralMeshModel *subModel:m_parentModel->m_subModels){
        subModel->showMesh(false);
    }
}

void GeneralMeshModel::showMesh(bool visible){
    for(GeneralMeshModel *subModel:m_subModels){
        subModel->showMesh(false);
        subModel->enablePick(false);
    }
    m_meshEntity->setEnabled(visible);
    if(m_isSelectMode) enablePick(visible);
}

void GeneralMeshModel::setVolume(const GeoVPhysVol *volume){
    m_volume = volume;
}

const GeoVPhysVol *GeneralMeshModel::Volume(){
    return m_volume;
}

void GeneralMeshModel::setTransformMatrix(QMatrix4x4 transform){
    for(GeneralMeshModel *subModel:m_subModels){
        subModel->setTransformMatrix(transform);
    }
    m_meshTransform->setMatrix(transform);
}

void GeneralMeshModel::translateMesh(QVector3D translation){
    for(GeneralMeshModel *subModel:m_subModels){
        subModel->translateMesh(translation);
    }
    m_meshTransform->setTranslation(translation);
}

void GeneralMeshModel::rotateMesh(QQuaternion rotation){
    for(GeneralMeshModel *subModel:m_subModels){
        subModel->rotateMesh(rotation);
    }
    m_meshTransform->setRotation(rotation);
}

void GeneralMeshModel::scaleMesh(QVector3D magnitude){
    for(GeneralMeshModel *subModel:m_subModels){
        subModel->scaleMesh(magnitude);
    }
    m_meshTransform->setScale3D(magnitude);
}





