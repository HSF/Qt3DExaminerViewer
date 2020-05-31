#include <QApplication>

#include "meshModel.h"

#include <QGuiApplication>

#include <Qt3DRender/qcamera.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcameralens.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCommandLinkButton>
#include <QtGui/QScreen>

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

#include <Qt3DRender/qrenderaspect.h>
#include <Qt3DExtras/qforwardrenderer.h>

#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DExtras/qfirstpersoncameracontroller.h>
QCommandLinkButton *info;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
    QWidget *container = QWidget::createWindowContainer(view);
    QSize screenSize = view->screen()->size();
    container->setMinimumSize(QSize(200, 100));
    container->setMaximumSize(screenSize);

    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setAlignment(Qt::AlignTop);
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);

    widget->setWindowTitle(QStringLiteral("Basic shapes"));

    // Root entity
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

    // Camera
    Qt3DRender::QCamera *cameraEntity = view->camera();

    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(0, 0, 20.0f));
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));

    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(cameraEntity->position());
    lightEntity->addComponent(lightTransform);

    // For camera controls
    Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
    camController->setCamera(cameraEntity);
    // Scenemodifier
    SceneModifier *modifier = new SceneModifier(rootEntity);

    // Set root object of the scene
    view->setRootEntity(rootEntity);

    // Create control widgets
    info = new QCommandLinkButton();
    info->setText(QStringLiteral("Some info about mesh:"));
    info->setDescription(QString::fromLatin1("Defaut message\n without\n click any object."));
    info->setIconSize(QSize(0,0));
    info->setFixedSize(QSize(200, 100));
    info->setFont(QFont ("Courier", 15));

    // Create a checkbox
    QCheckBox *meshCB = new QCheckBox(widget);
    meshCB->setChecked(true);
    meshCB->setText(QStringLiteral("Display Detector Mesh"));


    //create a label
    QLabel *labelScale = new QLabel(widget);
    labelScale->setText("Scale slider (0.001-0.011)");
    labelScale->setGeometry(10, 120, 210, 20);
    labelScale->show();
    // create a slider
    QSlider *sliderScale = new QSlider(widget);
    sliderScale->setOrientation(Qt::Horizontal);
    sliderScale->setRange(0, 100);
    sliderScale->setValue(50);
    sliderScale->setGeometry(10, 40, 210, 30);


    //create a label
    QLabel *labelX = new QLabel(widget);
    labelX->setText("rotation slider X (0-360)");
    labelX->setGeometry(10, 70, 210, 20);
    labelX->show();
    // create a slider
    QSlider *sliderX = new QSlider(widget);
    sliderX->setOrientation(Qt::Horizontal);
    sliderX->setRange(0, 100);
    sliderX->setValue(0);
    sliderX->setGeometry(10, 40, 210, 30);

    //create a label
    QLabel *labelY = new QLabel(widget);
    labelY->setText("rotation slider Y (0-360)");
    labelY->setGeometry(10, 70, 210, 20);
    labelY->show();
    // create a slider
    QSlider *sliderY = new QSlider(widget);
    sliderY->setOrientation(Qt::Horizontal);
    sliderY->setRange(0, 100);
    sliderY->setValue(0);
    sliderY->setGeometry(10, 40, 210, 30);

    //create a label
    QLabel *labelZ= new QLabel(widget);
    labelZ->setText("rotation slider Z (0-360)");
    labelZ->setGeometry(10, 70, 210, 20);
    labelZ->show();
    // create a slider
    QSlider *sliderZ = new QSlider(widget);
    sliderZ->setOrientation(Qt::Horizontal);
    sliderZ->setRange(0, 100);
    sliderZ->setValue(0);
    sliderZ->setGeometry(10, 40, 210, 30);

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

    QObject::connect(meshCB, &QCheckBox::stateChanged, modifier, &SceneModifier::enableMesh);
    QObject::connect(sliderScale, &QSlider::valueChanged, modifier, &SceneModifier::scaleMesh);
    QObject::connect(sliderX, SIGNAL(valueChanged(int)), modifier, SLOT(rotateMeshX(int)));
    QObject::connect(sliderY, SIGNAL(valueChanged(int)), modifier, SLOT(rotateMeshY(int)));
    QObject::connect(sliderZ, SIGNAL(valueChanged(int)), modifier, SLOT(rotateMeshZ(int)));

    // Show window
    widget->show();
    widget->resize(1200, 800);

    return app.exec();
}
