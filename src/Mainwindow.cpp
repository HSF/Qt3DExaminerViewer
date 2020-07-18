#include "headers/MainWindow.h"
#include <QDebug>
#include <QKeyEvent>
#include <Qt3DInput/QAnalogAxisInput>
#include <Qt3DInput/QAxis>
#include <Qt3DInput/QMouseDevice>

MainWindow::MainWindow(): Qt3DExtras::Qt3DWindow(){
}
MainWindow::~MainWindow(){
   qInfo()<<"windows is closed";
}
void MainWindow::resizeEvent(QResizeEvent* event)
{
   Qt3DExtras::Qt3DWindow::resizeEvent(event);
   Qt3DRender::QCamera *camera = this->camera();
   if(camera->projectionType() == Qt3DRender::QCameraLens::OrthographicProjection){
       float edge = camera->top();
       camera->setLeft(-edge*camera->aspectRatio());
       camera->setRight(edge*camera->aspectRatio());
   }
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    Qt3DExtras::Qt3DWindow::mousePressEvent(event);
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
    case Qt::Key_X:
        qInfo()<<"Key X is pressed";
        break;
    default:
        break;
    }
}

