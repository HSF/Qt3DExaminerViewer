#include <QApplication>

#include "headers/MeshModel.h"
#include "headers/SwithButton.h"
#include "headers/CameraWrapper.h"

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

#include <Qt3DExtras/qforwardrenderer.h>
#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DExtras/qfirstpersoncameracontroller.h>
#include <Qt3DExtras/qorbitcameracontroller.h>

QCommandLinkButton *info;

void setUpCamera(Qt3DRender::QCamera *cameraEntity){
    //cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setProjectionType(Qt3DRender::QCameraLens::OrthographicProjection);
    cameraEntity->setPosition(QVector3D(0, 0, 20.0f));
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));
}

void setUpLight(Qt3DCore::QEntity *lightEntity, Qt3DRender::QCamera *cameraEntity){
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);

    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(cameraEntity->position());
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
    info->setFont(QFont ("Courier", 15));
    info->show();
}

void setupControlPanel(QVBoxLayout *vLayout, QWidget *widget, MeshModel *dectectorModel){
    // Create a info window to display mesh properties
    setUpInfoWindow();

    // Control visibility of Volume
    QCheckBox *meshCB = new QCheckBox(widget);
    meshCB->setChecked(true);
    meshCB->setText(QStringLiteral("Display Detector Volume"));

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
    restoreBtn->setFixedSize(QSize(80, 20));
    restoreBtn->setText(QString("cancel select"));

    vLayout->addWidget(info);
    vLayout->addWidget(meshCB);
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
    QObject::connect(meshCB, &QCheckBox::stateChanged, dectectorModel, &MeshModel::showMesh);
    QObject::connect(sliderScale, &QSlider::valueChanged, dectectorModel, &MeshModel::scaleMesh);
    QObject::connect(sliderX, SIGNAL(valueChanged(int)), dectectorModel, SLOT(rotateMeshX(int)));
    QObject::connect(sliderY, SIGNAL(valueChanged(int)), dectectorModel, SLOT(rotateMeshY(int)));
    QObject::connect(sliderZ, SIGNAL(valueChanged(int)), dectectorModel, SLOT(rotateMeshZ(int)));
    QObject::connect(restoreBtn, SIGNAL(clicked(bool)), dectectorModel, SLOT(restoreState(bool)));

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
    widget->setWindowTitle(QStringLiteral("Basic shapes"));

    // Root entity
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

    // Camera and Camera controls
    Qt3DRender::QCamera *cameraEntity = view->camera();
    Qt3DExtras::QOrbitCameraController *camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    setUpCamera(cameraEntity);
    CameraWrapper *cameraWrapper = new CameraWrapper(widget, cameraEntity);
    cameraWrapper->addCameraController(camController);


    camController->setCamera(cameraEntity);

    // Light
    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    setUpLight(lightEntity, cameraEntity);

    // Set root object of the scene
    view->setRootEntity(rootEntity);

    // Create detector mesh model
    MeshModel *dectectorModel = new MeshModel(rootEntity);
    setupControlPanel(vLayout, widget, dectectorModel);

    SwitchButton* sbtn = new SwitchButton(widget);
    SwitchButton* selectBtn = new SwitchButton(widget);

    vLayout->addWidget(sbtn);
    vLayout->addWidget(selectBtn);

    QObject::connect(sbtn, SIGNAL(valueChanged(bool)),  cameraWrapper, SLOT(setProjectiveMode(bool)));
    QObject::connect(selectBtn, SIGNAL(valueChanged(bool)),  cameraWrapper, SLOT(enableCameraController(bool)));
    QObject::connect(selectBtn, SIGNAL(valueChanged(bool)),  dectectorModel, SLOT(enablePick(bool)));

    // Show window
    widget->show();
    widget->resize(1200, 800);

    return app.exec();
}
