#include "viewer/headers/CameraWrapper.h"
#include "viewer/headers/GeneralMeshModel.h"
#include "viewer/headers/MainWindow.h"
#include "viewer/headers/ModelFactory.h"
#include "viewer/headers/ExaminerViewer.h"

#include <QtMath>
#include <QApplication>
#include <QGuiApplication>

#include <QtGui/QScreen>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

#include <Qt3DCore/qtransform.h>
#include <Qt3DCore/qaspectengine.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/qrenderaspect.h>
#include <Qt3DRender/qcameralens.h>
#include <Qt3DRender/qcamera.h>
#include <Qt3DRender/QPickingSettings>
#include <Qt3DRender/QPointLight>
#include <Qt3DExtras/qforwardrenderer.h>
#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DExtras/qfirstpersoncameracontroller.h>
#include <Qt3DExtras/qorbitcameracontroller.h>

CameraWrapper *camera;

void setUpLight(Qt3DCore::QEntity *lightEntity, QVector3D position){
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);

    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(position);
    lightEntity->addComponent(lightTransform);
}

int main(int argc, char **argv){

    QApplication app(argc, argv);

    // Root entity
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

    // view and container
    MainWindow *view = new MainWindow();
    view->setRootEntity(rootEntity);
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f))); 
    QWidget *container = QWidget::createWindowContainer(view);
    QSize screenSize = view->screen()->size();
    container->setMinimumSize(QSize(100, 100));
    container->setMaximumSize(screenSize);

    // Layout
    QWidget *mainWindow = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(mainWindow);
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setAlignment(Qt::AlignTop);
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);
    mainWindow->setWindowTitle(QStringLiteral("Geo3D Examiner Viewer"));

    // Camera and Camera controls
    Qt3DRender::QCamera *cameraEntity = view->camera();
    CameraWrapper *cameraWrapper = new CameraWrapper(rootEntity, cameraEntity);
    Qt3DExtras::QOrbitCameraController *camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    cameraWrapper->addCameraController(camController);
    //camController->setCamera(nullptr);
    camera = cameraWrapper;

    // Light source
    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    setUpLight(lightEntity, cameraEntity->position());

    // volume picking setting
    Qt3DRender::QPickingSettings *settings = new Qt3DRender::QPickingSettings();
    settings->setPickMethod(Qt3DRender::QPickingSettings::PickMethod::TrianglePicking);

    // Create mesh model
    ModelFactory *builder = new ModelFactory(rootEntity);
    builder->build3DText();
    GeneralMeshModel *cylinderModel = builder->buildVolume();
    cylinderModel->enablePickAll(false);

    QObject::connect(cameraEntity, &Qt3DRender::QCamera::positionChanged, [lightEntity,cameraEntity](){
        Qt3DCore::QTransform* transform = (Qt3DCore::QTransform*)lightEntity->componentsOfType<Qt3DCore::QTransform>()[0];
        transform -> setTranslation(cameraEntity->position());
        //QQuaternion viewDir = cameraEntity->transform()->rotation();
        //for(int i = 0; i < 6; i++){
        //    textList[i]->rotateMesh(viewDir);
        //}
    });

    QObject::connect(cameraEntity, &Qt3DRender::QCamera::viewCenterChanged, [cameraWrapper, cameraEntity](){
        cameraWrapper->setViewCenter(cameraEntity->viewCenter());
    });

    ExaminerViewer *viewer = new ExaminerViewer(cylinderModel, cameraWrapper);
    viewer->setupControlPanel(vLayout, mainWindow);

    // Show window
    mainWindow->show();
    mainWindow->resize(1200, 800);
    return app.exec();
}
