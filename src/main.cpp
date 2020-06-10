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

void setUpCamera(Qt3DRender::QCamera *cameraEntity){
    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setProjectionType(Qt3DRender::QCameraLens::PerspectiveProjection);
    cameraEntity->setPosition(QVector3D(0, 0, 5));
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));
}

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
}

void setUpInfoWindow(){
    info = new QCommandLinkButton();
    info->setText(QStringLiteral("Info windows:"));
    info->setDescription(QString::fromLatin1("Click dectector volumes to see some properties."));
    info->setIconSize(QSize(0,0));
    info->setFixedSize(QSize(200, 100));
    info->setFont(QFont ("Courier", 14));
    info->show();
}

void setupControlPanel(QVBoxLayout *vLayout, QWidget *widget, MeshModel *detectorModel, GeneralMeshModel *cylinerModel, CameraWrapper *cameraWrapper){
    // Create a info window to display mesh properties
    setUpInfoWindow();

    // Control visibility of Volume
    QCheckBox *meshVisibleBtn = new QCheckBox(widget);
    meshVisibleBtn->setChecked(true);
    meshVisibleBtn->setText(QStringLiteral("Display Detector Volume"));

    QCheckBox *unpackBtn = new QCheckBox(widget);
    unpackBtn->setChecked(false);
    unpackBtn->setText(QStringLiteral("unpack children Volume"));

    // Control scale of Volume
    QLabel *labelScale = new QLabel(widget);
    QSlider *sliderScale = new QSlider(widget);
    setUpSliderController(labelScale, sliderScale, "Scale slider", 50);

    // Control rotation X of Volume
    QLabel *labelX = new QLabel(widget);
    QSlider *sliderX = new QSlider(widget);
    setUpSliderController(labelX, sliderX, "rotation slider X (0-360)", 0);

    // Control rotation Y of Volume
    QLabel *labelY = new QLabel(widget);
    QSlider *sliderY = new QSlider(widget);
    setUpSliderController(labelY, sliderY, "rotation slider Y (0-360)", 0);

    // Control rotation Z of Volume
    QLabel *labelZ = new QLabel(widget);
    QSlider *sliderZ = new QSlider(widget);
    setUpSliderController(labelZ, sliderZ, "rotation slider Z (0-360)", 0);

    QPushButton *restoreBtn = new QPushButton(widget);
    restoreBtn->setEnabled(true);
    restoreBtn->setFixedSize(QSize(130, 30));
    restoreBtn->setText(QString("restore"));

    vLayout->addWidget(info);
    vLayout->addWidget(meshVisibleBtn);
    vLayout->addWidget(unpackBtn);
    vLayout->addWidget(labelScale);
    vLayout->addWidget(sliderScale);
    vLayout->addWidget(labelX);
    vLayout->addWidget(sliderX);
    vLayout->addWidget(labelY);
    vLayout->addWidget(sliderY);
    vLayout->addWidget(labelZ);
    vLayout->addWidget(sliderZ);
    vLayout->addWidget(restoreBtn);


    // Connect UI with model
    QObject::connect(meshVisibleBtn, &QCheckBox::stateChanged, detectorModel, &MeshModel::showMesh);
   // QObject::connect(unpackBtn, &QCheckBox::stateChanged, cylinerModel, &GeneralMeshModel::unpackSubMesh);
    QObject::connect(sliderScale,SIGNAL(valueChanged(int)), cameraWrapper, SLOT(scaleView(int)));
    QObject::connect(sliderX, SIGNAL(valueChanged(int)), cameraWrapper, SLOT(rotateViewX(int)));
    QObject::connect(sliderY, SIGNAL(valueChanged(int)), cameraWrapper, SLOT(rotateViewY(int)));
    QObject::connect(sliderZ, SIGNAL(valueChanged(int)), cameraWrapper, SLOT(rotateViewZ(int)));
    QObject::connect(restoreBtn, SIGNAL(clicked(bool)), detectorModel, SLOT(restoreState(bool)));
    QObject::connect(restoreBtn, SIGNAL(clicked(bool)), cylinerModel, SLOT(restoreState(bool)));
}

int main(int argc, char **argv){

    QApplication app(argc, argv);

    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f))); 
    QWidget *container = QWidget::createWindowContainer(view);
    QSize screenSize = view->screen()->size();
    container->setMinimumSize(QSize(200, 100));
    container->setMaximumSize(screenSize);

    // Layout
    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setAlignment(Qt::AlignTop);
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);
    widget->setWindowTitle(QStringLiteral("Geo3D Examiner Viewer"));

    // Root entity
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

    // Camera and Camera controls
    Qt3DRender::QCamera *cameraEntity = view->camera();
    Qt3DExtras::QOrbitCameraController *camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    setUpCamera(cameraEntity);
    CameraWrapper *cameraWrapper = new CameraWrapper(rootEntity, cameraEntity);
    cameraWrapper->addCameraController(camController);
    camController->setCamera(cameraEntity);

    // Light
    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    setUpLight(lightEntity, cameraEntity->position());


    // Set root object of the scene
    view->setRootEntity(rootEntity);


    Qt3DExtras::QCylinderMesh *meshCyliner = new Qt3DExtras::QCylinderMesh();
    meshCyliner->setProperty("Vertices", QVariant(32));
    meshCyliner->setProperty("Edges", QVariant(58));
    meshCyliner->setProperty("Faces", QVariant(2));
    GeneralMeshModel *cylinerModel = new GeneralMeshModel(rootEntity, meshCyliner);
    cylinerModel->translateMesh(QVector3D(-3.0f, 0.0f, 0.0f));
    cylinerModel->scaleMesh(3);

    Qt3DExtras::QCuboidMesh *meshBox1 = new Qt3DExtras::QCuboidMesh();
    meshBox1->setProperty("Vertices", QVariant(32));
    meshBox1->setProperty("Edges", QVariant(58));
    meshBox1->setProperty("Faces", QVariant(2));
    GeneralMeshModel *cuboidModel1 = new GeneralMeshModel(rootEntity, meshBox1);
    cuboidModel1->translateMesh(QVector3D(-3.0f, 1.0f, 0.0f));
    cuboidModel1->showMesh(false);

    Qt3DExtras::QCuboidMesh *meshBox2 = new Qt3DExtras::QCuboidMesh();
    meshBox2->setProperty("Vertices", QVariant(32));
    meshBox2->setProperty("Edges", QVariant(58));
    meshBox2->setProperty("Faces", QVariant(2));
    GeneralMeshModel *cuboidModel2 = new GeneralMeshModel(rootEntity, meshBox2);
    cuboidModel2->translateMesh(QVector3D(-3.0f, -1.0f, 0.0f));
    cuboidModel2->showMesh(false);


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
    setupControlPanel(vLayout, widget, detectorModel, cylinerModel, cameraWrapper);




    SwitchButton* sbtn = new SwitchButton(widget, "", "Perspe");
    SwitchButton* selectBtn = new SwitchButton(widget, "", "Select");

    vLayout->addWidget(sbtn);
    vLayout->addWidget(selectBtn);

    QObject::connect(sbtn, SIGNAL(valueChanged(bool)),  cameraWrapper, SLOT(setProjectiveMode(bool)));
    QObject::connect(selectBtn, SIGNAL(valueChanged(bool)),  cameraWrapper, SLOT(enableCameraController(bool)));
    QObject::connect(selectBtn, SIGNAL(valueChanged(bool)),  detectorModel, SLOT(enablePick(bool)));
    // Show window
    widget->show();
    widget->resize(1200, 800);

    return app.exec();
}
