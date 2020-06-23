#include "headers/Mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(): Qt3DExtras::Qt3DWindow(){
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
