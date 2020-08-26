#include "viewer/headers/CameraWrapper.h"
#include <QCoreApplication>
#include <QTime>
#include <Qt3DCore/qtransform.h>
#include <QtMath>
#include <QPropertyAnimation>

CameraWrapper::CameraWrapper(Qt3DCore::QEntity *rootEntity,  Qt3DRender::QCamera *camera) : m_rootEntity(rootEntity){
    m_camera = camera;
    m_camera->setProjectionType(Qt3DRender::QCameraLens::PerspectiveProjection);
    resetCameraView();
}

Qt3DRender::QCamera *CameraWrapper::camera(){
    return m_camera;
}

void CameraWrapper::resetCameraView(){
    m_camera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, init_distanceToOrigin*20);
    m_camera->setPosition(QVector3D(0, 0, init_distanceToOrigin));
    m_camera->setUpVector(QVector3D(0, 1, 0));
    m_camera->setViewCenter(QVector3D(0, 0, 0));
    m_radius = init_distanceToOrigin;
    m_latitude = 0;
    m_longitude = 0;
    m_pitch = 0;
    m_roll = 0;
    m_yaw = M_PI;
    m_viewCenter = QVector3D(0.0f, 0.0f, 0.0f);
}

void CameraWrapper::viewAll(){
    zoomInOut(init_distanceToOrigin);
}

void CameraWrapper::setCustomView(QVector4D dof4){
    m_radius = dof4[0];
    m_latitude = qDegreesToRadians((float)dof4[1]);
    m_longitude = qDegreesToRadians((float)dof4[2]);
    m_pitch = -m_latitude;
    m_yaw = m_longitude + M_PI;
    m_roll = qDegreesToRadians((float)dof4[3]);
    sphericalToPosition();
    sphericalToDirection();
}

const QVector4D CameraWrapper::customView(){
     return QVector4D(m_radius, int(qRadiansToDegrees(m_latitude)), int(qRadiansToDegrees(m_longitude)),
                      int(qRadiansToDegrees(m_roll)));
}

void CameraWrapper::setProjectiveMode(bool isPerspective){
    if(!isPerspective){
        m_camera->setProjectionType(Qt3DRender::QCameraLens::OrthographicProjection);
        zoomOrth(m_radius);
    }
    else{
        m_camera->setProjectionType(Qt3DRender::QCameraLens::PerspectiveProjection);
    }
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

void CameraWrapper::zoomOrth(int edge){
    float edgeF = edge * 20.0  / 100.0;
    m_camera->setTop(edgeF);
    m_camera->setBottom(-edgeF);
    m_camera->setLeft(-edgeF*m_camera->aspectRatio());
    m_camera->setRight(edgeF*m_camera->aspectRatio());
}

void CameraWrapper::translatePosRad(int radius){
    // prevent divided by zero later
    m_radius = radius;
    QVector3D dir = (m_camera->viewCenter() - m_camera->position()).normalized();
    QVector3D newPos =  -(dir * radius);
    m_camera->setPosition(newPos);
}

void CameraWrapper::translateView(QVector3D bias, int scale){
    m_viewCenter = bias;
    if(scale != 0 && m_camera->projectionType() == Qt3DRender::QCameraLens::PerspectiveProjection){
        m_radius = scale;
        QVector3D endPos = m_radius*(-m_camera->viewVector()).normalized() + bias;
        QPropertyAnimation *smoothMove = new QPropertyAnimation(this, "position");
        smoothMove->setDuration(500);
        smoothMove->setStartValue(m_camera->position());
        smoothMove->setEndValue(endPos);
        smoothMove->start();
    }
    QPropertyAnimation *smoothTurn = new QPropertyAnimation(this, "viewCenter");
    smoothTurn->setDuration(500);
    smoothTurn->setStartValue(m_camera->viewCenter());
    smoothTurn->setEndValue(bias);
    smoothTurn->start();
}

void CameraWrapper::setViewCenter(QVector3D viewCenter){
    m_camera->setViewCenter(viewCenter);
    m_viewCenter = viewCenter;
}

void CameraWrapper::setPosition(QVector3D pos){
    m_radius = (m_camera->viewVector()).length();
    m_camera->setPosition(pos);
}


void CameraWrapper::sphericalToPosition(){
    float y = m_radius * qSin(m_latitude);
    float x = m_radius * qCos(m_latitude) * qSin(m_longitude);
    float z = m_radius * qCos(m_latitude) * qCos(m_longitude);
    m_camera -> setPosition(m_viewCenter + QVector3D(x, y, z));
}

void CameraWrapper::sphericalToDirection(){
    float y = qSin(m_pitch);
    float x = qCos(m_pitch) * qSin(m_yaw);
    float z = qCos(m_pitch) * qCos(m_yaw);
    QVector3D viewDirection = QVector3D(x, y, z);
    QVector3D viewCenter = m_camera->position() + m_radius * viewDirection;
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
}
