#ifndef CAMERAWRAPPER_H
#define CAMERAWRAPPER_H

#include <QWidget>
#include <QCamera>

class CameraWrapper : public QWidget
{
    Q_OBJECT
    Qt3DRender::QCamera *m_camera;
public:
    explicit CameraWrapper(QWidget *parent = nullptr, Qt3DRender::QCamera *camera=nullptr);

signals:

public slots:
    void setProjectiveMode(bool isPerspective);
};


#endif // CAMERAWRAPPER_H
