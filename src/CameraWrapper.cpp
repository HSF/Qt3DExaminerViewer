#include "headers/CameraWrapper.h"
#include <Qt3DCore/qtransform.h>

CameraWrapper::CameraWrapper(Qt3DCore::QEntity *rootEntity,  Qt3DRender::QCamera *camera) : m_rootEntity(rootEntity)
{
    m_camera = camera;
    m_cameraTransform = new Qt3DCore::QTransform();
    m_cameraTransform->setScale(1.0f);
    m_cameraTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 1.0f, 0.0f), 0.0f));
    m_cameraTransform->setTranslation(QVector3D(0.0f, 0.0f, 20.0f));
    m_camera->addComponent(m_cameraTransform);
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

void CameraWrapper::scaleView(int magnitude){
    float magnitudeF = (float)(magnitude - 30);
   // m_camera->setPosition(QVector3D(0, 0, magnitudeF));
    m_cameraTransform->setTranslation(QVector3D(0, 0, magnitudeF));
    qInfo() << "position" << m_camera->position() << endl;
}

void CameraWrapper::rotateViewX(int degree){
    float degreeF = degree * 360.0 / 100.0;
    m_cameraTransform ->setRotationX(degreeF);
}

void CameraWrapper::rotateViewY(int degree){
    float degreeF = degree * 360.0 / 100.0;
    m_cameraTransform->setRotationY(degreeF);
}

void CameraWrapper::rotateViewZ(int degree){
    float degreeF = degree * 360.0 / 100.0;
    m_cameraTransform->setRotationZ(degreeF);
}
