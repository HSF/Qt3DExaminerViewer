#include "headers/CameraWrapper.h"

CameraWrapper::CameraWrapper(Qt3DCore::QEntity *rootEntity,  Qt3DRender::QCamera *camera) : m_rootEntity(rootEntity)
{
    m_camera = camera;
}

void CameraWrapper::setProjectiveMode(bool isPerspective){
    if(isPerspective)
        m_camera->setProjectionType(Qt3DRender::QCameraLens::OrthographicProjection);
    else
        m_camera->setProjectionType(Qt3DRender::QCameraLens::PerspectiveProjection);

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
