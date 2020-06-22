#ifndef CAMERAWRAPPER_H
#define CAMERAWRAPPER_H
#include <QWidget>
#include <QCamera>
#include <Qt3DExtras/QAbstractCameraController.h>

class CameraWrapper : public QObject{
    Q_OBJECT
public:
    const int init_distanceToOrigin = 50;
private:
    int m_distanceToOrigin;
    float m_longitude, m_latitude;
    float m_roll, m_yaw, m_pitch;
    QVector3D m_bias;
    Qt3DCore::QEntity *m_rootEntity;
    Qt3DRender::QCamera *m_camera;
    Qt3DCore::QTransform *m_cameraTransform;
    Qt3DExtras::QAbstractCameraController *m_camController;
public:
    explicit CameraWrapper(Qt3DCore::QEntity *parent = nullptr, Qt3DRender::QCamera *camera=nullptr);
    void addCameraController(Qt3DExtras::QAbstractCameraController *camController);
    void setCustomView(int dis, int lat, int lng, int roll, int yaw, int pitch);
    void translateView(QVector3D bias, int scale);
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
    void interpolateMove(QVector3D endPos, QVector3D endViewCenter);
    void delay(int msec);
};


#endif // CAMERAWRAPPER_H
