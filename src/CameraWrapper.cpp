#include "headers/CameraWrapper.h"
#include <Qt3DCore/qtransform.h>
#include <QtMath>

CameraWrapper::CameraWrapper(Qt3DCore::QEntity *rootEntity,  Qt3DRender::QCamera *camera) : m_rootEntity(rootEntity)
{
    m_camera = camera;
    float x0 = m_camera->position()[0];
    float y0 = m_camera->position()[1];
    float z0 = m_camera->position()[2];
    m_distanceToOrigin = qSqrt(qPow(x0,2) + qPow(z0,2) + qPow(y0,2));
    m_latitude = qAsin(y0/m_distanceToOrigin);
    m_longitude = qAtan2(x0,z0);
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
    // prevent divided by zero later
    magnitude += 1;
    QVector3D position = m_camera -> position();
    m_camera->setPosition(position * magnitude / m_distanceToOrigin);
    m_distanceToOrigin = (float)(magnitude);
}

void CameraWrapper::rotateViewX(int degree){
    m_latitude = (degree - 50) * M_PI / 2 / 50.0;
    float y = m_distanceToOrigin * qSin(m_latitude);
    float x = m_distanceToOrigin * qCos(m_latitude) * qSin(m_longitude);
    float z = m_distanceToOrigin * qCos(m_latitude) * qCos(m_longitude);
    m_camera -> setPosition(QVector3D(x, y, z));
    m_camera -> setViewCenter(QVector3D(0, 0, 0));
    float upVectorY, upVectorX, upVectorZ;
    upVectorY = qCos(m_latitude);
    upVectorX = - qSin(m_latitude) * qSin(m_longitude);
    upVectorZ = - qSin(m_latitude) * qCos(m_longitude);
    m_camera -> setUpVector(QVector3D(upVectorX, upVectorY, upVectorZ));
}

void CameraWrapper::rotateViewY(int degree){
    m_longitude = degree * 2 * M_PI / 100.0;
    float x0 = m_camera->position()[0];
    float y0 = m_camera->position()[1];
    float z0 = m_camera->position()[2];
    float xzNorm = qSqrt(qPow(x0,2) + qPow(z0,2));
    float x = xzNorm * qSin(m_longitude);
    float z = xzNorm * qCos(m_longitude);
    m_camera -> setPosition(QVector3D(x, y0, z));
    m_camera -> setViewCenter(QVector3D(0, 0, 0));
    float upVectorY, upVectorX, upVectorZ;
    upVectorY = qCos(m_latitude);
    upVectorX = - qSin(qAbs(m_latitude)) * qSin(m_longitude);
    upVectorZ = - qSin(qAbs(m_latitude)) * qCos(m_longitude);
    m_camera -> setUpVector(QVector3D(upVectorX, upVectorY, upVectorZ));
}

void CameraWrapper::rotateViewZ(int degree){
    m_roll = degree * 2 * M_PI / 100.0;
    float x = qCos(m_roll);
    float y = qSin(m_roll);
    QVector3D position = m_camera->position();
    QVector3D extraAxisY = QVector3D::crossProduct(position.normalized(), QVector3D(0,1,0));
    QVector3D extraAxisX = QVector3D::crossProduct(-position.normalized(), extraAxisY);
    QVector3D newUpVector = x * extraAxisX + y * extraAxisY;
    m_camera -> setUpVector(newUpVector);
}
