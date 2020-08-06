#include "../headers/GeneralMeshModel.h"
#include <QGuiApplication>
#include <QtCore/QDebug>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QLabel>
#include <Qt3DRender/qpickevent.h>

GeneralMeshModel::GeneralMeshModel(Qt3DCore::QEntity *rootEntity, Qt3DRender::QGeometryRenderer *mesh, Qt3DRender::QMaterial* mat)
    : m_mesh(mesh), m_parentModel(nullptr), m_isSelectMode(true), m_isVisiable(true)
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
    m_picker = new Qt3DRender::QObjectPicker(m_meshEntity);
    m_picker->setEnabled(true);
    m_picker->setHoverEnabled(true);

    m_meshEntity->addComponent(m_mesh);
    m_meshEntity->addComponent(m_meshMaterial);
    m_meshEntity->addComponent(m_meshTransform);
    m_meshEntity->addComponent(m_picker);
    QObject::connect(m_picker, &Qt3DRender::QObjectPicker::clicked, this, &GeneralMeshModel::unpackSubMesh);
    QObject::connect(m_picker, &Qt3DRender::QObjectPicker::clicked, this, &GeneralMeshModel::changeState);
    QObject::connect(m_picker, &Qt3DRender::QObjectPicker::clicked, this, &GeneralMeshModel::onMoveCamera);
    QObject::connect(m_picker, &Qt3DRender::QObjectPicker::clicked, this, &GeneralMeshModel::packMesh);
    QObject::connect(m_picker, &Qt3DRender::QObjectPicker::exited, this, [](){ info->setDescription(TIPS);});
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

void GeneralMeshModel::addParentModel(GeneralMeshModel *parentModel){
    m_parentModel = parentModel;
}

void GeneralMeshModel::onMoveCamera(Qt3DRender::QPickEvent *event){
     if(event->button() == Qt3DRender::QPickEvent::LeftButton && event->modifiers() == Qt::ShiftModifier){
         camera->translateView(event->worldIntersection(), 0);
     }
     else if(event->button() == Qt3DRender::QPickEvent::RightButton && event->modifiers() == Qt::ShiftModifier){
         camera->camera()->viewEntity(m_meshEntity);
         //camera->translateView(m_meshTransform->translation(), m_mesh->property("maxLength").toInt());
     }
}

void GeneralMeshModel::changeState(Qt3DRender::QPickEvent *event){
    if(event->button() == Qt3DRender::QPickEvent::LeftButton && event->modifiers() == Qt::NoModifier){
        //m_meshMaterial->setDiffuse(QColor(255, 0, 0, 127));
        setColor(QColor(255, 0, 0, 127));
        info->setDescription(QString("This is ") + m_mesh->objectName());
    }
}

void GeneralMeshModel::enablePickAll(bool enable){
    for(GeneralMeshModel *subModel:m_subModels){
        subModel->enablePickAll(enable);
    }
    enablePick(enable & m_isVisiable);
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
    }
    //m_meshMaterial->setDiffuse(QColor(QRgb(0xbeb32b)));
    setColor(QColor(QRgb(0xbeb32b)));
    showMesh(true);
    if(m_isSelectMode) enablePick(true);
}

void GeneralMeshModel::unpackSubMesh(Qt3DRender::QPickEvent* event){
    if(event->modifiers() == Qt::ControlModifier && event->button() == Qt3DRender::QPickEvent::LeftButton){
        if(m_subModels.size() == 0){
            info->setDescription(QString("This volume has no children"));
            return;
        }
        showMesh(false);
        enablePick(false);
        /*for(GeneralMeshModel *subModel:m_subModels){
            subModel->showMesh(true);
            subModel->enablePick(true);
        }*/
    }
}

void GeneralMeshModel::packMesh(Qt3DRender::QPickEvent* event){
    if(event->modifiers() == Qt::AltModifier && event->button() == Qt3DRender::QPickEvent::LeftButton){
        if(m_parentModel == nullptr){
            qInfo() << "has not parent";
            info->setDescription(QString("This volume has no parent"));
            return;
        }
        qInfo() << "has parent";
        /*showMesh(false);
        enablePick(false);*/
        m_parentModel->showMesh(true);
        m_parentModel->enablePick(true);
    }
}

void GeneralMeshModel::showMesh(bool visible){
    m_meshEntity->setEnabled(visible);
    m_isVisiable = visible;
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





