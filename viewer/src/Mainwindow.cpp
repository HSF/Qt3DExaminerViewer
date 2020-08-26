#include "../headers/MainWindow.h"
#include <QDebug>
#include <QKeyEvent>
#include <Qt3DInput/QAnalogAxisInput>
#include <Qt3DInput/QAxis>
#include <Qt3DInput/QMouseDevice>
#include <Qt3DInput/QMouseEvent>

MainWindow::MainWindow():
    Qt3DExtras::Qt3DWindow(){
}
MainWindow::~MainWindow(){
}
void MainWindow::resizeEvent(QResizeEvent* event){
   Qt3DExtras::Qt3DWindow::resizeEvent(event);
   Qt3DRender::QCamera *camera = this->camera();
   if(camera->projectionType() == Qt3DRender::QCameraLens::OrthographicProjection){
       float edge = camera->top();
       camera->setLeft(-edge*camera->aspectRatio());
       camera->setRight(edge*camera->aspectRatio());
   }
}
void MainWindow::receiveModel(CameraWrapper *cameraWrapper, GeneralMeshModel *world){
    m_cameraWrapper = cameraWrapper;
    m_world = world;
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    Qt3DExtras::Qt3DWindow::mousePressEvent(event);
    if (event->button() == Qt::LeftButton){
        if(event->modifiers() == Qt3DInput::QMouseEvent::NoModifier){
            m_world->deselect();
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
    case Qt::Key_X:
        m_world->deselect();
        break;
    default:
        break;
    }
}

