#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "CameraWrapper.h"
#include "GeneralMeshModel.h"
#include <Qt3DRender/QCamera>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DInput/QLogicalDevice>

class MainWindow : public Qt3DExtras::Qt3DWindow{
    Q_OBJECT
public:
    explicit MainWindow();
    void receiveModel(CameraWrapper *cameraWrapper, GeneralMeshModel *world);
     ~MainWindow();
private:
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    CameraWrapper *m_cameraWrapper;
    GeneralMeshModel *m_world;

};

#endif // MAINWINDOW_H
