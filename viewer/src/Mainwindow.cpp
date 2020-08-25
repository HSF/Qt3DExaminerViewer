#include "../headers/MainWindow.h"
#include "../headers/GeneralMeshModel.h"
#include <QDebug>
#include <QKeyEvent>
#include <Qt3DInput/QAnalogAxisInput>
#include <Qt3DInput/QAxis>
#include <Qt3DInput/QMouseDevice>
#include <Qt3DInput/QMouseEvent>

extern GeneralMeshModel *loadedModel;
extern CameraWrapper *cameraWrapper;

MainWindow::MainWindow(): Qt3DExtras::Qt3DWindow(){
}
MainWindow::~MainWindow(){
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
    if (event->button() == Qt::LeftButton)
    {
        if(event->modifiers() == Qt3DInput::QMouseEvent::NoModifier){
            qDebug() << "ok";
            loadedModel->deselect();
            //cameraWrapper->camera();
            //cameraWrapper->setViewCenter(cameraWrapper->camera()->viewCenter());
        }

    }
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
    case Qt::Key_X:
        loadedModel->deselect();
        break;
    default:
        break;
    }
}

