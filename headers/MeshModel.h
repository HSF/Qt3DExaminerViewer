#ifndef SCENEMODIFIER_H
#define SCENEMODIFIER_H

#include <QtWidgets/QLabel>

#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qtransform.h>
#include <QtCore/QObject>

#include <Qt3DRender/qmesh.h>
#include <Qt3DRender/qpickevent.h>

#include <Qt3DExtras/QPhongMaterial>

class MeshModel : public QObject{
    Q_OBJECT

public:
    explicit MeshModel(Qt3DCore::QEntity *rootEntity);
    ~MeshModel();

public slots:
    void showMesh(bool visible);
    void rotateMeshX(int degree);
    void rotateMeshY(int degree);
    void rotateMeshZ(int degree);
    void scaleMesh(int magnitude);
private slots:
    void showInfo(bool isContainsMouse);
    void changeState(Qt3DRender::QPickEvent* event);
    void enablePick(bool enable);
    void restoreState(bool checked);

private:
    Qt3DCore::QEntity *m_rootEntity;
    Qt3DCore::QEntity *m_meshEntity;
};

#endif // SCENEMODIFIER_H
