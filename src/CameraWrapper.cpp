#include "headers/CameraWrapper.h"
#include <QCoreApplication>
#include <QTime>
#include <Qt3DCore/qtransform.h>
#include <QtMath>

CameraWrapper::CameraWrapper(Qt3DCore::QEntity *rootEntity,  Qt3DRender::QCamera *camera) : m_rootEntity(rootEntity)
{
    m_camera = camera;
    m_camera->setProjectionType(Qt3DRender::QCameraLens::PerspectiveProjection);
    resetCameraView();
}

Qt3DRender::QCamera *CameraWrapper::camera(){
    return m_camera;
}

void CameraWrapper::resetCameraView(){
    m_camera->setPosition(QVector3D(0, 0, init_distanceToOrigin));
    m_camera->setUpVector(QVector3D(0, 1, 0));
    m_camera->setViewCenter(QVector3D(0, 0, 0));
    m_distanceToOrigin = init_distanceToOrigin;
    m_latitude = 0;
    m_longitude = 0;
    m_pitch = 0;
    m_roll = 0;
    m_yaw = M_PI;
    m_bias = QVector3D(0.0f, 0.0f, 0.0f);
}

void CameraWrapper::setCustomView(int dis, int lat, int lng, int pitch, int yaw, int roll){
    //m_distanceToOrigin = dis;
    m_latitude = qDegreesToRadians((float)lat);
    m_longitude = qDegreesToRadians((float)lng);
    m_roll = qDegreesToRadians((float)roll);
    m_yaw = qDegreesToRadians((float)yaw);
    m_pitch = qDegreesToRadians((float)pitch);
    setPosition();
    setDirection();
}

void CameraWrapper::setProjectiveMode(bool isPerspective){
    if(!isPerspective)
        m_camera->setProjectionType(Qt3DRender::QCameraLens::OrthographicProjection);
    else
        m_camera->setProjectionType(Qt3DRender::QCameraLens::PerspectiveProjection);

}

void CameraWrapper::addCameraController(Qt3DExtras::QAbstractCameraController *camController){
    m_camController = camController;
    m_camController ->setCamera(m_camera);
}

void CameraWrapper::disableCameraController(bool disEnble){
    if(disEnble)
        m_camController ->setCamera(nullptr);
    else
        m_camController ->setCamera(m_camera);
}

void CameraWrapper::zoomInOut(int extent){
    if(m_camera->projectionType() == Qt3DRender::QCameraLens::PerspectiveProjection){
        translatePosRad(extent);
    }
    else{
        zoomOrth(extent);
    }
}

void CameraWrapper::translateView(QVector3D bias, int scale){
    m_bias = bias;
    if(scale != 0 && m_camera->projectionType() == Qt3DRender::QCameraLens::PerspectiveProjection){
        m_distanceToOrigin = 10 * scale;
        m_camera->setPosition(m_distanceToOrigin*(-m_camera->viewVector()).normalized() + bias);
    }
    m_camera->setViewCenter(bias);
}

void CameraWrapper::zoomOrth(int edge){
    float edgeF = edge * 20.0  / 100.0;
    m_camera->setTop(edgeF);
    m_camera->setBottom(-edgeF);
    m_camera->setLeft(-edgeF*m_camera->aspectRatio());
    m_camera->setRight(edgeF*m_camera->aspectRatio());
}

void CameraWrapper::translatePosRad(int radius){
    // prevent divided by zero later
    QVector3D position = m_camera -> position();
    m_camera->setPosition(position * radius / m_distanceToOrigin);
    m_distanceToOrigin = radius;
}

void CameraWrapper::translatePosLat(int latitude){
    m_latitude = qDegreesToRadians((float)latitude);;
    setPosition();
}

void CameraWrapper::translatePosLng(int longitude){
    m_longitude = qDegreesToRadians((float)longitude);
    setPosition();
}


void CameraWrapper::rotateViewRoll(int roll){
    m_roll = qDegreesToRadians((float)roll);
    setDirection();
}

void CameraWrapper::rotateViewYaw(int yaw){
    m_yaw = qDegreesToRadians((float)yaw);
    setDirection();
}

void CameraWrapper::rotateViewPitch(int pitch){
    m_pitch = qDegreesToRadians((float)pitch);
    setDirection();
}

void CameraWrapper::setPosition(){
    float y = m_distanceToOrigin * qSin(m_latitude);
    float x = m_distanceToOrigin * qCos(m_latitude) * qSin(m_longitude);
    float z = m_distanceToOrigin * qCos(m_latitude) * qCos(m_longitude);
    m_camera -> setPosition(m_bias + QVector3D(x, y, z));
   /* m_camera -> setViewCenter(QVector3D(0, 0, 0));
    float upVectorY, upVectorX, upVectorZ;
    upVectorY = qCos(m_latitude);
    upVectorX = - qSin(m_latitude) * qSin(m_longitude);
    upVectorZ = - qSin(m_latitude) * qCos(m_longitude);
    m_camera -> setUpVector(QVector3D(upVectorX, upVectorY, upVectorZ));*/
}
void CameraWrapper::setDirection(){
    float y = qSin(m_pitch);
    float x = qCos(m_pitch) * qSin(m_yaw);
    float z = qCos(m_pitch) * qCos(m_yaw);
    QVector3D viewDirection = QVector3D(x, y, z);
    QVector3D viewCenter = m_camera->position() + init_distanceToOrigin * viewDirection;
    m_camera->setViewCenter(viewCenter);

    x = qCos(m_roll);
    y = qSin(m_roll);
    QVector3D extraAxisY;
    QVector3D extraAxisX;
    if(qAbs(m_pitch + M_PI/2) < 1e-2) {
        extraAxisX = qCos(m_yaw) * QVector3D(0, 0, 1) + qSin(m_yaw) * QVector3D(1, 0, 0);
        extraAxisY = qSin(m_yaw) * QVector3D(0, 0, 1) - qCos(m_yaw) * QVector3D(1, 0, 0);
    }
    else if(qAbs(m_pitch - M_PI/2) < 1e-2) {
        extraAxisX = -qCos(m_yaw) * QVector3D(0, 0, 1) - qSin(m_yaw) * QVector3D(1, 0, 0);
        extraAxisY = -qSin(m_yaw) * QVector3D(0, 0, 1) + qCos(m_yaw) * QVector3D(1, 0, 0);
    }
    else {
        extraAxisY = QVector3D::crossProduct(viewDirection, QVector3D(0,1,0));
        extraAxisX = QVector3D::crossProduct(extraAxisY, viewDirection);
    }
    QVector3D newUpVector = x * extraAxisX + y * extraAxisY;
    m_camera -> setUpVector(newUpVector);
    //qInfo() << "position: " << m_camera -> position();
    //qInfo() << "upvector: " << m_camera -> upVector();
    //qInfo() << "viewCenter: " << m_camera -> viewCenter();
}
