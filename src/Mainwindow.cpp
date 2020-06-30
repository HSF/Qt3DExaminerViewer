#include "headers/Mainwindow.h"
#include <QDebug>
#include <QKeyEvent>
#include <Qt3DInput/QAnalogAxisInput>
#include <Qt3DInput/QAxis>
#include <Qt3DInput/QMouseDevice>

MainWindow::MainWindow(): Qt3DExtras::Qt3DWindow(){
    Qt3DCore::QEntity *deviceEntity = new Qt3DCore::QEntity();
    Qt3DInput::QAnalogAxisInput *mouseTxInput = new Qt3DInput::QAnalogAxisInput();
    Qt3DInput::QMouseDevice *mouse = new Qt3DInput::QMouseDevice();
    mouseTxInput->setSourceDevice(mouse);
    mouseTxInput->setAxis(Qt3DInput::QMouseDevice::X);
    m_axisX = new Qt3DInput::QAxis();
    m_axisX->addInput(mouseTxInput);
    qInfo() << m_axisX->value();

    Qt3DInput::QAnalogAxisInput *mouseTyInput = new Qt3DInput::QAnalogAxisInput();
    mouseTyInput->setSourceDevice(mouse);
    mouseTyInput->setAxis(Qt3DInput::QMouseDevice::Y);
    m_axisY = new Qt3DInput::QAxis();
    m_axisY->addInput(mouseTyInput);
    qInfo() << m_axisY->value();

    m_logicalDevice = new Qt3DInput::QLogicalDevice();
    m_logicalDevice->addAxis(m_axisX);
    m_logicalDevice->addAxis(m_axisY);
    m_logicalDevice->setEnabled(true);
    qInfo() << m_logicalDevice->isEnabled();
    deviceEntity->addComponent(m_logicalDevice);
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
    //qInfo()<<"mouse is pressed";
    QObject::connect(m_axisX, &Qt3DInput::QAxis::valueChanged, this, [this](){qInfo() << "x: " << m_axisX->value();});
    QObject::connect(m_axisY, &Qt3DInput::QAxis::valueChanged, this, [this](){qInfo() << "y: " << m_axisY->value();});
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_X:
        qInfo()<<"Key X is pressed";
        break;
    default:
        break;
    }
}
