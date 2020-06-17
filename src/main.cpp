#include "headers/MeshModel.h"
#include "headers/SwithButton.h"
#include "headers/CameraWrapper.h"
#include "headers/GeneralMeshModel.h"

#include <QApplication>
#include <QGuiApplication>
#include <QtGui/QScreen>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QGroupBox>
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
    label->setGeometry(10, 120, 210, 15);
    label->setMaximumHeight(15);

    //Setup a slider
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(0, 359);
    slider->setSingleStep(1);
    slider->setValue(initalPos);
    slider->setGeometry(10, 40, 210, 20);
    slider->setMaximumHeight(20);
    slider->setTickPosition(QSlider::TicksBelow);
    slider->setTickInterval(10);
}

void setUpInfoWindow(){
    info = new QCommandLinkButton();
    info->setText(QStringLiteral("Info windows:"));
    info->setDescription(QString::fromLatin1("Click dectector volumes to see some properties."));
    info->setIconSize(QSize(0,0));
    info->setFixedSize(QSize(180, 140));
    info->setFont(QFont ("Courier", 13));
}

inline void setupControlPanel(QVBoxLayout *vLayout, QWidget *mainWindow, MeshModel *detectorModel, GeneralMeshModel *cylinerModel, CameraWrapper *cameraWrapper){
    // Create a info window to display mesh properties
    setUpInfoWindow();

    // Control visibility of Volume
    QCheckBox *meshVisibleBtn = new QCheckBox(mainWindow);
    meshVisibleBtn->setChecked(true);
    meshVisibleBtn->setText(QStringLiteral("Display Detector Volume"));

    // Control radius of Camera to origin
    QHBoxLayout *hLayoutRad = new QHBoxLayout(mainWindow);
    QLabel *labelScale = new QLabel(mainWindow);
    QSlider *sliderScale = new QSlider(mainWindow);
    setUpSliderController(labelScale, sliderScale, "radius (1~100m)", int(cameraWrapper->init_distanceToOrigin));
    sliderScale->setRange(1, 100);
    QSpinBox *spinScale = new QSpinBox(mainWindow);
    spinScale->setRange(1, 100);
    spinScale->setValue(int(cameraWrapper->init_distanceToOrigin));
    spinScale->setMaximumWidth(50);
    hLayoutRad->addWidget(labelScale);
    hLayoutRad->addWidget(spinScale);
    QLabel *labelScaleTicks = new QLabel(mainWindow);
    labelScaleTicks->setText("  1  \t\t  50  \t\t  100");
    QFont font = labelScaleTicks->font();
    font.setPointSize(10);
    labelScaleTicks->setFont(font);
    labelScaleTicks->setMaximumHeight(10);

    // Control longitude of Camera
    QLabel *labelLng = new QLabel(mainWindow);
    QSlider *sliderLng = new QSlider(mainWindow);
    setUpSliderController(labelLng, sliderLng, "longitude (0~359°)", 0);
    QHBoxLayout *hLayoutLng = new QHBoxLayout(mainWindow);
    QSpinBox *spinLng = new QSpinBox(mainWindow);
    spinLng->setRange(0, 359);
    spinLng->setMaximumWidth(50);
    hLayoutLng->addWidget(labelLng);
    hLayoutLng->addWidget(spinLng);
    QLabel *labelLngTicks = new QLabel(mainWindow);
    labelLngTicks->setText("  0  \t\t  180  \t\t  359");
    labelLngTicks->setFont(font);
    labelLngTicks->setMaximumHeight(10);

    // Contro latitude of Camera
    QHBoxLayout *hLayoutLat = new QHBoxLayout(mainWindow);
    QLabel *labelLat = new QLabel(mainWindow);
    QSlider *sliderLat = new QSlider(mainWindow);
    setUpSliderController(labelLat, sliderLat, "latitude (-90~90°)", 0);
    sliderLat->setRange(-90, 90);
    QSpinBox *spinLat = new QSpinBox(mainWindow);
    spinLat->setRange(-90, 90);
    spinLat->setSingleStep(1);
    spinLat->setMaximumWidth(50);
    hLayoutLat->addWidget(labelLat);
    hLayoutLat->addWidget(spinLat);
    QLabel *labelLatTicks = new QLabel(mainWindow);
    labelLatTicks->setText(" -90  \t\t   0  \t\t    90");
    labelLatTicks->setFont(font);
    labelLatTicks->setMaximumHeight(10);

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
    setUpSliderController(labelYaw, sliderYaw, "yaw (0~359°)", 180);
    QHBoxLayout *hLayoutYaw = new QHBoxLayout(mainWindow);
    QSpinBox *spinYaw = new QSpinBox(mainWindow);
    spinYaw->setRange(0, 359);
    spinYaw->setValue(180);
    spinYaw->setMaximumWidth(50);
    hLayoutYaw->addWidget(labelYaw);
    hLayoutYaw->addWidget(spinYaw);
    QLabel *labelYawTicks = new QLabel(mainWindow);
    labelYawTicks->setText("  0  \t\t  180  \t\t  359");
    labelYawTicks->setFont(font);
    labelYawTicks->setMaximumHeight(8);

    // Control pitch angle of Camera
    QLabel *labelPitch = new QLabel(mainWindow);
    QSlider *sliderPitch = new QSlider(mainWindow);
    setUpSliderController(labelPitch, sliderPitch, "pitch (-90~90°)", 0);
    sliderPitch->setRange(-90, 90);
    QHBoxLayout *hLayoutPitch = new QHBoxLayout(mainWindow);
    QSpinBox *spinPitch = new QSpinBox(mainWindow);
    spinPitch->setRange(-90, 90);
    spinPitch->setMaximumWidth(50);
    hLayoutPitch->addWidget(labelPitch);
    hLayoutPitch->addWidget(spinPitch);
    QLabel *labelPitchTicks = new QLabel(mainWindow);
    labelPitchTicks->setText(" -90  \t\t   0  \t\t    90");
    labelPitchTicks->setFont(font);
    labelPitchTicks->setMaximumHeight(8);


    // Control roll angle of Camera
    QLabel *labelRoll = new QLabel(mainWindow);
    QSlider *sliderRoll = new QSlider(mainWindow);
    setUpSliderController(labelRoll, sliderRoll, "roll (0~359°)", 0);
    QHBoxLayout *hLayoutRoll = new QHBoxLayout(mainWindow);
    QSpinBox *spinRoll = new QSpinBox(mainWindow);
    spinRoll->setRange(0, 359);
    spinRoll->setMaximumWidth(50);
    hLayoutRoll->addWidget(labelRoll);
    hLayoutRoll->addWidget(spinRoll);
    QLabel *labelRollTicks = new QLabel(mainWindow);
    labelRollTicks->setText("  0  \t\t  180  \t\t  359");
    labelRollTicks->setFont(font);
    labelRollTicks->setMaximumHeight(8);

    QVBoxLayout *positionControlLayout = new QVBoxLayout(mainWindow);

    vLayout->addWidget(info);
    vLayout->addWidget(meshVisibleBtn);

    positionControlLayout->addLayout(hLayoutRad);
    //vLayout->addWidget(labelScale);
    positionControlLayout->addWidget(sliderScale);
    positionControlLayout->addWidget(labelScaleTicks);

    positionControlLayout->addLayout(hLayoutLng);
    //vLayout->addWidget(labelLng);
    positionControlLayout->addWidget(sliderLng);
    positionControlLayout->addWidget(labelLngTicks);

    positionControlLayout->addLayout(hLayoutLat);
    //vLayout->addWidget(labelLat);
    positionControlLayout->addWidget(sliderLat);
    positionControlLayout->addWidget(labelLatTicks);

    QGroupBox *positionControl = new QGroupBox("Camera Position");
    positionControl->setLayout(positionControlLayout);
    positionControl->setFixedSize(230, 260);
    vLayout->addWidget(positionControl);

    vLayout->addWidget(restoreSelectBtn);
    vLayout->addWidget(restoreViewBtn);

    QHBoxLayout *hLayoutSwitch = new QHBoxLayout(mainWindow);
    hLayoutSwitch->addWidget(projSwitch);
    hLayoutSwitch->addWidget(selectSwitch);
    vLayout->addLayout(hLayoutSwitch);

    QVBoxLayout *directionControlLayout = new QVBoxLayout(mainWindow);

    directionControlLayout->addLayout(hLayoutYaw);
    //vLayout->addWidget(labelYaw);
    directionControlLayout->addWidget(sliderYaw);
    directionControlLayout->addWidget(labelYawTicks);

    directionControlLayout->addLayout(hLayoutPitch);
   // vLayout->addWidget(labelPitch);
    directionControlLayout->addWidget(sliderPitch);
    directionControlLayout->addWidget(labelPitchTicks);

    directionControlLayout->addLayout(hLayoutRoll);
    //vLayout->addWidget(labelRoll);
    directionControlLayout->addWidget(sliderRoll);
    directionControlLayout->addWidget(labelRollTicks);
    QGroupBox *directionControl = new QGroupBox("Camera Direction");
    directionControl->setLayout(directionControlLayout);
    directionControl->setFixedSize(230, 260);
    vLayout->addWidget(directionControl);

    // Connect UI with model
    QObject::connect(meshVisibleBtn, &QCheckBox::stateChanged, detectorModel, &MeshModel::showMesh);  
    QObject::connect(restoreSelectBtn, SIGNAL(clicked(bool)), cylinerModel, SLOT(restoreState(bool)));
    QObject::connect(restoreViewBtn, &QPushButton::clicked, cameraWrapper, &CameraWrapper::resetCameraView);
    QObject::connect(restoreViewBtn, &QPushButton::clicked, cameraWrapper, [sliderRoll, sliderLat, sliderLng, sliderScale](){
        sliderRoll->setValue(0);
        sliderLat->setValue(0);
        sliderLng->setValue(0);
        sliderScale->setValue(50);
    });
    QObject::connect(projSwitch, SIGNAL(valueChanged(bool)),  cameraWrapper, SLOT(setProjectiveMode(bool)));
    QObject::connect(selectSwitch, SIGNAL(valueChanged(bool)),  cameraWrapper, SLOT(disableCameraController(bool)));
    QObject::connect(selectSwitch, SIGNAL(valueChanged(bool)),  cylinerModel, SLOT(enablePickAll(bool)));

    QObject::connect(sliderScale,SIGNAL(valueChanged(int)), cameraWrapper, SLOT(translatePosRad(int)));
    QObject::connect(sliderScale, SIGNAL(valueChanged(int)), spinScale, SLOT(setValue(int)));
    QObject::connect(spinScale, SIGNAL(valueChanged(int)), sliderScale, SLOT(setValue(int)));

    QObject::connect(sliderLat, SIGNAL(valueChanged(int)), cameraWrapper, SLOT(translatePosLat(int)));
    QObject::connect(sliderLat, SIGNAL(valueChanged(int)), spinLat, SLOT(setValue(int)));
    QObject::connect(spinLat, SIGNAL(valueChanged(int)), sliderLat, SLOT(setValue(int)));
    QObject::connect(sliderLat, &QSlider::valueChanged, sliderPitch, [sliderPitch, sliderLat](){sliderPitch->setValue(-sliderLat->value());});

    QObject::connect(sliderLng, SIGNAL(valueChanged(int)), cameraWrapper, SLOT(translatePosLng(int)));
    QObject::connect(sliderLng, SIGNAL(valueChanged(int)), spinLng, SLOT(setValue(int)));
    QObject::connect(spinLng, SIGNAL(valueChanged(int)), sliderLng, SLOT(setValue(int)));
    QObject::connect(sliderLng, &QSlider::valueChanged, sliderYaw, [sliderYaw, sliderLng](){sliderYaw->setValue((180 + sliderLng->value())%360 );});

    QObject::connect(sliderYaw, SIGNAL(valueChanged(int)), cameraWrapper, SLOT(rotateViewYaw(int)));
    QObject::connect(sliderYaw, SIGNAL(valueChanged(int)), spinYaw, SLOT(setValue(int)));
    QObject::connect(spinYaw, SIGNAL(valueChanged(int)), sliderYaw, SLOT(setValue(int)));

    QObject::connect(sliderPitch, SIGNAL(valueChanged(int)), cameraWrapper, SLOT(rotateViewPitch(int)));
    QObject::connect(sliderPitch, SIGNAL(valueChanged(int)), spinPitch, SLOT(setValue(int)));
    QObject::connect(spinPitch, SIGNAL(valueChanged(int)), sliderPitch, SLOT(setValue(int)));

    QObject::connect(sliderRoll, SIGNAL(valueChanged(int)), cameraWrapper, SLOT(rotateViewRoll(int)));
    QObject::connect(sliderRoll, SIGNAL(valueChanged(int)), spinRoll, SLOT(setValue(int)));
    QObject::connect(spinRoll, SIGNAL(valueChanged(int)), sliderRoll, SLOT(setValue(int)));
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

    // volume picking setting
    Qt3DRender::QPickingSettings *settings = new Qt3DRender::QPickingSettings();
    settings->setPickMethod(Qt3DRender::QPickingSettings::PickMethod::TrianglePicking);

    // geometry model
    Qt3DExtras::QCylinderMesh *meshCyliner = new Qt3DExtras::QCylinderMesh();
    meshCyliner->setObjectName(QString("World Volume"));
    GeneralMeshModel *cylinerModel = new GeneralMeshModel(rootEntity, meshCyliner);
    cylinerModel->translateMesh(QVector3D(-5.0f, 0.0f, 0.0f));
    cylinerModel->scaleMesh(4);

    Qt3DExtras::QCuboidMesh *meshBox1 = new Qt3DExtras::QCuboidMesh();
    meshBox1->setObjectName(QString("Muon"));
    GeneralMeshModel *cuboidModel1 = new GeneralMeshModel(rootEntity, meshBox1);
    cuboidModel1->translateMesh(QVector3D(-5.0f, 1.0f, 0.0f));
    cuboidModel1->scaleMesh(2);
    cuboidModel1->showMesh(false);

    Qt3DExtras::QCuboidMesh *meshBox2 = new Qt3DExtras::QCuboidMesh();
    meshBox2->setObjectName(QString("Calorimeter"));
    GeneralMeshModel *cuboidModel2 = new GeneralMeshModel(rootEntity, meshBox2);
    cuboidModel2->translateMesh(QVector3D(-5.0f, -1.0f, 0.0f));
    cuboidModel2->scaleMesh(2);
    cuboidModel2->showMesh(false);

    Qt3DExtras::QCuboidMesh *meshBox3 = new Qt3DExtras::QCuboidMesh();
    meshBox3->setObjectName(QString("one daughter of Muon"));
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
