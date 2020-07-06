#ifndef GENERALMESHMODEL_H
#define GENERALMESHMODEL_H
#include "CameraWrapper.h"
#include <QWidget>
#include <QCamera>
#include <QtCore/QString>
#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qtransform.h>
#include <QtCore/QObject>
#include <Qt3DRender/qmesh.h>
#include <Qt3DExtras/QPhongMaterial>
#include <QtWidgets/QCommandLinkButton>
#include <Qt3DRender/QObjectPicker>

#define TIPS QString("1) Left click to highlight\n" \
           "2) CMD/Ctrl + left click to open volume\n" \
           "3) Option/Alt + left click to close volume\n" \
           "4) Shift + left click to focus on clicked surface point\n" \
           "5) Shift + right click to focus on clicked object volume center")

extern QCommandLinkButton *info;
extern CameraWrapper *camera;

class GeneralMeshModel: public QObject{
    Q_OBJECT
public:
    explicit GeneralMeshModel(Qt3DCore::QEntity *rootEntity, Qt3DRender::QGeometryRenderer *mesh);
    ~GeneralMeshModel();
    void add_subModel(GeneralMeshModel *subModel);
    void add_parentModel(GeneralMeshModel *parentModel);

signals:
    void mouseEnter(QString tip);

public slots:
    void showMesh(bool visible);
    void translateMesh(QVector3D translation);
    void rotateMesh(QQuaternion rotation);
    void scaleMesh(QVector3D magnitude);
    void unpackSubMesh(Qt3DRender::QPickEvent* event);
    void packMesh(Qt3DRender::QPickEvent* event);
    void enablePickAll(bool enable);

private slots:
    void changeState(Qt3DRender::QPickEvent* event);
    void restoreState(bool checked);
    void enablePick(bool enable);
    void onMoveCamera(Qt3DRender::QPickEvent *event);

private:
    Qt3DCore::QEntity *m_meshEntity;
    Qt3DRender::QGeometryRenderer *m_mesh;
    Qt3DCore::QTransform *m_meshTransform;
    Qt3DExtras::QPhongMaterial *m_meshMaterial;
    Qt3DRender::QObjectPicker *m_picker;
    QVector<GeneralMeshModel*> m_subModels;
    GeneralMeshModel *m_parentModel;
    bool m_isSelectMode;
    bool m_isVisiable;
};

#endif // GENERALMESHMODEL_H
