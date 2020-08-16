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
                     "2) press X key to cancel selected volume\n" \
                     "3) CMD/Ctrl + left click to open volume\n" \
                     "4) Option/Alt + left click to close volume\n" \
                     "5) Shift + left click to focus on clicked surface point\n" \
                     "6) Shift + right click to focus on clicked object volume center\n" \
                     "7) Ctrl(Mac) + left click to rotate\n" \
                     "8) left click to translate view\n")

extern QCommandLinkButton *info;
extern CameraWrapper *camera;

class GeneralMeshModel: public QObject{
    Q_OBJECT
public:
    explicit GeneralMeshModel(Qt3DCore::QEntity *rootEntity, Qt3DRender::QGeometryRenderer *mesh, Qt3DRender::QMaterial* mat = nullptr /*if null, a default material will be used*/);
    ~GeneralMeshModel();
    void addSubModel(GeneralMeshModel *subModel);
    void addParentModel(GeneralMeshModel *parentModel);
    GeneralMeshModel *subModel(int i);
    int subModelCount();
    void setColor(QColor color);
signals:
    void mouseEnter(QString tip);

public slots:
    void showMesh(bool visible);
    void translateMesh(QVector3D translation);
    void rotateMesh(QQuaternion rotation);
    void scaleMesh(QVector3D magnitude);
    void unpackSubMesh(Qt3DRender::QPickEvent* event);
    void packMesh(Qt3DRender::QPickEvent* event);
    void setPickMode(bool enable);
    void restoreState(bool checked);
    void disselect();

private slots:
    void changeState(Qt3DRender::QPickEvent* event);
    void enablePick(bool enable);
    void onMoveCamera(Qt3DRender::QPickEvent *event);

private:
    Qt3DCore::QEntity *m_meshEntity;
    Qt3DRender::QGeometryRenderer *m_mesh;
    Qt3DCore::QTransform *m_meshTransform;
    //Qt3DExtras::QPhongMaterial *m_meshMaterial;
    Qt3DRender::QMaterial *m_meshMaterial;
    Qt3DRender::QObjectPicker *m_picker;
    QVector<GeneralMeshModel*> m_subModels;
    GeneralMeshModel *m_parentModel;
    bool m_isSelectMode;
    bool m_isVisiable;
};

#endif // GENERALMESHMODEL_H
