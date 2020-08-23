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
#include <QTreeWidget>

QCommandLinkButton *info;
QComboBox *bookmarkedView;
QVector<QVector4D> bookmarkedViewls;


ExaminerViewer::ExaminerViewer(GeneralMeshModel *worldModel, CameraWrapper *m_cameraWrapper)
    : m_worldModel(worldModel),  m_cameraWrapper(m_cameraWrapper){
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
    //slider->setTickPosition(QSlider::TicksBelow);
    slider->setTickInterval(initalPos/5);
}

void ExaminerViewer::setUpInfoWindow(){
    info = new QCommandLinkButton();
    info->setText(QStringLiteral("Info windows:"));
    info->setDescription(TIPS);
    info->setIconSize(QSize(0,0));
    info->setMaximumSize(QSize(300, 250));
    info->setMinimumSize(QSize(300, 250));
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
    QRadioButton *viewBtn = new QRadioButton("view", mainWindow);
    viewBtn->setChecked(true);
    hLayoutSelect->addWidget(labelSel, 0, 0);
    hLayoutSelect->addWidget(viewBtn, 0, 2);
    hLayoutSelect->addWidget(selectBtn, 0, 1);
    volLy->addLayout(hLayoutSelect);

    // Cancel selected and unpacked state
    QHBoxLayout *hLayoutRestore = new QHBoxLayout(mainWindow);
    QPushButton *restoreSelectBtn = new QPushButton("reset everything", volBox);
    restoreSelectBtn->setMaximumSize(QSize(200, 25));
    hLayoutRestore->addWidget(restoreSelectBtn);
    volLy->addLayout(hLayoutRestore);
    volBox->setLayout(volLy);
    vLayout->addWidget(volBox);

    QObject::connect(restoreSelectBtn, SIGNAL(clicked(bool)), m_worldModel, SLOT(restoreState(bool)));
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

QSequentialAnimationGroup *ExaminerViewer::getRoute2Tour(){
    QSequentialAnimationGroup *tour1 = new QSequentialAnimationGroup();
    int radius = m_cameraWrapper->init_distanceToOrigin-5;
    QVector4D dof1 = QVector4D(radius, 0, 0, 0);
    QVector4D dof2 = QVector4D(radius, 20, 90, 0);
    QVector4D dof3 = QVector4D(radius, 30, 180, 0);
    QVector4D dof4 = QVector4D(radius, 40, 270, 0);
    QVector4D dof5 = QVector4D(radius, 50, 360, 0);
    QVector4D dof6 = QVector4D(radius, 0, 360, 0);

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
    smoothMove2->setKeyValueAt(0.2, dof2);
    smoothMove2->setKeyValueAt(0.4, dof3);
    smoothMove2->setKeyValueAt(0.6, dof4);
    smoothMove2->setKeyValueAt(0.8, dof5);
    smoothMove2->setKeyValueAt(0.99999, dof6);
    smoothMove2->setKeyValueAt(1.0, dof1);

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

inline void loopDaughters(QTreeWidgetItem *item, GeneralMeshModel *model){
    int n = model->subModelCount();
    for(int i = 0; i < n; i++){
        GeneralMeshModel *daughter = model->getSubModel(i);
        QTreeWidgetItem *subItem = new QTreeWidgetItem();
        subItem->setText(0, QString::fromStdString(daughter->Volume()->getLogVol()->getName()));
        subItem->setText(1, QString::number(daughter->subModelCount()));
        loopDaughters(subItem, daughter);
        item->addChild(subItem);
    }
}

inline GeneralMeshModel *queryItem(QTreeWidgetItem *parent, GeneralMeshModel *model, QTreeWidgetItem *target){
    int idx = parent->indexOfChild(target);
    if(idx != -1)
        return model->getSubModel(idx);
    for(int i = 0; i < model->subModelCount(); i++){
        return queryItem(parent->child(i), model->getSubModel(i), target);
    }
    return nullptr;
}

void ExaminerViewer::setupControlPanel(QVBoxLayout *vLayout, QWidget *mainWindow){
    /************ Info window ******************/
    // Create a info window to display mesh properties
    setUpInfoWindow();
    vLayout->addWidget(info);

    QTreeWidget *treeWidget = new QTreeWidget(mainWindow);
    treeWidget->setColumnCount(2);
    treeWidget->setColumnWidth(0, 200);
    treeWidget->setColumnWidth(1, 50);
    QStringList headerLabels;
    headerLabels.push_back("volume");
    headerLabels.push_back("#children");
    treeWidget->setHeaderLabels(headerLabels);
    QTreeWidgetItem *topItem = new QTreeWidgetItem(treeWidget);
    topItem->setText(0, "world");
    topItem->setText(1, QString::number(m_worldModel->subModelCount()));
    loopDaughters(topItem, m_worldModel);
    QObject::connect(treeWidget, &QTreeWidget::itemClicked, [this, topItem, treeWidget](QTreeWidgetItem *item){
        int idx = treeWidget->indexOfTopLevelItem(item);
        if(idx != -1){
            this->m_cameraWrapper->viewAll();
        }
        GeneralMeshModel *target = queryItem(topItem, m_worldModel, item);
        if(target==nullptr) return;
        this->m_cameraWrapper->camera()->viewEntity(target->m_meshEntity);
        target->showMesh(true);
        target->getSelected();
    });
    treeWidget->expandItem(topItem);
    treeWidget->insertTopLevelItem(0, topItem);
    vLayout->addWidget(treeWidget);

    vLayout->addWidget(treeWidget);

    /************ Volume control******************/
    setUpVolumePanel(vLayout, mainWindow);


    /************ Camera control ******************/
    QGroupBox *cameraBox = new QGroupBox("Viewport Control", mainWindow);
    QVBoxLayout *cameraLy = new QVBoxLayout(mainWindow);

    // Switch between Ortho and Perspective
    QLabel *labelProj = new QLabel("projection", mainWindow);
    QRadioButton *orthoBtn = new QRadioButton("ortho", cameraBox);
    QRadioButton *perspBtn = new QRadioButton("persp", cameraBox);
    perspBtn->setChecked(true);
    QObject::connect(perspBtn, &QRadioButton::clicked, [this](bool clicked){
                      m_cameraWrapper->setProjectiveMode(clicked); });
    QObject::connect(orthoBtn, &QRadioButton::clicked, [this](bool clicked){
                      m_cameraWrapper->setProjectiveMode(!clicked); });

    QGridLayout *hLayoutSwitch = new QGridLayout(mainWindow);
    hLayoutSwitch->addWidget(labelProj, 0, 0);
    hLayoutSwitch->addWidget(perspBtn, 0, 1);
    hLayoutSwitch->addWidget(orthoBtn, 0, 2);
    cameraLy->addLayout(hLayoutSwitch);

    // Control scale fo zoom
    QLabel *labelScale = new QLabel(mainWindow);
    QSlider *sliderScale = new QSlider(mainWindow);
    setUpSliderController(labelScale, sliderScale, "Scale", int(m_cameraWrapper->init_distanceToOrigin));
    sliderScale->setRange(int(m_cameraWrapper->init_distanceToOrigin)/4, int(m_cameraWrapper->init_distanceToOrigin*1.5));
    sliderScale->setValue(int(m_cameraWrapper->init_distanceToOrigin));

    QHBoxLayout *hLayoutScale = new QHBoxLayout(mainWindow);
    hLayoutScale->addWidget(labelScale);
    hLayoutScale->addWidget(sliderScale);
    cameraLy->addLayout(hLayoutScale);
    cameraBox->setLayout(cameraLy);
    vLayout->addWidget(cameraBox);
    QObject::connect(sliderScale,SIGNAL(valueChanged(int)), m_cameraWrapper, SLOT(zoomInOut(int)));

    QTabWidget *posTab = new QTabWidget(mainWindow);
    posTab->setMaximumSize(QSize(240, 280));

/*
    // Control vertical rotation of Camera
    QLabel *labelLng = new QLabel(mainWindow);
    QSlider *sliderLng = new QSlider(mainWindow);
    setUpSliderController(labelLng, sliderLng, "longitude", 0);
    QHBoxLayout *hLayoutLng = new QHBoxLayout(mainWindow);
    hLayoutLng->addWidget(labelLng);
    hLayoutLng->addWidget(sliderLng);

    // Control horizontional rotation of Camera
    QLabel *labelLat = new QLabel(mainWindow);
    QSlider *sliderLat = new QSlider(mainWindow);
    setUpSliderController(labelLat, sliderLat, "latitude", 0);
    sliderLat->setRange(-90, 90);
    QHBoxLayout *hLayoutLat = new QHBoxLayout(mainWindow);
    hLayoutLat->addWidget(labelLat);
    hLayoutLat->addWidget(sliderLat);

    QVBoxLayout *positionControlLayout = new QVBoxLayout(mainWindow);
    positionControlLayout->addLayout(hLayoutLat);
    positionControlLayout->addLayout(hLayoutLng);
    QWidget *positionControl = new QWidget(mainWindow);
    positionControl->setLayout(positionControlLayout);
    positionControl->setFixedSize(230, 260);
    positionControl->setMinimumSize(QSize(100, 100));
    positionControl->setMaximumSize(QSize(230, 130));
    posTab->addTab(positionControl, "Rotation");

    QObject::connect(sliderLat, SIGNAL(valueChanged(int)), m_cameraWrapper, SLOT(translatePosLat(int)));
    QObject::connect(sliderLng, SIGNAL(valueChanged(int)), m_cameraWrapper, SLOT(translatePosLng(int)));



    // Control vertical translation of Camera
    QLabel *labelHorz = new QLabel(mainWindow);
    QSlider *sliderHorz = new QSlider(mainWindow);
    setUpSliderController(labelHorz, sliderHorz, "horizontal", 0);
    QHBoxLayout *hLayoutHorz = new QHBoxLayout(mainWindow);
    hLayoutHorz->addWidget(labelHorz);
    hLayoutHorz->addWidget(sliderHorz);

    // Control horizontional translation of Camera
    QHBoxLayout *hLayoutVert = new QHBoxLayout(mainWindow);
    QLabel *labelVert = new QLabel(mainWindow);
    QSlider *sliderVert = new QSlider(mainWindow);
    setUpSliderController(labelVert, sliderVert, "vertical", 0);
    sliderVert->setRange(-90, 90);
    hLayoutVert->addWidget(labelVert);
    hLayoutVert->addWidget(sliderVert);

    QVBoxLayout *translationControlLayout = new QVBoxLayout(mainWindow);
    translationControlLayout->addLayout(hLayoutHorz);
    translationControlLayout->addLayout(hLayoutVert);
    QWidget *translationControl = new QWidget(mainWindow);
    translationControl->setLayout(translationControlLayout);
    translationControl->setFixedSize(230, 260);
    translationControl->setMinimumSize(QSize(100, 100));
    translationControl->setMaximumSize(QSize(230, 130));
    posTab->addTab(translationControl, "translation");


    cameraLy->addWidget(posTab);*/
    cameraBox->setLayout(cameraLy);
    vLayout->addWidget(cameraBox);

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
    bookmarkedView->addItem("null");
    hLayoutBookmark->addWidget(bookmarkTip);
    hLayoutBookmark->addWidget(bookmarkedView);
    hLayoutBookmark->addWidget(addViewBtn);

    // start a tour
    QLabel *tourTipView = new QLabel("Start tours", mainWindow);
    QPushButton *tour1Btn = new QPushButton("route 1", mainWindow);
    QPushButton *tour2Btn = new QPushButton("route 2", mainWindow);
    tour1Btn->setMaximumSize(70, 25);
    tour2Btn->setMaximumSize(70, 25);
    hLayoutPredefinedView -> addWidget(tipView, 0, 0);
    hLayoutPredefinedView -> addWidget(viewAllBtn, 0, 1);
    hLayoutPredefinedView -> addWidget(frontViewBtn, 1, 0);
    hLayoutPredefinedView -> addWidget(leftViewBtn, 1, 1);
    hLayoutPredefinedView -> addWidget(topViewBtn, 1, 2);

    hLayoutPredefinedView -> addWidget(tourTipView, 3, 0);
    hLayoutPredefinedView -> addWidget(tour1Btn, 3, 1);
    hLayoutPredefinedView -> addWidget(tour2Btn, 3, 2);

    quickLy->addLayout(hLayoutPredefinedView);
    quickLy->addLayout(hLayoutBookmark);
    quickVisitBox->setLayout(quickLy);
    vLayout->addWidget(quickVisitBox);

    QObject::connect(viewAllBtn, &QPushButton::clicked, m_cameraWrapper, &CameraWrapper::viewAll);
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
    });
    QObject::connect(tour1Btn, &QPushButton::clicked, [this](){
        QAnimationGroup *aniGroup = getRoute1Tour();
        aniGroup->start();
    });
    QObject::connect(tour2Btn, &QPushButton::clicked, [this](){
        QAnimationGroup *aniGroup = getRoute2Tour();
        aniGroup->start();
    });
    QObject::connect(addViewBtn, &QPushButton::clicked, [](){
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
    });
}
