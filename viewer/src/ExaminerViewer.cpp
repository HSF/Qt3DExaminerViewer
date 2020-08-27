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

ExaminerViewer::ExaminerViewer(GeneralMeshModel *worldModel, CameraWrapper *cameraWrapper)
    : m_worldModel(worldModel),  m_cameraWrapper(cameraWrapper){
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
    info->setMinimumSize(QSize(150, 250));
    info->setFont(QFont ("Courier", 12));
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

    QVector3D pos = m_cameraWrapper->camera()->position();
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

    QVector3D pos = m_cameraWrapper->camera()->position();
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
        GeneralMeshModel *res = queryItem(parent->child(i), model->getSubModel(i), target);
        if(res != nullptr)
            return res;
    }
    return nullptr;
}

void ExaminerViewer::setupControlPanel(QVBoxLayout *vLayout, QWidget *mainWindow){
    /************ Info window ******************/
    // Create a info window to display mesh properties
    setUpInfoWindow();
    vLayout->addWidget(info);

    QTreeWidget *treeWidget = new QTreeWidget(mainWindow);
    treeWidget->setMaximumSize(QSize(300, 150));
    treeWidget->setMinimumSize(QSize(150, 100));
    treeWidget->setColumnCount(2);
    treeWidget->setColumnWidth(0, 200);
    treeWidget->setColumnWidth(1, 50);
    QStringList headerLabels;
    headerLabels.push_back("volume");
    headerLabels.push_back("#children");
    treeWidget->setHeaderLabels(headerLabels);
    QTreeWidgetItem *volumeItem = new QTreeWidgetItem(treeWidget);
    volumeItem->setText(0, "world");
    volumeItem->setText(1, QString::number(m_worldModel->subModelCount()));
    loopDaughters(volumeItem, m_worldModel);
    qInfo() << "count: " << m_worldModel->subModelCount();
    if(m_worldModel->subModelCount() == 1)
        m_cameraWrapper->translateView(m_worldModel->getSubModel(0)->m_meshTransform->translation(),
                                       m_cameraWrapper->init_distanceToOrigin);
        //m_cameraWrapper->camera()->viewEntity(m_worldModel->getSubModel(0)->m_meshEntity);
    QObject::connect(treeWidget, &QTreeWidget::itemClicked, [this, volumeItem, treeWidget](QTreeWidgetItem *item){
        int idx = treeWidget->indexOfTopLevelItem(item);
        if(idx != -1){
            this->m_cameraWrapper->viewAll();
            return;
        }
        GeneralMeshModel *target = queryItem(volumeItem, m_worldModel, item);
        if(target==nullptr) {
            return;
        }
        m_cameraWrapper->translateView(target->m_meshTransform->translation(), m_cameraWrapper->init_distanceToOrigin);
        target->showMesh(true);
        target->getSelected();
    });
    treeWidget->expandItem(volumeItem);
    treeWidget->insertTopLevelItem(0, volumeItem);
    vLayout->addWidget(treeWidget);


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

    // reset volume and view
    QHBoxLayout *hLayoutRestore = new QHBoxLayout(mainWindow);
    QPushButton *restoreSelectBtn = new QPushButton("reset volume");
    QPushButton *restoreViewBtn = new QPushButton("reset view");
    restoreSelectBtn->setMaximumSize(QSize(200, 25));
    hLayoutRestore->addWidget(restoreSelectBtn);
    hLayoutRestore->addWidget(restoreViewBtn);
    cameraLy->addLayout(hLayoutRestore);
    QObject::connect(restoreSelectBtn, SIGNAL(clicked(bool)), m_worldModel, SLOT(restoreState(bool)));
    QObject::connect(restoreViewBtn, &QPushButton::clicked, [this](){
        m_cameraWrapper->camera()->setPosition(QVector3D(0, 0, m_cameraWrapper->init_distanceToOrigin));
        m_cameraWrapper->camera()->setUpVector(QVector3D(0, 1, 0));
        m_cameraWrapper->camera()->setViewCenter(QVector3D(0, 0, 0));
        m_cameraWrapper->viewAll();
    });
    cameraBox->setLayout(cameraLy);
    vLayout->addWidget(cameraBox);



    /************* quick visit *****************/
    QGroupBox *quickVisitBox = new QGroupBox("Quick Visit", mainWindow);
    QVBoxLayout *quickLy = new QVBoxLayout(mainWindow);

    // Predefined view
    QGridLayout *hLayoutPredefinedView = new QGridLayout(mainWindow);
    QLabel *tipView = new QLabel("Predef View", mainWindow);
    QPushButton *viewAllBtn = new QPushButton("view all", mainWindow);
    viewAllBtn->setMaximumSize(QSize(70, 25));
    QPushButton *frontViewBtn = new QPushButton("front", mainWindow);
    frontViewBtn->setMaximumSize(QSize(70, 25));
    QPushButton *leftViewBtn = new QPushButton("left", mainWindow);
    leftViewBtn->setMaximumSize(QSize(70, 25));
    QPushButton *topViewBtn = new QPushButton("top", mainWindow);
    topViewBtn->setMaximumSize(QSize(70, 25));

    // Bookmarked view
    QComboBox *bookmarkedView = new QComboBox(mainWindow);
    QVector<QVector4D> *bookmarkedViewls = new QVector<QVector4D>;
    QHBoxLayout *hLayoutBookmark = new QHBoxLayout(mainWindow);
    QLabel *bookmarkTip = new QLabel("Bookmarks", mainWindow);
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
    hLayoutPredefinedView -> addWidget(viewAllBtn, 0, 2);
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
        QVector3D pos = -m_cameraWrapper->camera()->viewVector();
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
        QVector3D pos = -m_cameraWrapper->camera()->viewVector();
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
        QVector3D pos = -m_cameraWrapper->camera()->viewVector();
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
    QObject::connect(addViewBtn, &QPushButton::clicked, [this, bookmarkedViewls, bookmarkedView](){
        QVector3D pos = -m_cameraWrapper->camera()->viewVector();
        int radius = (int)(pos.length());
        int latitude = (int)qRadiansToDegrees(qAtan2(pos[1], sqrt(pow(pos[0], 2) + pow(pos[2], 2))));
        int longitude = (int)qRadiansToDegrees(qAtan2(pos[0], pos[2]));
        const QVector4D dof4current = QVector4D(radius, latitude, longitude, 0);

        if((!(bookmarkedViewls->empty()) && ( radius != bookmarkedViewls->last()[0]
            || latitude != bookmarkedViewls->last()[1]
            || longitude != bookmarkedViewls->last()[2]) ) || (bookmarkedViewls->empty())){
            bookmarkedView->addItem(QString("view") + QString::number(bookmarkedViewls->size()));
            bookmarkedViewls->push_back(dof4current);
        }
        });
    QObject::connect(bookmarkedView, QOverload<int>::of(&QComboBox::currentIndexChanged),
    [this, bookmarkedViewls](int index){
        if(index==0) return;
        QVector4D dof4end = bookmarkedViewls->at(index-1);
        QVector3D pos = -m_cameraWrapper->camera()->viewVector();
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
