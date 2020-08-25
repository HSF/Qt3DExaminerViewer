#include "viewer/headers/CameraWrapper.h"
#include "viewer/headers/GeneralMeshModel.h"
#include "viewer/headers/MainWindow.h"
#include "viewer/headers/ModelFactory.h"
#include "viewer/headers/ExaminerViewer.h"
#include "loader/headers/GeoLoaderQt.h"

#include <QtMath>
#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QFileDialog>
#include <QtGui/QScreen>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QRenderSettings>

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
#include <QClearBuffers>
#include <QDepthTest>
#include <QRenderStateSet>
#include <Qt3DRender/QCullFace>

// Please change it according to where you put binary files
// This path works if you put the "build" folder inside upper level of source
#define DEFAULT_FOLDER "../../../../resources/db/"

CameraWrapper *cameraWrapper;
GeneralMeshModel *loadedModel;

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
    Q_INIT_RESOURCE(resources);
    
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

    // Shows the framegraph
    view->activeFrameGraph()->dumpObjectTree();
    
    auto buf = view->defaultFrameGraph()->findChild<Qt3DRender::QClearBuffers*>();

    if(buf){
        Qt3DRender::QRenderStateSet *set = new Qt3DRender::QRenderStateSet(buf);
        Qt3DRender::QDepthTest *depthTest = new Qt3DRender::QDepthTest;
        depthTest->setDepthFunction(Qt3DRender::QDepthTest::Always);
        //set->addRenderState(depthTest);
        // Create a front face culling render state
        Qt3DRender::QCullFace *cullFront = new Qt3DRender::QCullFace();
        cullFront->setMode(Qt3DRender::QCullFace::Back);

        // Add the render state to the render pass
        //set->addRenderState(cullFront);
    }

    // view's picking settings
    // see: https://stackoverflow.com/a/58412885/32036
    auto rendersettings=view->renderSettings();
    rendersettings->pickingSettings()->setPickMethod(Qt3DRender::QPickingSettings::TrianglePicking);
    rendersettings->pickingSettings()->setPickResultMode(Qt3DRender::QPickingSettings::NearestPick);

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
    cameraWrapper = cameraWrapper;

    // load volume
    QString fileName;
    fileName = QFileDialog::getOpenFileName(mainWindow, "Open database file", DEFAULT_FOLDER, "Database Files (*.db)");

    GeoLoaderQt *loader = new GeoLoaderQt(rootEntity);
    loadedModel = loader->loadFromDB(fileName);
    for(int i = 0; i < loadedModel->subModelCount(); i++){
        loadedModel->getSubModel(i)->showMesh(true);
    }


    ModelFactory *builder = ModelFactory::GetInstance(rootEntity);
    cameraWrapper->init_distanceToOrigin = builder->MaxSize() * 1.5 / tan(qDegreesToRadians(22.5f));
    cameraWrapper->viewAll();
    cameraWrapper->resetCameraView(builder->MaxSize()*7);
    camController->setLinearSpeed(builder->MaxSize()*3);
    // Light source
    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    setUpLight(lightEntity, cameraEntity->position());

    // Create coordinate text
    GeneralMeshModel **textList = builder->build3DText();
    builder->buildCoordinateLine();
    builder->buildCoordinatePlane();
    
    QObject::connect(cameraEntity, &Qt3DRender::QCamera::positionChanged,
                     [lightEntity,cameraEntity, textList](){
        Qt3DCore::QTransform* transform = (Qt3DCore::QTransform*)lightEntity->componentsOfType<Qt3DCore::QTransform>()[0];
        transform -> setTranslation(cameraEntity->position());
        QQuaternion viewDir = cameraEntity->transform()->rotation();
        for(int i = 0; i < 6; i++){
            textList[i]->rotateMesh(viewDir);
        }
    });

    if(loadedModel != nullptr){
        ExaminerViewer *viewer = new ExaminerViewer(loadedModel, cameraWrapper);
        viewer->setupControlPanel(vLayout, mainWindow);
    }
    // Show window
    mainWindow->show();
    mainWindow->resize(1200, 800);
    return app.exec();
}
