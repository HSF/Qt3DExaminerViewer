#ifndef CAMERAWRAPPER_H
#define CAMERAWRAPPER_H

#include <QWidget>
#include <QCamera>
#include <Qt3DExtras/QAbstractCameraController.h>

class CameraWrapper : public QWidget
{
    Q_OBJECT
    Qt3DRender::QCamera *m_camera;
    Qt3DExtras::QAbstractCameraController *m_camController;
public:
    explicit CameraWrapper(QWidget *parent = nullptr, Qt3DRender::QCamera *camera=nullptr);
    void addCameraController(Qt3DExtras::QAbstractCameraController *camController);

signals:

public slots:
    void setProjectiveMode(bool isPerspective);
    void enableCameraController(bool isEnble);
};


#endif // CAMERAWRAPPER_H
