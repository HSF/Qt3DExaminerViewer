#ifndef CAMERAWRAPPER_H
#define CAMERAWRAPPER_H
#include <QWidget>
#include <Qt3DRender/QCamera>
#include <Qt3DExtras/QAbstractCameraController.h>

class CameraWrapper : public QObject{
    Q_OBJECT
    Q_PROPERTY(QVector3D viewCenter WRITE setViewCenter)
    Q_PROPERTY(QVector3D position WRITE setPosition)
    Q_PROPERTY(QVector4D dof4 WRITE setCustomView READ customView)
public:
    int init_distanceToOrigin;
    explicit CameraWrapper(Qt3DCore::QEntity *parent = nullptr, Qt3DRender::QCamera *camera=nullptr);
    void addCameraController(Qt3DExtras::QAbstractCameraController *camController);
    void setCustomView(QVector4D dof4);
    const QVector4D customView();
    void setViewCenter(QVector3D viewCenter);
    void setPosition(QVector3D pos);
    void translateView(QVector3D bias, int scale);
    Qt3DRender::QCamera *camera();
public slots:
    void resetCameraView();
    void zoomInOut(int extent);
    void viewAll();
    void setProjectiveMode(bool isPerspective);
    void disableCameraController(bool disEnble);
private:
    int m_radius;
    float m_longitude, m_latitude;
    float m_roll, m_yaw, m_pitch;
    QVector3D m_viewCenter;
    Qt3DCore::QEntity *m_rootEntity;
    Qt3DRender::QCamera *m_camera;
    Qt3DCore::QTransform *m_cameraTransform;
    Qt3DExtras::QAbstractCameraController *m_camController;

    void sphericalToDirection();
    void sphericalToPosition();
    void translatePosRad(int radius);
    void zoomOrth(int edge);
};


#endif // CAMERAWRAPPER_H
