#include "headers/Mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent): Qt3DExtras::Qt3DWindow(){
}

void MainWindow::addCamera(Qt3DRender::QCamera *camera){
    m_camera = camera;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   Qt3DExtras::Qt3DWindow::resizeEvent(event);
   if(m_camera->projectionType() == Qt3DRender::QCameraLens::OrthographicProjection){
       float edge = m_camera->top();
       m_camera->setLeft(-edge*m_camera->aspectRatio());
       m_camera->setRight(edge*m_camera->aspectRatio());
   }
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    Qt3DExtras::Qt3DWindow::mousePressEvent(event);
}
