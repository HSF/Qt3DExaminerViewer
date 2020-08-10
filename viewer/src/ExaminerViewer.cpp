#include "../headers/ExaminerViewer.h"
#include <QSettings>
#include <QVector4D>
#include <QtMath>
#include <QPropertyAnimation>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QRadioButton>


QCommandLinkButton *info;
QComboBox *bookmarkedView;
QVector<QVector4D> bookmarkedViewls;


ExaminerViewer::ExaminerViewer(GeneralMeshModel *cylinderModel, CameraWrapper *m_cameraWrapper)
    : m_cylinderModel(cylinderModel),  m_cameraWrapper(m_cameraWrapper){
}

void ExaminerViewer::setUpSliderController(QLabel *label, QSlider *slider, QString tip, int initalPos){
    //Setup a label
    label->setText(tip);
    label->setGeometry(10, 120, 210, 15);
    label->setMaximumHeight(20);

    //Setup a slider
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(0, 359);
    slider->setSingleStep(initalPos/10);
    slider->setValue(initalPos);
    slider->setGeometry(10, 40, 210, 20);
    slider->setMaximumHeight(25);
    slider->setTickPosition(QSlider::TicksBelow);
    slider->setTickInterval(initalPos/5);
}

void ExaminerViewer::setUpInfoWindow(){
    info = new QCommandLinkButton();
    info->setText(QStringLiteral("Info windows:"));
    info->setDescription(TIPS);
    info->setIconSize(QSize(0,0));
    info->setMaximumSize(QSize(250, 350));
    info->setMinimumSize(QSize(200, 250));
    info->setFont(QFont ("Courier", 12));
}

void ExaminerViewer::setUpVolumePanel(QVBoxLayout *vLayout, QWidget *mainWindow){
    // Control visibility of Volume
    QGroupBox *volBox = new QGroupBox("Volume Control", mainWindow);
    QVBoxLayout *volLy = new QVBoxLayout(mainWindow);

    //Switch between navigation and select mode
    QGridLayout *hLayoutSelect = new QGridLayout(mainWindow);
    QLabel *labelSel = new QLabel("mouse", mainWindow);
    QRadioButton *selectBtn = new QRadioButton("fixed", mainWindow);
    QRadioButton *viewBtn = new QRadioButton("dynamic", mainWindow);
    viewBtn->setChecked(true);
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

    QObject::connect(restoreSelectBtn, SIGNAL(clicked(bool)), m_cylinderModel, SLOT(restoreState(bool)));
    QObject::connect(selectBtn, &QRadioButton::clicked,
                     [this](bool clicked){
                      m_cameraWrapper->disableCameraController(clicked);
                      /*m_cylinderModel->enablePickAll(clicked);*/ });
    QObject::connect(viewBtn, &QRadioButton::clicked,
                     [this](bool clicked){
                      m_cameraWrapper->disableCameraController(!clicked);
                      /*m_cylinderModel->enablePickAll(!clicked);*/ });
}

QSequentialAnimationGroup *ExaminerViewer::getRoute1Tour(){
    QSequentialAnimationGroup *tour1 = new QSequentialAnimationGroup();
    int radius = m_cameraWrapper->init_distanceToOrigin-5;
    QVector4D dof1 = QVector4D(m_cameraWrapper->init_distanceToOrigin-5, 0, 0, 0);
    QVector4D dof2 = QVector4D(radius, 0, 270, 0);
    QVector4D dof3 = QVector4D(radius, 90, 270, 0);
    QVector4D dof4 = QVector4D(radius, 90, 90, 0);
    QVector4D dof5 = QVector4D(radius, -90, 90, 0);
    QVector4D dof6 = QVector4D(radius, -90, 0, 0);

    QVector3D pos = camera->camera()->position();
    int radius0 = (int)(pos.length());
    int longitude = (int)qRadiansToDegrees(qAtan2(pos[0], pos[2]));
    int latitude = (int)qRadiansToDegrees(qAtan2(pos[1], sqrt(pow(pos[0], 2) + pow(pos[2], 2))));
    QVector4D dof4start = QVector4D(radius0, latitude, longitude, 0);

    QPropertyAnimation *smoothMove1 = new QPropertyAnimation(m_cameraWrapper, "dof4");
    smoothMove1->setDuration(1000);
    smoothMove1->setStartValue(QVariant::fromValue(dof4start));
    smoothMove1->setEndValue(QVariant::fromValue(dof1));

    QPropertyAnimation *smoothMove2 = new QPropertyAnimation(m_cameraWrapper, "dof4");
    smoothMove2->setDuration(9000);
    smoothMove2->setKeyValueAt(0, dof1);
    smoothMove2->setKeyValueAt(0.3, dof2);
    smoothMove2->setKeyValueAt(0.4, dof3);
    smoothMove2->setKeyValueAt(0.6, dof4);
    smoothMove2->setKeyValueAt(0.8, dof5);
    smoothMove2->setKeyValueAt(0.9, dof6);
    smoothMove2->setKeyValueAt(1, dof1);

    QPropertyAnimation *smoothMove3 = new QPropertyAnimation(m_cameraWrapper, "dof4");
    smoothMove3->setDuration(1000);
    smoothMove3->setStartValue(QVariant::fromValue(dof1));
    smoothMove3->setEndValue(QVariant::fromValue(dof4start));
    if((dof4start - dof1).length() > 1e-1)
        tour1->addAnimation(smoothMove1);
    tour1->addAnimation(smoothMove2);
    if((dof4start - dof1).length() > 1e-1)
        tour1->addAnimation(smoothMove3);
    return tour1;
}

void ExaminerViewer::setupControlPanel(QVBoxLayout *vLayout, QWidget *mainWindow){
    /************ Info window ******************/
    // Create a info window to display mesh properties
    setUpInfoWindow();
    vLayout->addWidget(info);

    /************ Volume control******************/
    setUpVolumePanel(vLayout, mainWindow);


    /************ Camera control ******************/
    QGroupBox *cameraBox = new QGroupBox("Viewport Control", mainWindow);
    QVBoxLayout *cameraLy = new QVBoxLayout(mainWindow);
    //position control
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

    QLabel *labelScale = new QLabel(mainWindow);
    QSlider *sliderScale = new QSlider(mainWindow);
    setUpSliderController(labelScale, sliderScale, "Scale", int(m_cameraWrapper->init_distanceToOrigin));
    sliderScale->setRange(int(m_cameraWrapper->init_distanceToOrigin)/3, int(m_cameraWrapper->init_distanceToOrigin)*2);

    cameraLy->addWidget(labelScale);
    cameraLy->addWidget(sliderScale);
    QObject::connect(sliderScale,SIGNAL(valueChanged(int)), m_cameraWrapper, SLOT(zoomInOut(int)));


    cameraBox->setLayout(cameraLy);
    vLayout->addWidget(cameraBox);
    QObject::connect(perspBtn, &QRadioButton::clicked, [this](bool clicked){
                      m_cameraWrapper->setProjectiveMode(clicked); });
    QObject::connect(orthoBtn, &QRadioButton::clicked, [this](bool clicked){
                      m_cameraWrapper->setProjectiveMode(!clicked); });

    /*QTabWidget *posTab = new QTabWidget(mainWindow);
    posTab->setMaximumSize(QSize(240, 280));
    QComboBox *focusCenter = new QComboBox(mainWindow);
    focusCenter->addItem(QString("global coordinate"));
    focusCenter->addItem(QString("local coordinate"));

    // Control radius of Camera to origin
    QHBoxLayout *hLayoutRad = new QHBoxLayout(mainWindow);
    QLabel *labelScale = new QLabel(mainWindow);
    QSlider *sliderScale = new QSlider(mainWindow);
    setUpSliderController(labelScale, sliderScale, "radius", int(m_cameraWrapper->init_distanceToOrigin));
    sliderScale->setRange(1, 100);
    QSpinBox *spinScale = new QSpinBox(mainWindow);
    spinScale->setRange(1, 100);
    spinScale->setValue(int(m_cameraWrapper->init_distanceToOrigin));
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

    // orientation control
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
    directionControlLayout->addLayout(hLayoutRoll);
    directionControlLayout->addWidget(sliderRoll);
    directionControlLayout->addWidget(labelRollTicks);
    directionControlLayout->addLayout(hLayoutYaw);
    directionControlLayout->addWidget(sliderYaw);
    directionControlLayout->addWidget(labelYawTicks);
    directionControlLayout->addLayout(hLayoutPitch);
    directionControlLayout->addWidget(sliderPitch);
    directionControlLayout->addWidget(labelPitchTicks);
    QWidget *directionControl = new QWidget(mainWindow);
    directionControl->setLayout(directionControlLayout);
    directionControl->setFixedSize(230, 260);
    directionControl->setMinimumSize(QSize(100, 200));
    directionControl->setMaximumSize(QSize(230, 260));
    posTab->addTab(directionControl, "Orientation");

    //cameraLy->addWidget(posTab);
    cameraBox->setLayout(cameraLy);
    vLayout->addWidget(cameraBox);

    QObject::connect(focusCenter, SIGNAL(currentIndexChanged(int)), m_cameraWrapper, SLOT(setCoordinateCenter(int)));
    QObject::connect(perspBtn, &QRadioButton::clicked,
                     [this, sliderScale](bool clicked){
                      m_cameraWrapper->setProjectiveMode(clicked);
                      m_cameraWrapper->zoomInOut(sliderScale->value()); });
    QObject::connect(orthoBtn, &QRadioButton::clicked,
                     [this, sliderScale](bool clicked){
                      m_cameraWrapper->setProjectiveMode(!clicked);
                      m_cameraWrapper->zoomInOut(sliderScale->value()); });

    QObject::connect(sliderScale,SIGNAL(valueChanged(int)), m_cameraWrapper, SLOT(zoomInOut(int)));
    QObject::connect(sliderScale, SIGNAL(valueChanged(int)), spinScale, SLOT(setValue(int)));
    QObject::connect(spinScale, SIGNAL(valueChanged(int)), sliderScale, SLOT(setValue(int)));

    QObject::connect(sliderLat, SIGNAL(valueChanged(int)), m_cameraWrapper, SLOT(translatePosLat(int)));
    QObject::connect(sliderLat, SIGNAL(valueChanged(int)), spinLat, SLOT(setValue(int)));
    QObject::connect(spinLat, SIGNAL(valueChanged(int)), sliderLat, SLOT(setValue(int)));
    QObject::connect(sliderLat, &QSlider::valueChanged, [sliderPitch, sliderLat](){sliderPitch->setValue(-sliderLat->value());});

    QObject::connect(sliderLng, SIGNAL(valueChanged(int)), m_cameraWrapper, SLOT(translatePosLng(int)));
    QObject::connect(sliderLng, SIGNAL(valueChanged(int)), spinLng, SLOT(setValue(int)));
    QObject::connect(spinLng, SIGNAL(valueChanged(int)), sliderLng, SLOT(setValue(int)));
    QObject::connect(sliderLng, &QSlider::valueChanged, [sliderYaw, sliderLng](){sliderYaw->setValue((180 + sliderLng->value())%360 );});

    QObject::connect(sliderYaw, SIGNAL(valueChanged(int)), m_cameraWrapper, SLOT(rotateViewYaw(int)));
    QObject::connect(sliderYaw, SIGNAL(valueChanged(int)), spinYaw, SLOT(setValue(int)));
    QObject::connect(spinYaw, SIGNAL(valueChanged(int)), sliderYaw, SLOT(setValue(int)));

    QObject::connect(sliderPitch, SIGNAL(valueChanged(int)), m_cameraWrapper, SLOT(rotateViewPitch(int)));
    QObject::connect(sliderPitch, SIGNAL(valueChanged(int)), spinPitch, SLOT(setValue(int)));
    QObject::connect(spinPitch, SIGNAL(valueChanged(int)), sliderPitch, SLOT(setValue(int)));

    QObject::connect(sliderRoll, SIGNAL(valueChanged(int)), m_cameraWrapper, SLOT(rotateViewRoll(int)));
    QObject::connect(sliderRoll, SIGNAL(valueChanged(int)), spinRoll, SLOT(setValue(int)));
    QObject::connect(spinRoll, SIGNAL(valueChanged(int)), sliderRoll, SLOT(setValue(int)));

*/
    /************* quick visit *****************/
    QGroupBox *quickVisitBox = new QGroupBox("Quick Visit", mainWindow);
    QVBoxLayout *quickLy = new QVBoxLayout(mainWindow);

    // Predefined view
    QGridLayout *hLayoutPredefinedView = new QGridLayout(mainWindow);
    QLabel *tipView = new QLabel("Predef View", mainWindow);
    //QPushButton *initialViewBtn = new QPushButton("initial", mainWindow);
    //initialViewBtn->setMaximumSize(QSize(70, 25));
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
   /* bookmarkedViewls = settings.value("fullView").value<QVector<QVector<float>>>();
    for(int i = 0; i < bookmarkedViewls.size(); i++){
        bookmarkedView->addItem(QString("view") + i);
        qInfo() << i;
    }*/
    bookmarkedView->addItem("null");
    hLayoutBookmark->addWidget(bookmarkTip);
    hLayoutBookmark->addWidget(bookmarkedView);
    hLayoutBookmark->addWidget(addViewBtn);

    // start a tour
    QLabel *tourTipView = new QLabel("Start tours", mainWindow);
    QPushButton *tourBtn = new QPushButton("route 1", mainWindow);
    tourBtn->setMaximumSize(70, 25);
    hLayoutPredefinedView -> addWidget(tipView, 0, 0);
    //hLayoutPredefinedView -> addWidget(initialViewBtn, 0, 2);
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

    QObject::connect(viewAllBtn, &QPushButton::clicked, m_cameraWrapper, &CameraWrapper::viewAll);
   /* QObject::connect(initialViewBtn, &QPushButton::clicked, [this]{
        sliderRoll->setValue(0);
        sliderLat->setValue(0);
        sliderLng->setValue(0);
        sliderScale->setValue(m_cameraWrapper->init_distanceToOrigin);
        m_cameraWrapper->resetCameraView();
    });*/
    QObject::connect(frontViewBtn, &QPushButton::clicked, m_cameraWrapper,
                     [this](){
        QVector4D dof4end = QVector4D(m_cameraWrapper->camera()->viewVector().length(), 0, 0, 0);
        QVector3D pos = -camera->camera()->viewVector();
        int radius = (int)(pos.length());
        int longitude = (int)qRadiansToDegrees(qAtan2(pos[0], pos[2]));
        int latitude = (int)qRadiansToDegrees(qAtan2(pos[1], sqrt(pow(pos[0], 2) + pow(pos[2], 2))));
        QVector4D dof4start = QVector4D(radius, latitude, longitude, 0);
        QPropertyAnimation *smoothMove = new QPropertyAnimation(m_cameraWrapper, "dof4");
        smoothMove->setDuration(500);
        smoothMove->setStartValue(QVariant::fromValue(dof4start));
        smoothMove->setEndValue(QVariant::fromValue(dof4end));
        smoothMove->start();
        //m_cameraWrapper->setCustomView(dof4);
        /*sliderLat->setValue(0);
        sliderLng->setValue(0);
        sliderRoll->setValue(0);*/
    });
    QObject::connect(leftViewBtn, &QPushButton::clicked,
                     [this](){
        QVector4D dof4end = QVector4D(m_cameraWrapper->camera()->viewVector().length(), 0, -90, 0);
        QVector3D pos = -camera->camera()->viewVector();
        int radius = (int)(pos.length());
        int longitude = (int)qRadiansToDegrees(qAtan2(pos[0], pos[2]));
        int latitude = (int)qRadiansToDegrees(qAtan2(pos[1], sqrt(pow(pos[0], 2) + pow(pos[2], 2))));
        QVector4D dof4start = QVector4D(radius, latitude, longitude, 0);
        QPropertyAnimation *smoothMove = new QPropertyAnimation(m_cameraWrapper, "dof4");
        smoothMove->setDuration(500);
        smoothMove->setStartValue(QVariant::fromValue(dof4start));
        smoothMove->setEndValue(QVariant::fromValue(dof4end));
        smoothMove->start();
        //m_cameraWrapper->setCustomView(dof4);
        /*sliderLat->setValue(0);
        sliderLng->setValue(270);
        sliderRoll->setValue(0);*/
    });
    QObject::connect(topViewBtn, &QPushButton::clicked,
                     [this](){
        QVector4D dof4end = QVector4D(m_cameraWrapper->camera()->viewVector().length(), 89, 0, 0);
        QVector3D pos = -camera->camera()->viewVector();
        int radius = (int)(pos.length());
        int longitude = (int)qRadiansToDegrees(qAtan2(pos[0], pos[2]));
        int latitude = (int)qRadiansToDegrees(qAtan2(pos[1], sqrt(pow(pos[0], 2) + pow(pos[2], 2))));
        QVector4D dof4start = QVector4D(radius, latitude, longitude, 0);
        QPropertyAnimation *smoothMove = new QPropertyAnimation(m_cameraWrapper, "dof4");
        smoothMove->setDuration(500);
        smoothMove->setStartValue(QVariant::fromValue(dof4start));
        smoothMove->setEndValue(QVariant::fromValue(dof4end));
        smoothMove->start();
        //m_cameraWrapper->setCustomView(dof4);
        /*sliderLat->setValue(90);
        sliderLng->setValue(0);
        sliderRoll->setValue(0);*/
    });
    QObject::connect(tourBtn, &QPushButton::clicked, [this](){
        QAnimationGroup *aniGroup = getRoute1Tour();
        aniGroup->start();
    });
    QObject::connect(addViewBtn, &QPushButton::clicked, [this](){
        QVector3D pos = -camera->camera()->viewVector();
        int radius = (int)(pos.length());
        int latitude = (int)qRadiansToDegrees(qAtan2(pos[1], sqrt(pow(pos[0], 2) + pow(pos[2], 2))));
        int longitude = (int)qRadiansToDegrees(qAtan2(pos[0], pos[2]));
        QVector4D dof4current = QVector4D(radius, latitude, longitude, 0);

        if((!(bookmarkedViewls.empty()) && ( radius != bookmarkedViewls.last()[0]
            || latitude != bookmarkedViewls.last()[1]
            || longitude != bookmarkedViewls.last()[2]) ) || (bookmarkedViewls.empty())){
            bookmarkedView->addItem(QString("view") + QString::number(bookmarkedViewls.size()));
            bookmarkedViewls.push_back(dof4current);
            qInfo() << "clicked "  + QString::number(bookmarkedViewls.size()) + " "
                       + QString::number(bookmarkedView->count()) ;
            qInfo() << bookmarkedViewls.empty();
        } else {
            qInfo() << "empty? " << bookmarkedViewls.empty();
            qInfo() << QString::number(bookmarkedView->count());
            qInfo() << bookmarkedViewls.size();
            qInfo() << "radius: " << bookmarkedViewls.last()[0] << " vs " << radius;
            qInfo() << "latitude: " << bookmarkedViewls.last()[1] << " vs " << latitude;
            qInfo() << "longitude: " << bookmarkedViewls.last()[2] << " vs " << longitude;
        }

        });
    QObject::connect(bookmarkedView, QOverload<int>::of(&QComboBox::currentIndexChanged),
    [this](int index){

    if(index==0) return;
    QVector4D dof4end = bookmarkedViewls.at(index-1);
    QVector3D pos = -camera->camera()->viewVector();
    int radius = (int)(pos.length());
    int latitude = (int)qRadiansToDegrees(qAtan2(pos[1], sqrt(pow(pos[0], 2) + pow(pos[2], 2))));
    int longitude = (int)qRadiansToDegrees(qAtan2(pos[0], pos[2]));
    QVector4D dof4current = QVector4D(radius, latitude, longitude, 0);
    QPropertyAnimation *smoothMove = new QPropertyAnimation(m_cameraWrapper, "dof4");
    smoothMove->setDuration(500);
    smoothMove->setStartValue(QVariant::fromValue(dof4current));
    smoothMove->setEndValue(QVariant::fromValue(dof4end));
    smoothMove->start();

    /*sliderScale->setValue(dof6[0]);
    sliderLat->setValue(dof6[1]);
    sliderLng->setValue(dof6[2]);
    sliderRoll->setValue(dof6[5]);*/
    });
    /*
    QObject::connect(m_cameraWrapper, &m_cameraWrapper::viewCenterChanged, [focusCenter, bookmarkedView](QVector3D viewCenter){
        if((viewCenter-QVector3D(0.0f,0.0f,0.0f)).length() < 1e-1)
            focusCenter->setCurrentIndex(0);
        else
            focusCenter->setCurrentIndex(1);
        bookmarkedView->setCurrentIndex(0);
    });
    QObject::connect(m_cameraWrapper, &m_cameraWrapper::positionChanged, [bookmarkedView](){
        bookmarkedView->setCurrentIndex(0);
    });*/
}
