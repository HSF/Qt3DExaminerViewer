#ifndef CAMERAWRAPPER_H
#define CAMERAWRAPPER_H

#include <QWidget>
#include <QCamera>
#include <Qt3DExtras/QAbstractCameraController.h>

class CameraWrapper : public QObject{
    Q_OBJECT
public:
    const float init_distanceToOrigin = 50;
private:
    float m_distanceToOrigin;
    float m_longitude, m_latitude;
    float m_roll, m_yaw, m_pitch;
    Qt3DCore::QEntity *m_rootEntity;
    Qt3DRender::QCamera *m_camera;
    Qt3DCore::QTransform *m_cameraTransform;
    Qt3DExtras::QAbstractCameraController *m_camController;
public:
    explicit CameraWrapper(Qt3DCore::QEntity *parent = nullptr, Qt3DRender::QCamera *camera=nullptr);
    void addCameraController(Qt3DExtras::QAbstractCameraController *camController);

signals:

public slots:
    void resetCameraView();
    void setProjectiveMode(bool isPerspective);
    void disableCameraController(bool disEnble);

    void translatePosRad(int radius);
    void translatePosLat(int latitude);
    void translatePosLng(int longitude);

    void rotateViewRoll(int roll);
    void rotateViewYaw(int yaw);
    void rotateViewPitch(int pitch);

private:
    void setDirection();
    void setPosition();
};


#endif // CAMERAWRAPPER_H
