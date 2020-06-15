#include <QApplication>

#include "headers/MeshModel.h"
#include "headers/SwithButton.h"
#include "headers/CameraWrapper.h"
#include "headers/GeneralMeshModel.h"

#include <QGuiApplication>
#include <QtGui/QScreen>

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCommandLinkButton>

#include <Qt3DExtras/qtorusmesh.h>
#include <Qt3DRender/qmesh.h>
#include <Qt3DRender/qtechnique.h>
#include <Qt3DRender/qmaterial.h>
#include <Qt3DRender/qeffect.h>
#include <Qt3DRender/qtexture.h>
#include <Qt3DRender/qrenderpass.h>
#include <Qt3DRender/qsceneloader.h>
#include <Qt3DRender/qpointlight.h>

#include <Qt3DCore/qtransform.h>
#include <Qt3DCore/qaspectengine.h>
#include <Qt3DCore/qentity.h>

#include <Qt3DRender/qrenderaspect.h>
#include <Qt3DRender/qcameralens.h>
#include <Qt3DRender/qcamera.h>
#include <Qt3DRender/QPickingSettings>

#include <Qt3DExtras/qforwardrenderer.h>
#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DExtras/qfirstpersoncameracontroller.h>
#include <Qt3DExtras/qorbitcameracontroller.h>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QCuboidMesh>

QCommandLinkButton *info;

void setUpLight(Qt3DCore::QEntity *lightEntity, QVector3D position){
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);

    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(position);
    lightEntity->addComponent(lightTransform);
}

void setUpSliderController(QLabel *label, QSlider *slider, QString tip, int initalPos){
    //Setup a label
    label->setText(tip);
    label->setGeometry(10, 120, 210, 20);
    label->show();

    //Setup a slider
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(0, 100);
    slider->setValue(initalPos);
    slider->setGeometry(10, 40, 210, 30);
    slider->setTickPosition(QSlider::TicksBelow);
    slider->setTickInterval(10);
}

void setUpInfoWindow(){
    info = new QCommandLinkButton();
    info->setText(QStringLiteral("Info windows:"));
    info->setDescription(QString::fromLatin1("Click dectector volumes to see some properties."));
    info->setIconSize(QSize(0,0));
    info->setFixedSize(QSize(180, 200));
    info->setFont(QFont ("Courier", 13));
    info->show();
}

inline void setupControlPanel(QVBoxLayout *vLayout, QWidget *mainWindow, MeshModel *detectorModel, GeneralMeshModel *cylinerModel, CameraWrapper *cameraWrapper){
    // Create a info window to display mesh properties
    setUpInfoWindow();

    // Control visibility of Volume
    QCheckBox *meshVisibleBtn = new QCheckBox(mainWindow);
    meshVisibleBtn->setChecked(true);
    meshVisibleBtn->setText(QStringLiteral("Display Detector Volume"));

    // Control radius of Camera to origin
    QLabel *labelScale = new QLabel(mainWindow);
    QSlider *sliderScale = new QSlider(mainWindow);
    setUpSliderController(labelScale, sliderScale, "radius slider", 50);

    // Contro latitude of Camera
    QLabel *labelLat = new QLabel(mainWindow);
    QSlider *sliderLat = new QSlider(mainWindow);
    setUpSliderController(labelLat, sliderLat, "latitude slider (-90~90)", 50);

    // Control longitude of Camera
    QLabel *labelLng = new QLabel(mainWindow);
    QSlider *sliderLng = new QSlider(mainWindow);
    setUpSliderController(labelLng, sliderLng, "longitude slider (0~360)", 0);

    // Cancel selected and unpacked state
    QPushButton *restoreSelectBtn = new QPushButton(mainWindow);
    restoreSelectBtn->setEnabled(true);
    restoreSelectBtn->setFixedSize(QSize(200, 30));
    restoreSelectBtn->setText(QString("revert original state"));

    // Cancel moved camera position and direction
    QPushButton *restoreViewBtn = new QPushButton(mainWindow);
    restoreViewBtn->setEnabled(true);
    restoreViewBtn->setFixedSize(QSize(200, 30));
    restoreViewBtn->setText(QString("revert original view"));

    // Switch between Ortho and Perspective
    SwitchButton* projSwitch = new SwitchButton(mainWindow, "Ortho", "Persp");
    projSwitch->setInitialState(true);

    // Switch between navigation and select mode
    SwitchButton* selectSwitch = new SwitchButton(mainWindow, "View", "Select");
    selectSwitch->setInitialState(true);

    // Control yaw angle of Camera
    QLabel *labelYaw = new QLabel(mainWindow);
    QSlider *sliderYaw = new QSlider(mainWindow);
    setUpSliderController(labelYaw, sliderYaw, "yaw slider (0~360)", 50);

    // Control pitch angle of Camera
    QLabel *labelPitch = new QLabel(mainWindow);
    QSlider *sliderPitch = new QSlider(mainWindow);
    setUpSliderController(labelPitch, sliderPitch, "pitch slider (-90~90)", 50);

    // Control roll angle of Camera
    QLabel *labelRoll = new QLabel(mainWindow);
    QSlider *sliderRoll = new QSlider(mainWindow);
    setUpSliderController(labelRoll, sliderRoll, "roll slider (0~360)", 0);

    vLayout->addWidget(info);
    vLayout->addWidget(meshVisibleBtn);
    vLayout->addWidget(labelScale);
    vLayout->addWidget(sliderScale);
    vLayout->addWidget(labelLat);
    vLayout->addWidget(sliderLat);
    vLayout->addWidget(labelLng);
    vLayout->addWidget(sliderLng);
    vLayout->addWidget(restoreSelectBtn);
    vLayout->addWidget(restoreViewBtn);
    vLayout->addWidget(projSwitch);
    vLayout->addWidget(selectSwitch);
    vLayout->addWidget(labelYaw);
    vLayout->addWidget(sliderYaw);
    vLayout->addWidget(labelPitch);
    vLayout->addWidget(sliderPitch);
    vLayout->addWidget(labelRoll);
    vLayout->addWidget(sliderRoll);

    // Connect UI with model
    QObject::connect(meshVisibleBtn, &QCheckBox::stateChanged, detectorModel, &MeshModel::showMesh);
    QObject::connect(sliderScale,SIGNAL(valueChanged(int)), cameraWrapper, SLOT(translatePosRad(int)));
    QObject::connect(sliderLat, SIGNAL(valueChanged(int)), cameraWrapper, SLOT(translatePosLat(int)));
    QObject::connect(sliderLng, SIGNAL(valueChanged(int)), cameraWrapper, SLOT(translatePosLng(int)));
    QObject::connect(restoreSelectBtn, SIGNAL(clicked(bool)), cylinerModel, SLOT(restoreState(bool)));
    QObject::connect(restoreViewBtn, SIGNAL(clicked(bool)), cameraWrapper, SLOT(resetCameraView(bool)));
    QObject::connect(projSwitch, SIGNAL(valueChanged(bool)),  cameraWrapper, SLOT(setProjectiveMode(bool)));
    QObject::connect(selectSwitch, SIGNAL(valueChanged(bool)),  cameraWrapper, SLOT(disableCameraController(bool)));
    QObject::connect(selectSwitch, SIGNAL(valueChanged(bool)),  cylinerModel, SLOT(enablePickAll(bool)));


    QObject::connect(sliderLat, &QSlider::valueChanged, sliderPitch, [sliderPitch, sliderLat](){sliderPitch->setValue(100-sliderLat->value());});
    QObject::connect(sliderLng, &QSlider::valueChanged, sliderYaw, [sliderYaw, sliderLng](){sliderYaw->setValue((50 + sliderLng->value())%100 );});

    QObject::connect(sliderYaw, SIGNAL(valueChanged(int)), cameraWrapper, SLOT(rotateViewYaw(int)));
    QObject::connect(sliderPitch, SIGNAL(valueChanged(int)), cameraWrapper, SLOT(rotateViewPitch(int)));
    QObject::connect(sliderRoll, SIGNAL(valueChanged(int)), cameraWrapper, SLOT(rotateViewRoll(int)));

}

int main(int argc, char **argv){

    QApplication app(argc, argv);

    // Root entity
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

    // view and container
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    view->setRootEntity(rootEntity);
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f))); 
    QWidget *container = QWidget::createWindowContainer(view);
    QSize screenSize = view->screen()->size();
    container->setMinimumSize(QSize(200, 100));
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
    camController->setCamera(nullptr);

    // Light source
    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    setUpLight(lightEntity, cameraEntity->position());

    // geometry model
    Qt3DExtras::QCylinderMesh *meshCyliner = new Qt3DExtras::QCylinderMesh();
    meshCyliner->setObjectName("World Volume");
    GeneralMeshModel *cylinerModel = new GeneralMeshModel(rootEntity, meshCyliner);
    cylinerModel->translateMesh(QVector3D(-5.0f, 0.0f, 0.0f));
    cylinerModel->scaleMesh(4);

    Qt3DExtras::QCuboidMesh *meshBox1 = new Qt3DExtras::QCuboidMesh();
    meshBox1->setObjectName("Muon");
    GeneralMeshModel *cuboidModel1 = new GeneralMeshModel(rootEntity, meshBox1);
    cuboidModel1->translateMesh(QVector3D(-5.0f, 1.0f, 0.0f));
    cuboidModel1->scaleMesh(2);
    cuboidModel1->showMesh(false);

    Qt3DExtras::QCuboidMesh *meshBox2 = new Qt3DExtras::QCuboidMesh();
    meshBox2->setObjectName("Calorimeter");
    GeneralMeshModel *cuboidModel2 = new GeneralMeshModel(rootEntity, meshBox2);
    cuboidModel2->translateMesh(QVector3D(-5.0f, -1.0f, 0.0f));
    cuboidModel2->scaleMesh(2);
    cuboidModel2->showMesh(false);

    Qt3DExtras::QCuboidMesh *meshBox3 = new Qt3DExtras::QCuboidMesh();
    meshBox3->setObjectName("one daughter of Muon");
    GeneralMeshModel *cuboidModel3 = new GeneralMeshModel(rootEntity, meshBox3);
    cuboidModel3->translateMesh(QVector3D(-5.0f, 1.0f, 0.0f));
    cuboidModel3->showMesh(false);

    cylinerModel->add_subModel(cuboidModel1);
    cylinerModel->add_subModel(cuboidModel2);
    cuboidModel1->add_subModel(cuboidModel3);

    // Create detector mesh model
    // Mesh shape and properties
    Qt3DRender::QMesh *mesh = new Qt3DRender::QMesh();
    mesh->setSource(QUrl("qrc:/mesh/TrackML-PixelDetector.obj"));
    mesh->setProperty("Vertices", QVariant(37216));
    mesh->setProperty("Edges", QVariant(58416));
    mesh->setProperty("Faces", QVariant(29208));
    MeshModel *detectorModel = new MeshModel(rootEntity, mesh);

    /*   Qt3DRender::QMesh *meshLeft = new Qt3DRender::QMesh();
       meshLeft->setSource(QUrl("qrc:/mesh/left_part.obj"));
       meshLeft->setProperty("Vertices", QVariant(3));
       meshLeft->setProperty("Edges", QVariant(5));
       meshLeft->setProperty("Faces", QVariant(29));
       MeshModel *subModelLeft = new MeshModel(rootEntity, meshLeft);


       Qt3DRender::QMesh *meshRight = new Qt3DRender::QMesh();
       meshRight->setSource(QUrl("qrc:/mesh/right_part.obj"));
       meshRight->setProperty("Vertices", QVariant(32));
       meshRight->setProperty("Edges", QVariant(58));
       meshRight->setProperty("Faces", QVariant(2));
       MeshModel *subModelRight = new MeshModel(rootEntity, meshRight);

       Qt3DRender::QMesh *meshMiddle = new Qt3DRender::QMesh();
       meshMiddle->setSource(QUrl("qrc:/mesh/middle_part.obj"));
       meshMiddle->setProperty("Vertices", QVariant(16));
       meshMiddle->setProperty("Edges", QVariant(56));
       meshMiddle->setProperty("Faces", QVariant(8));
       MeshModel *subModelMiddle = new MeshModel(rootEntity, meshMiddle);

       detectorModel->add_subModel(subModelLeft);
       detectorModel->add_subModel(subModelRight);
       //detectorModel->add_subModel(subModelMiddle);

   */
    setupControlPanel(vLayout, mainWindow, detectorModel, cylinerModel, cameraWrapper);

    // Show window
    mainWindow->show();
    mainWindow->resize(1200, 800);

    return app.exec();
}
