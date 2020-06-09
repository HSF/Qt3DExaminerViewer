#ifndef GENERALMESHMODEL_H
#define GENERALMESHMODEL_H
#include <QWidget>
#include <Qt3DRender/qpickevent.h>
#include <QString>

#include <QtCore/QDebug>
#include <QtCore/QString>


#include <QtWidgets/QLabel>

#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qtransform.h>
#include <QtCore/QObject>

#include <Qt3DRender/qmesh.h>
#include <Qt3DRender/qpickevent.h>

#include <Qt3DExtras/QPhongMaterial>

#include <QtWidgets/QCommandLinkButton>

#include <Qt3DRender/QObjectPicker>

extern QCommandLinkButton *info;

template <class T>
class generalMeshModel: public QObject{
    Q_OBJECT
public:
    explicit generalMeshModel(Qt3DCore::QEntity *rootEntity, T *mesh): m_rootEntity(rootEntity){
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
        m_meshEntity->addComponent(picker);
        QObject::connect(picker, &Qt3DRender::QObjectPicker::clicked, this, &generalMeshModel::changeState);
        QObject::connect(picker, &Qt3DRender::QObjectPicker::clicked, this, &generalMeshModel::showCancelInfo);
        QObject::connect(picker, &Qt3DRender::QObjectPicker::containsMouseChanged, this, &generalMeshModel::showInfo);
    }
    ~generalMeshModel();
    void add_subModel(generalMeshModel *subModel);

public slots:
    void showMesh(bool visible){
        m_meshEntity->setEnabled(visible);
        enablePick(visible);
    }

    void translateMesh(QVector3D translation){
        for(generalMeshModel *subModel:m_subModels){
            subModel->translateMesh(translation);
        }
        Qt3DCore::QTransform *transform = (Qt3DCore::QTransform*)(m_meshEntity->componentsOfType<Qt3DCore::QTransform>()[0]);
        transform->setTranslation(translation);
    }
private slots:
    void showInfo(bool isContainsMouse){
    if(isContainsMouse){
        Qt3DRender::QMesh *mesh = (Qt3DRender::QMesh*)(m_meshEntity->componentsOfType<Qt3DRender::QMesh>()[0]);
        info->setDescription(QString("Vertices: ") + mesh->property("Vertices").toString() +
                             QString("\nEdges: ") + mesh->property("Edges").toString() +
                             QString("\nFaces: ") + mesh->property("Faces").toString());
    }
    else
        info->setDescription(QString("Move cursor close to Volumns for more Info"));
}
    void showCancelInfo(Qt3DRender::QPickEvent* event);
    void changeState(Qt3DRender::QPickEvent* event);
    void enablePick(bool enable);
    void restoreState(bool checked);

private:
    Qt3DCore::QEntity *m_rootEntity;
    Qt3DCore::QEntity *m_meshEntity;

    QVector<generalMeshModel*> m_subModels;
};

#endif // GENERALMESHMODEL_H
