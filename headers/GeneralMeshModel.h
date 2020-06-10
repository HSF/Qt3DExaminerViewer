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

class GeneralMeshModel: public QObject{
    Q_OBJECT
public:
    explicit GeneralMeshModel(Qt3DCore::QEntity *rootEntity, Qt3DRender::QGeometryRenderer *mesh);
    ~GeneralMeshModel();
    void add_subModel(GeneralMeshModel *subModel);

public slots:
    void showMesh(bool visible);
    void translateMesh(QVector3D translation);
    void scaleMesh(int magnitude);
    void unpackSubMesh(bool isUnpack);
private slots:
    void changeState(Qt3DRender::QPickEvent* event);
    void enablePick(bool enable);
    void restoreState(bool checked);


private:
    Qt3DCore::QEntity *m_rootEntity;
    Qt3DCore::QEntity *m_meshEntity;
    QVector<GeneralMeshModel*> m_subModels;
};

#endif // GENERALMESHMODEL_H
