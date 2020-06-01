#include "CameraWrapper.h"

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
