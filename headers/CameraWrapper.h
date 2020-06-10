#ifndef CAMERAWRAPPER_H
#define CAMERAWRAPPER_H

#include <QWidget>
#include <QCamera>
#include <Qt3DExtras/QAbstractCameraController.h>

class CameraWrapper : public QObject{
    Q_OBJECT
    Qt3DCore::QEntity *m_rootEntity;
    Qt3DRender::QCamera *m_camera;
    Qt3DCore::QTransform *m_cameraTransform;
    Qt3DExtras::QAbstractCameraController *m_camController;
public:
    explicit CameraWrapper(Qt3DCore::QEntity *parent = nullptr, Qt3DRender::QCamera *camera=nullptr);
    void addCameraController(Qt3DExtras::QAbstractCameraController *camController);

signals:

public slots:
    void setProjectiveMode(bool isPerspective);
    void enableCameraController(bool isEnble);
    void scaleView(int magnitude);
    void rotateViewX(int degree);
    void rotateViewY(int degree);
    void rotateViewZ(int degree);
};


#endif // CAMERAWRAPPER_H
