#include "headers/CameraWrapper.h"
#include "headers/GeneralMeshModel.h"
#include "headers/Mainwindow.h"
#include "headers/ModelFactory.h"
#include <QSettings>
#include <QtMath>
#include <QApplication>
#include <QGuiApplication>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
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
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QRadioButton>
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

QCommandLinkButton *info;
CameraWrapper *camera;
QComboBox *bookmarkedView;
QVector<QVector<float>> bookmarkedViewls;

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
    label->setMaximumHeight(20);

    //Setup a slider
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(0, 359);
    slider->setSingleStep(1);
    slider->setValue(initalPos);
    slider->setGeometry(10, 40, 210, 20);
    slider->setMaximumHeight(25);
    slider->setTickPosition(QSlider::TicksBelow);
    slider->setTickInterval(10);
}

void setUpInfoWindow(){
    info = new QCommandLinkButton();
    info->setText(QStringLiteral("Info windows:"));
    info->setDescription(TIPS);
    info->setIconSize(QSize(0,0));
    info->setMaximumSize(QSize(250, 150));
    info->setMinimumSize(QSize(200, 150));
    info->setFont(QFont ("Courier", 12));
}

void setUpVolumePanel(QVBoxLayout *vLayout, QWidget *mainWindow, GeneralMeshModel *cylinerModel, CameraWrapper *cameraWrapper){
    // Control visibility of Volume
    QGroupBox *volBox = new QGroupBox("Volume Control", mainWindow);
    QVBoxLayout *volLy = new QVBoxLayout(mainWindow);

    //Switch between navigation and select mode
    QGridLayout *hLayoutSelect = new QGridLayout(mainWindow);
    QLabel *labelSel = new QLabel("mouse", mainWindow);
    QRadioButton *selectBtn = new QRadioButton("select", mainWindow);
    QRadioButton *viewBtn = new QRadioButton("view", mainWindow);
    selectBtn->setChecked(true);
    hLayoutSelect->addWidget(labelSel, 0, 0);
    hLayoutSelect->addWidget(viewBtn, 0, 2);
    hLayoutSelect->addWidget(selectBtn, 0, 1);
    volLy->addLayout(hLayoutSelect);

    // Cancel selected and unpacked state
    QHBoxLayout *hLayoutRestore = new QHBoxLayout(mainWindow);
    QPushButton *restoreSelectBtn = new QPushButton("revert original state", volBox);
    restoreSelectBtn->setMaximumSize(QSize(200, 25));
    hLayoutRestore->addWidget(restoreSelectBtn);
    volLy->addLayout(hLayoutRestore);
    volBox->setLayout(volLy);
    vLayout->addWidget(volBox);

    QObject::connect(restoreSelectBtn, SIGNAL(clicked(bool)), cylinerModel, SLOT(restoreState(bool)));
    QObject::connect(selectBtn, &QRadioButton::clicked,
                     [cameraWrapper, cylinerModel](bool clicked){
                      cameraWrapper->disableCameraController(clicked);
                      cylinerModel->enablePickAll(clicked); });
    QObject::connect(viewBtn, &QRadioButton::clicked,
                     [cameraWrapper, cylinerModel](bool clicked){
                      cameraWrapper->disableCameraController(!clicked);
                      cylinerModel->enablePickAll(!clicked); });
}

inline QSequentialAnimationGroup *getRoute1Tour(CameraWrapper *cameraWrapper){
    QSequentialAnimationGroup *tour1 = new QSequentialAnimationGroup();
    int radius = cameraWrapper->init_distanceToOrigin-5;
    QVector4D dof1 = QVector4D(cameraWrapper->init_distanceToOrigin-5, 0, 0, 0);
    QVector4D dof2 = QVector4D(radius, 0, 270, 0);
    QVector4D dof3 = QVector4D(radius, 90, 270, 0);
    QVector4D dof4 = QVector4D(radius, 90, 90, 0);
    QVector4D dof5 = QVector4D(radius, -90, 90, 0);
    QVector4D dof6 = QVector4D(radius, -90, 0, 0);

    QVector4D initialView = cameraWrapper->customView();
    QPropertyAnimation *smoothMove1 = new QPropertyAnimation(cameraWrapper, "dof4");
    smoothMove1->setDuration(2000);
    smoothMove1->setStartValue(QVariant::fromValue(initialView));
    smoothMove1->setEndValue(QVariant::fromValue(dof1));

    QPropertyAnimation *smoothMove2 = new QPropertyAnimation(cameraWrapper, "dof4");
    smoothMove2->setDuration(9000);
    smoothMove2->setKeyValueAt(0, dof1);
    smoothMove2->setKeyValueAt(0.3, dof2);
    smoothMove2->setKeyValueAt(0.4, dof3);
    smoothMove2->setKeyValueAt(0.6, dof4);
    smoothMove2->setKeyValueAt(0.8, dof5);
    smoothMove2->setKeyValueAt(0.9, dof6);
    smoothMove2->setKeyValueAt(1, dof1);

    QPropertyAnimation *smoothMove3 = new QPropertyAnimation(cameraWrapper, "dof4");
    smoothMove3->setDuration(2000);
    smoothMove3->setStartValue(QVariant::fromValue(dof1));
    smoothMove3->setEndValue(QVariant::fromValue(initialView));
    if((initialView - dof1).length() > 1e-1)
        tour1->addAnimation(smoothMove1);
    tour1->addAnimation(smoothMove2);
    if((initialView - dof1).length() > 1e-1)
        tour1->addAnimation(smoothMove3);
    return tour1;
}

inline void setupControlPanel(QVBoxLayout *vLayout, QWidget *mainWindow, GeneralMeshModel *cylinerModel, CameraWrapper *cameraWrapper){
    /************ Info window ******************/
    // Create a info window to display mesh properties
    setUpInfoWindow();
    vLayout->addWidget(info);

    /************ Volume control******************/
    setUpVolumePanel(vLayout, mainWindow, cylinerModel, cameraWrapper);


    /************ Camera control ******************/
    QGroupBox *cameraBox = new QGroupBox("Viewport Control", mainWindow);
    QVBoxLayout *cameraLy = new QVBoxLayout(mainWindow);
    /**** position control ****/
    // Switch between Ortho and Perspective
    QLabel *labelProj = new QLabel("projection", mainWindow);
    QRadioButton *orthoBtn = new QRadioButton("ortho", cameraBox);
    QRadioButton *perspBtn = new QRadioButton("persp", cameraBox);
    perspBtn->setChecked(true);
    QGridLayout *hLayoutSwitch = new QGridLayout(mainWindow);
    hLayoutSwitch->addWidget(labelProj, 0, 0);
    hLayoutSwitch->addWidget(perspBtn, 0, 1);
    hLayoutSwitch->addWidget(orthoBtn, 0, 2);

    cameraLy->addLayout(hLayoutSwitch);

    QTabWidget *posTab = new QTabWidget(mainWindow);
    posTab->setMaximumSize(QSize(240, 280));
    QComboBox *focusCenter = new QComboBox(mainWindow);
    focusCenter->addItem(QString("global coordinate"));
    focusCenter->addItem(QString("local coordinate"));

    // Control radius of Camera to origin
    QHBoxLayout *hLayoutRad = new QHBoxLayout(mainWindow);
    QLabel *labelScale = new QLabel(mainWindow);
    QSlider *sliderScale = new QSlider(mainWindow);
    setUpSliderController(labelScale, sliderScale, "radius", int(cameraWrapper->init_distanceToOrigin));
    sliderScale->setRange(1, 100);
    QSpinBox *spinScale = new QSpinBox(mainWindow);
    spinScale->setRange(1, 100);
    spinScale->setValue(int(cameraWrapper->init_distanceToOrigin));
    spinScale->setMaximumWidth(50);
    hLayoutRad->addWidget(labelScale);
    hLayoutRad->addWidget(spinScale);

    // Control longitude of Camera
    QLabel *labelLng = new QLabel(mainWindow);
    QSlider *sliderLng = new QSlider(mainWindow);
    setUpSliderController(labelLng, sliderLng, "longitude", 0);
    QHBoxLayout *hLayoutLng = new QHBoxLayout(mainWindow);
    QSpinBox *spinLng = new QSpinBox(mainWindow);
    spinLng->setRange(0, 359);
    spinLng->setMaximumWidth(50);
    hLayoutLng->addWidget(labelLng);
    hLayoutLng->addWidget(spinLng);
    QLabel *labelLngTicks = new QLabel("0\t\t180\t\t359", mainWindow);
    QFont font = labelLngTicks->font();
    font.setPointSize(10);
    labelLngTicks->setFont(font);
    labelLngTicks->setMaximumHeight(10);

    // Contro latitude of Camera
    QHBoxLayout *hLayoutLat = new QHBoxLayout(mainWindow);
    QLabel *labelLat = new QLabel(mainWindow);
    QSlider *sliderLat = new QSlider(mainWindow);
    setUpSliderController(labelLat, sliderLat, "latitude", 0);
    sliderLat->setRange(-90, 90);
    QSpinBox *spinLat = new QSpinBox(mainWindow);
    spinLat->setRange(-90, 90);
    spinLat->setSingleStep(1);
    spinLat->setMaximumWidth(50);
    hLayoutLat->addWidget(labelLat);
    hLayoutLat->addWidget(spinLat);
    QLabel *labelLatTicks = new QLabel("-90\t\t  0\t\t90", mainWindow);
    labelLatTicks->setFont(font);
    labelLatTicks->setMaximumHeight(10);

    QVBoxLayout *positionControlLayout = new QVBoxLayout(mainWindow);
    positionControlLayout->addWidget(focusCenter);
    positionControlLayout->addLayout(hLayoutRad);
    positionControlLayout->addWidget(sliderScale);
    positionControlLayout->addLayout(hLayoutLng);
    positionControlLayout->addWidget(sliderLng);
    positionControlLayout->addWidget(labelLngTicks);
    positionControlLayout->addLayout(hLayoutLat);
    positionControlLayout->addWidget(sliderLat);
    positionControlLayout->addWidget(labelLatTicks);
    QWidget *positionControl = new QWidget(mainWindow);
    positionControl->setLayout(positionControlLayout);
    positionControl->setFixedSize(230, 260);
    positionControl->setMinimumSize(QSize(100, 200));
    positionControl->setMaximumSize(QSize(230, 260));
    posTab->addTab(positionControl, "Position");

    /****** orientation control*****/
    // Control yaw angle of Camera
    QLabel *labelYaw = new QLabel(mainWindow);
    QSlider *sliderYaw = new QSlider(mainWindow);
    setUpSliderController(labelYaw, sliderYaw, "yaw", 180);
    QHBoxLayout *hLayoutYaw = new QHBoxLayout(mainWindow);
    QSpinBox *spinYaw = new QSpinBox(mainWindow);
    spinYaw->setRange(0, 359);
    spinYaw->setValue(180);
    spinYaw->setMaximumWidth(50);
    hLayoutYaw->addWidget(labelYaw);
    hLayoutYaw->addWidget(spinYaw);
    QLabel *labelYawTicks = new QLabel("  0\t\t180\t\t359", mainWindow);
    labelYawTicks->setFont(font);
    labelYawTicks->setMaximumHeight(8);

    // Control pitch angle of Camera
    QLabel *labelPitch = new QLabel(mainWindow);
    QSlider *sliderPitch = new QSlider(mainWindow);
    setUpSliderController(labelPitch, sliderPitch, "pitch", 0);
    sliderPitch->setRange(-90, 90);
    QHBoxLayout *hLayoutPitch = new QHBoxLayout(mainWindow);
    QSpinBox *spinPitch = new QSpinBox(mainWindow);
    spinPitch->setRange(-90, 90);
    spinPitch->setMaximumWidth(50);
    hLayoutPitch->addWidget(labelPitch);
    hLayoutPitch->addWidget(spinPitch);
    QLabel *labelPitchTicks = new QLabel("-90\t\t  0\t\t90", mainWindow);
    labelPitchTicks->setFont(font);
    labelPitchTicks->setMaximumHeight(8);

    // Control roll angle of Camera
    QLabel *labelRoll = new QLabel(mainWindow);
    QSlider *sliderRoll = new QSlider(mainWindow);
    setUpSliderController(labelRoll, sliderRoll, "roll", 0);
    QHBoxLayout *hLayoutRoll = new QHBoxLayout(mainWindow);
    QSpinBox *spinRoll = new QSpinBox(mainWindow);
    spinRoll->setRange(0, 359);
    spinRoll->setMaximumWidth(50);
    hLayoutRoll->addWidget(labelRoll);
    hLayoutRoll->addWidget(spinRoll);
    QLabel *labelRollTicks = new QLabel("  0\t\t180\t\t359", mainWindow);
    labelRollTicks->setFont(font);
    labelRollTicks->setMaximumHeight(8);

    QVBoxLayout *directionControlLayout = new QVBoxLayout(mainWindow);
    directionControlLayout->addLayout(hLayoutYaw);
    directionControlLayout->addWidget(sliderYaw);
    directionControlLayout->addWidget(labelYawTicks);
    directionControlLayout->addLayout(hLayoutPitch);
    directionControlLayout->addWidget(sliderPitch);
    directionControlLayout->addWidget(labelPitchTicks);
    directionControlLayout->addLayout(hLayoutRoll);
    directionControlLayout->addWidget(sliderRoll);
    directionControlLayout->addWidget(labelRollTicks);
    QWidget *directionControl = new QWidget(mainWindow);
    directionControl->setLayout(directionControlLayout);
    directionControl->setFixedSize(230, 260);
    directionControl->setMinimumSize(QSize(100, 200));
    directionControl->setMaximumSize(QSize(230, 260));
    posTab->addTab(directionControl, "Orientation");

    cameraLy->addWidget(posTab);
    cameraBox->setLayout(cameraLy);
    vLayout->addWidget(cameraBox);

    QObject::connect(focusCenter, SIGNAL(currentIndexChanged(int)), cameraWrapper, SLOT(setCoordinateCenter(int)));
    QObject::connect(perspBtn, &QRadioButton::clicked,
                     [cameraWrapper, sliderScale](bool clicked){
                      cameraWrapper->setProjectiveMode(clicked);
                      cameraWrapper->zoomInOut(sliderScale->value()); });
    QObject::connect(orthoBtn, &QRadioButton::clicked,
                     [cameraWrapper, sliderScale](bool clicked){
                      cameraWrapper->setProjectiveMode(!clicked);
                      cameraWrapper->zoomInOut(sliderScale->value()); });

    QObject::connect(sliderScale,SIGNAL(valueChanged(int)), cameraWrapper, SLOT(zoomInOut(int)));
    QObject::connect(sliderScale, SIGNAL(valueChanged(int)), spinScale, SLOT(setValue(int)));
    QObject::connect(spinScale, SIGNAL(valueChanged(int)), sliderScale, SLOT(setValue(int)));

    QObject::connect(sliderLat, SIGNAL(valueChanged(int)), cameraWrapper, SLOT(translatePosLat(int)));
    QObject::connect(sliderLat, SIGNAL(valueChanged(int)), spinLat, SLOT(setValue(int)));
    QObject::connect(spinLat, SIGNAL(valueChanged(int)), sliderLat, SLOT(setValue(int)));
    QObject::connect(sliderLat, &QSlider::valueChanged, [sliderPitch, sliderLat](){sliderPitch->setValue(-sliderLat->value());});

    QObject::connect(sliderLng, SIGNAL(valueChanged(int)), cameraWrapper, SLOT(translatePosLng(int)));
    QObject::connect(sliderLng, SIGNAL(valueChanged(int)), spinLng, SLOT(setValue(int)));
    QObject::connect(spinLng, SIGNAL(valueChanged(int)), sliderLng, SLOT(setValue(int)));
    QObject::connect(sliderLng, &QSlider::valueChanged, [sliderYaw, sliderLng](){sliderYaw->setValue((180 + sliderLng->value())%360 );});

    QObject::connect(sliderYaw, SIGNAL(valueChanged(int)), cameraWrapper, SLOT(rotateViewYaw(int)));
    QObject::connect(sliderYaw, SIGNAL(valueChanged(int)), spinYaw, SLOT(setValue(int)));
    QObject::connect(spinYaw, SIGNAL(valueChanged(int)), sliderYaw, SLOT(setValue(int)));

    QObject::connect(sliderPitch, SIGNAL(valueChanged(int)), cameraWrapper, SLOT(rotateViewPitch(int)));
    QObject::connect(sliderPitch, SIGNAL(valueChanged(int)), spinPitch, SLOT(setValue(int)));
    QObject::connect(spinPitch, SIGNAL(valueChanged(int)), sliderPitch, SLOT(setValue(int)));

    QObject::connect(sliderRoll, SIGNAL(valueChanged(int)), cameraWrapper, SLOT(rotateViewRoll(int)));
    QObject::connect(sliderRoll, SIGNAL(valueChanged(int)), spinRoll, SLOT(setValue(int)));
    QObject::connect(spinRoll, SIGNAL(valueChanged(int)), sliderRoll, SLOT(setValue(int)));


    /************* quick visit *****************/
    QGroupBox *quickVisitBox = new QGroupBox("Quick Visit", mainWindow);
    QVBoxLayout *quickLy = new QVBoxLayout(mainWindow);

    // Predefined view
    QGridLayout *hLayoutPredefinedView = new QGridLayout(mainWindow);
    QLabel *tipView = new QLabel("Quick visit", mainWindow);
    QPushButton *initialViewBtn = new QPushButton("initial", mainWindow);
    initialViewBtn->setMaximumSize(QSize(70, 25));
    QPushButton *viewAllBtn = new QPushButton("view all", mainWindow);
    viewAllBtn->setMaximumSize(QSize(70, 25));
    QPushButton *frontViewBtn = new QPushButton("front", mainWindow);
    frontViewBtn->setMaximumSize(QSize(70, 25));
    QPushButton *leftViewBtn = new QPushButton("left", mainWindow);
    leftViewBtn->setMaximumSize(QSize(70, 25));
    QPushButton *topViewBtn = new QPushButton("top", mainWindow);
    topViewBtn->setMaximumSize(QSize(70, 25));

    // Bookmarked view
    QHBoxLayout *hLayoutBookmark = new QHBoxLayout(mainWindow);
    QLabel *bookmarkTip = new QLabel("Bookmarks", mainWindow);
    bookmarkedView = new QComboBox(mainWindow);
    QPushButton *addViewBtn = new QPushButton("Add", mainWindow);
    bookmarkedView->setMaximumSize(QSize(100, 25));
    addViewBtn->setMaximumSize(QSize(50,25));
    QSettings settings("ExamViewer", "CERN-HSF");
    bookmarkedViewls = settings.value("fullView").value<QVector<QVector<float>>>();
    for(int i = 0; i < bookmarkedViewls.size(); i++){
        bookmarkedView->addItem(QString("view") + i);
        qInfo() << i;
    }
    bookmarkedView->addItem("null");
    hLayoutBookmark->addWidget(bookmarkTip);
    hLayoutBookmark->addWidget(bookmarkedView);
    hLayoutBookmark->addWidget(addViewBtn);

    // start a tour
    QLabel *tourTipView = new QLabel("Start tours", mainWindow);
    QPushButton *tourBtn = new QPushButton("route 1", mainWindow);
    tourBtn->setMaximumSize(70, 25);
    hLayoutPredefinedView -> addWidget(tipView, 0, 0);
    hLayoutPredefinedView -> addWidget(initialViewBtn, 0, 2);
    hLayoutPredefinedView -> addWidget(viewAllBtn, 0, 1);
    hLayoutPredefinedView -> addWidget(frontViewBtn, 1, 0);
    hLayoutPredefinedView -> addWidget(leftViewBtn, 1, 1);
    hLayoutPredefinedView -> addWidget(topViewBtn, 1, 2);

    hLayoutPredefinedView -> addWidget(tourTipView, 3, 0);
    hLayoutPredefinedView -> addWidget(tourBtn, 3, 1);

    quickLy->addLayout(hLayoutPredefinedView);
    quickLy->addLayout(hLayoutBookmark);
    quickVisitBox->setLayout(quickLy);
    vLayout->addWidget(quickVisitBox);

    QObject::connect(viewAllBtn, &QPushButton::clicked, cameraWrapper, &CameraWrapper::viewAll);
    QObject::connect(initialViewBtn, &QPushButton::clicked, [cameraWrapper, sliderRoll, sliderLat, sliderLng, sliderScale]{
        sliderRoll->setValue(0);
        sliderLat->setValue(0);
        sliderLng->setValue(0);
        sliderScale->setValue(cameraWrapper->init_distanceToOrigin);
        cameraWrapper->resetCameraView();
    });
    QObject::connect(frontViewBtn, &QPushButton::clicked, cameraWrapper,
                     [cameraWrapper, sliderScale, sliderRoll, sliderLat, sliderLng](){
        QVector4D dof4 = QVector4D(sliderScale->value(), 0, 0, 0);
        QPropertyAnimation *smoothMove = new QPropertyAnimation(cameraWrapper, "dof4");
        smoothMove->setDuration(500);
        smoothMove->setStartValue(QVariant::fromValue(cameraWrapper->customView()));
        smoothMove->setEndValue(QVariant::fromValue(dof4));
        smoothMove->start();
        cameraWrapper->setCustomView(dof4);
        sliderLat->setValue(0);
        sliderLng->setValue(0);
        sliderRoll->setValue(0);
    });
    QObject::connect(leftViewBtn, &QPushButton::clicked,
                     [cameraWrapper, sliderScale, sliderRoll, sliderLat, sliderLng](){
        QVector4D dof4 = QVector4D(sliderScale->value(), 0, -90, 0);
        QPropertyAnimation *smoothMove = new QPropertyAnimation(cameraWrapper, "dof4");
        smoothMove->setDuration(500);
        smoothMove->setStartValue(QVariant::fromValue(cameraWrapper->customView()));
        smoothMove->setEndValue(QVariant::fromValue(dof4));
        smoothMove->start();
        cameraWrapper->setCustomView(dof4);
        sliderLat->setValue(0);
        sliderLng->setValue(270);
        sliderRoll->setValue(0);
    });
    QObject::connect(topViewBtn, &QPushButton::clicked,
                     [cameraWrapper, sliderRoll, sliderLat, sliderLng, sliderScale](){
        QVector4D dof4 = QVector4D(sliderScale->value(), 90, 0, 0);
        QPropertyAnimation *smoothMove = new QPropertyAnimation(cameraWrapper, "dof4");
        smoothMove->setDuration(500);
        smoothMove->setStartValue(QVariant::fromValue(cameraWrapper->customView()));
        smoothMove->setEndValue(QVariant::fromValue(dof4));
        smoothMove->start();
        cameraWrapper->setCustomView(dof4);
        sliderLat->setValue(90);
        sliderLng->setValue(0);
        sliderRoll->setValue(0);
    });
    QObject::connect(tourBtn, &QPushButton::clicked, [cameraWrapper](){
        QAnimationGroup *aniGroup = getRoute1Tour(cameraWrapper);
        aniGroup->start();
    });
    QObject::connect(addViewBtn, &QPushButton::clicked, [cameraWrapper](){
        QVector<float> currentView = cameraWrapper->fullCustomView();
        if((!(bookmarkedViewls.empty()) && currentView[3] != bookmarkedViewls.last()[3]
            && currentView[1] != bookmarkedViewls.last()[1]
            && currentView[0] != bookmarkedViewls.last()[0]
            ) || bookmarkedViewls.empty())
            bookmarkedView->addItem(QString("view") + QString::number(bookmarkedViewls.size()));
            bookmarkedViewls.push_back(currentView);
            qInfo() << "clicked";
        });
    QObject::connect(bookmarkedView, QOverload<int>::of(&QComboBox::currentIndexChanged),
    [cameraWrapper, sliderScale, sliderYaw, sliderPitch, sliderRoll, sliderLat, sliderLng](int index)
    {
    if(index==0) return;
    QVector<float> dof6 = bookmarkedViewls.at(index-1);
    QVector4D dof4 = QVector4D(dof6[1], dof6[2], dof6[3], dof6[4]);
    QVector3D bias = QVector3D(dof6[6], dof6[7], dof6[8]);
    cameraWrapper->setViewCenter(bias);
    cameraWrapper->setCustomView(dof4);
    sliderScale->setValue(dof6[0]);
    sliderLat->setValue(dof6[1]);
    sliderLng->setValue(dof6[2]);
    sliderPitch->setValue(dof6[3]);
    sliderYaw->setValue(dof6[4]);
    sliderRoll->setValue(dof6[5]);
    });
    /*
    QObject::connect(cameraWrapper, &CameraWrapper::viewCenterChanged, [focusCenter, bookmarkedView](QVector3D viewCenter){
        if((viewCenter-QVector3D(0.0f,0.0f,0.0f)).length() < 1e-1)
            focusCenter->setCurrentIndex(0);
        else
            focusCenter->setCurrentIndex(1);
        bookmarkedView->setCurrentIndex(0);
    });
    QObject::connect(cameraWrapper, &CameraWrapper::positionChanged, [bookmarkedView](){
        bookmarkedView->setCurrentIndex(0);
    });*/
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
    camController->setCamera(nullptr);
    camera = cameraWrapper;

    // Light source
    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    setUpLight(lightEntity, cameraEntity->position());
    QObject::connect(cameraEntity, &Qt3DRender::QCamera::positionChanged, lightEntity, [lightEntity,cameraEntity](){
        Qt3DCore::QTransform* transform = (Qt3DCore::QTransform*)lightEntity->componentsOfType<Qt3DCore::QTransform>()[0];
        transform -> setTranslation(cameraEntity->position());
    });

    // volume picking setting
    Qt3DRender::QPickingSettings *settings = new Qt3DRender::QPickingSettings();
    settings->setPickMethod(Qt3DRender::QPickingSettings::PickMethod::TrianglePicking);

    // Create mesh model
    ModelFactory *builder = new ModelFactory(rootEntity);
    builder->build3DText();
    GeneralMeshModel *cylinderModel = builder->buildVolume();
    setupControlPanel(vLayout, mainWindow, cylinderModel, cameraWrapper);

    // Show window
    mainWindow->show();
    mainWindow->resize(1200, 800);
    return app.exec();
}
