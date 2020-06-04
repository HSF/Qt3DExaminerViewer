#include "headers/CameraWrapper.h"

CameraWrapper::CameraWrapper(QWidget *parent,  Qt3DRender::QCamera *camera) : QWidget(parent)
{
    m_camera = camera;
}

void CameraWrapper::setProjectiveMode(bool isPerspective){
    if(isPerspective)
        m_camera->setProjectionType(Qt3DRender::QCameraLens::PerspectiveProjection);
    else
        m_camera->setProjectionType(Qt3DRender::QCameraLens::OrthographicProjection);
}


void CameraWrapper::addCameraController(Qt3DExtras::QAbstractCameraController *camController){
    m_camController = camController;
    m_camController ->setCamera(m_camera);
}

void CameraWrapper::enableCameraController(bool isEnble){
    if(isEnble)
        m_camController ->setCamera(nullptr);
    else
        m_camController ->setCamera(m_camera);
}
