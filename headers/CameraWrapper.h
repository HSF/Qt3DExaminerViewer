#ifndef CAMERAWRAPPER_H
#define CAMERAWRAPPER_H
#include <QWidget>
#include <Qt3DRender/QCamera>
#include <Qt3DExtras/QAbstractCameraController.h>

class CameraWrapper : public QObject{
    Q_OBJECT
    Q_PROPERTY(QVector3D viewCenter WRITE setViewCenter)
    Q_PROPERTY(QVector3D position WRITE setPosition)
    Q_PROPERTY(QVector4D dof4 READ customView WRITE setCustomView )
public:
    const int init_distanceToOrigin = 20;
    const int GLOBAL_CENTER = 0, LOCAL_CENTER = 1;
private:
    int m_distanceToOrigin;
    float m_longitude, m_latitude;
    float m_roll, m_yaw, m_pitch;
    int m_center;
    QVector3D m_bias;
    Qt3DCore::QEntity *m_rootEntity;
    Qt3DRender::QCamera *m_camera;
    Qt3DCore::QTransform *m_cameraTransform;
    Qt3DExtras::QAbstractCameraController *m_camController;
public:
    explicit CameraWrapper(Qt3DCore::QEntity *parent = nullptr, Qt3DRender::QCamera *camera=nullptr);
    void addCameraController(Qt3DExtras::QAbstractCameraController *camController);
    void setCustomView(QVector4D dof4);
    const QVector4D customView();
    void setViewCenter(QVector3D viewCenter);
    void setPosition(QVector3D pos);
    void translateView(QVector3D bias, int scale);
    Qt3DRender::QCamera *camera();
signals:
    void viewCenterChanged(const QVector3D &viewCenter);
public slots:
    void resetCameraView();
    void setProjectiveMode(bool isPerspective);
    void setCoordinateCenter(int index);
    void disableCameraController(bool disEnble);

    void zoomInOut(int extent);
    void translatePosLat(int latitude);
    void translatePosLng(int longitude);

    void rotateViewRoll(int roll);
    void rotateViewYaw(int yaw);
    void rotateViewPitch(int pitch);
private:
    void setDirection();
    void setPosition();
    void translatePosRad(int radius);
    void zoomOrth(int edge);
};


#endif // CAMERAWRAPPER_H
