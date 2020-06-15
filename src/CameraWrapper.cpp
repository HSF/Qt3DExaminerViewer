#include "headers/CameraWrapper.h"
#include <Qt3DCore/qtransform.h>
#include <QtMath>

CameraWrapper::CameraWrapper(Qt3DCore::QEntity *rootEntity,  Qt3DRender::QCamera *camera) : m_rootEntity(rootEntity)
{
    m_camera = camera;
    resetCameraView(true);
    m_distanceToOrigin = m_init_distanceToOrigin;
    m_latitude = 0;
    m_longitude = 0;
    m_pitch = 0;
    m_roll = 0;
    m_yaw = M_PI;
}

void CameraWrapper::resetCameraView(bool isReset){
    m_camera->setProjectionType(Qt3DRender::QCameraLens::PerspectiveProjection);
    m_camera->setPosition(QVector3D(0, 0, m_init_distanceToOrigin));
    m_camera->setUpVector(QVector3D(0, 1, 0));
    m_camera->setViewCenter(QVector3D(0, 0, 0));
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

void CameraWrapper::translatePosRad(int radius){
    // prevent divided by zero later
    radius += 1;
    QVector3D position = m_camera -> position();
    m_camera->setPosition(position * radius / m_distanceToOrigin);
    m_distanceToOrigin = (float)(radius);
}

void CameraWrapper::translatePosLat(int latitude){
    m_latitude = (latitude - 50) * M_PI / 2 / 50.0;
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

void CameraWrapper::translatePosLng(int longitude){
    m_longitude = longitude * 2 * M_PI / 100.0;
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


void CameraWrapper::rotateViewRoll(int roll){
    m_roll = roll * 2 * M_PI / 100.0;

    float y = qSin(m_pitch);
    float x = qCos(m_pitch) * qSin(m_yaw);
    float z = qCos(m_pitch) * qCos(m_yaw);
    QVector3D viewDirection = QVector3D(x, y, z);
    QVector3D viewCenter = m_camera->position() + m_init_distanceToOrigin * viewDirection;
    qInfo() << viewDirection << endl;
        qInfo() << viewCenter << endl;
            qInfo() << m_camera->position() << endl;
                        qInfo() << m_pitch << endl;
                                    qInfo() << m_yaw << endl;
    m_camera->setViewCenter(viewCenter);

    x = qCos(m_roll);
    y = qSin(m_roll);
    QVector3D extraAxisY = QVector3D::crossProduct(viewDirection, QVector3D(0,1,0));
    QVector3D extraAxisX = QVector3D::crossProduct(extraAxisY, viewDirection);
    QVector3D newUpVector = x * extraAxisX + y * extraAxisY;
    m_camera -> setUpVector(newUpVector);
}

void CameraWrapper::rotateViewYaw(int yaw){
    m_yaw = yaw * 2 * M_PI / 100.0;

    float y = qSin(m_pitch);
    float x = qCos(m_pitch) * qSin(m_yaw);
    float z = qCos(m_pitch) * qCos(m_yaw);
    QVector3D viewDirection = QVector3D(x, y, z);
    QVector3D viewCenter = m_camera->position() + m_init_distanceToOrigin * viewDirection;
    m_camera->setViewCenter(viewCenter);

    x = qCos(m_roll);
    y = qSin(m_roll);
    QVector3D extraAxisY = QVector3D::crossProduct(viewDirection, QVector3D(0,1,0));
    QVector3D extraAxisX = QVector3D::crossProduct(extraAxisY, viewDirection);
    QVector3D newUpVector = x * extraAxisX + y * extraAxisY;
    m_camera -> setUpVector(newUpVector);

    qInfo() << viewDirection << endl;
        qInfo() << viewCenter << endl;
            qInfo() << m_camera->position() << endl;
                        qInfo() << m_pitch << endl;
                                    qInfo() << m_yaw << endl;
}

void CameraWrapper::rotateViewPitch(int pitch){
    m_pitch = (pitch - 50) * M_PI / 2 / 50.0;

    float y = qSin(m_pitch);
    float x = qCos(m_pitch) * qSin(m_yaw);
    float z = qCos(m_pitch) * qCos(m_yaw);
    QVector3D viewDirection = QVector3D(x, y, z);
    QVector3D viewCenter = m_camera->position() + m_init_distanceToOrigin * viewDirection;
    m_camera->setViewCenter(viewCenter);

    x = qCos(m_roll);
    y = qSin(m_roll);
    QVector3D extraAxisY;
    QVector3D extraAxisX;
    if(qAbs(m_pitch) == M_PI/2){
        extraAxisX = qCos(m_yaw) * QVector3D(0, 0, 1) + qSin(m_yaw) * QVector3D(1, 0, 0);
        extraAxisY = qSin(m_yaw) * QVector3D(0, 0, 1) - qCos(m_yaw) * QVector3D(1, 0, 0);
    }
    else {
        extraAxisY = QVector3D::crossProduct(viewDirection, QVector3D(0,1,0));
        extraAxisX = QVector3D::crossProduct(extraAxisY, viewDirection);
    }
    QVector3D newUpVector = x * extraAxisX + y * extraAxisY;
    m_camera -> setUpVector(newUpVector);


    qInfo() << viewDirection << endl;
        qInfo() << viewCenter << endl;
            qInfo() << m_camera->position() << endl;
                        qInfo() << m_pitch << endl;
                                    qInfo() << m_yaw << endl;
}
