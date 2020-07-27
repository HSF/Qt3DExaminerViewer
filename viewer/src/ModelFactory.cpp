#include "../headers/ModelFactory.h"
#include <QColor>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QExtrudedTextMesh>

ModelFactory::ModelFactory(Qt3DCore::QEntity *rootEntity): m_rootEntity(rootEntity){
}

GeneralMeshModel **ModelFactory::build3DText(){

    QColor qColor = QColor(147, 147, 147, 147);
    Qt3DExtras::QExtrudedTextMesh *textMesh1 = new Qt3DExtras::QExtrudedTextMesh();
    textMesh1->setObjectName("Z+");
    textMesh1->setText("Z+");
    GeneralMeshModel *textModel1 = new GeneralMeshModel(m_rootEntity, textMesh1);
    textModel1->translateMesh(QVector3D(0.0f, 0.0f, 100.0f));
    textModel1->scaleMesh(QVector3D(1.0f, 1.0f, 0.2f));
    textModel1->enablePickAll(false);
    textModel1->setColor(qColor);

    Qt3DExtras::QExtrudedTextMesh *textMesh2 = new Qt3DExtras::QExtrudedTextMesh();
    textMesh2->setObjectName("Z-");
    textMesh2->setText("Z-");
    GeneralMeshModel *textModel2 = new GeneralMeshModel(m_rootEntity, textMesh2);
    textModel2->translateMesh(QVector3D(0.0f, 0.0f, -100.0f));
    textModel2->scaleMesh(QVector3D(1.0f, 1.0f, 0.2f));
    textModel2->enablePickAll(false);
    textModel2->setColor(qColor);


    Qt3DExtras::QExtrudedTextMesh *textMesh3 = new Qt3DExtras::QExtrudedTextMesh();
    textMesh3->setObjectName("Y+");
    textMesh3->setText("Y+");
    GeneralMeshModel *textModel3 = new GeneralMeshModel(m_rootEntity, textMesh3);
    textModel3->translateMesh(QVector3D(0.0f, 100.0f, 0.0f));
    textModel3->scaleMesh(QVector3D(1.0f, 1.0f, 0.2f));
    textModel3->enablePickAll(false);
    textModel3->setColor(qColor);

    Qt3DExtras::QExtrudedTextMesh *textMesh4 = new Qt3DExtras::QExtrudedTextMesh();
    textMesh4->setObjectName("Y-");
    textMesh4->setText("Y-");
    GeneralMeshModel *textModel4 = new GeneralMeshModel(m_rootEntity, textMesh4);
    textModel4->translateMesh(QVector3D(0.0f, -100.0f, 0.0f));
    textModel4->scaleMesh(QVector3D(1.0f, 1.0f, 0.2f));
    textModel4->enablePickAll(false);
    textModel4->setColor(qColor);


    Qt3DExtras::QExtrudedTextMesh *textMesh5 = new Qt3DExtras::QExtrudedTextMesh();
    textMesh5->setObjectName("X+");
    textMesh5->setText("X+");
    GeneralMeshModel *textModel5 = new GeneralMeshModel(m_rootEntity, textMesh5);
    textModel5->translateMesh(QVector3D(100.0f, 0.0f, 0.0f));
    textModel5->scaleMesh(QVector3D(1.0f, 1.0f, 0.2f));
    textModel5->enablePickAll(false);
    textModel5->setColor(qColor);

    Qt3DExtras::QExtrudedTextMesh *textMesh6 = new Qt3DExtras::QExtrudedTextMesh();
    textMesh6->setObjectName("X-");
    textMesh6->setText("X-");
    GeneralMeshModel *textModel6 = new GeneralMeshModel(m_rootEntity, textMesh6);
    textModel6->translateMesh(QVector3D(-100.0f, 0.0f, 0.0f));
    textModel6->scaleMesh(QVector3D(1.0f, 1.0f, 0.2f));
    textModel6->enablePickAll(false);
    textModel6->setColor(qColor);
    GeneralMeshModel **textList = new GeneralMeshModel*[6];
    textList[0] = textModel1;
    textList[1] = textModel2;
    textList[2] = textModel3;
    textList[3] = textModel4;
    textList[4] = textModel5;
    textList[5] = textModel6;
    return textList;
}

GeneralMeshModel *ModelFactory::buildVolume(){
    // geometry model
    Qt3DExtras::QCylinderMesh *meshCyliner = new Qt3DExtras::QCylinderMesh();
    meshCyliner->setObjectName(QString("World Volume"));
    meshCyliner->setProperty("maxLength", 4);
    GeneralMeshModel *cylinderModel = new GeneralMeshModel(m_rootEntity, meshCyliner);
    cylinderModel->translateMesh(QVector3D(0.0f, 0.0f, 0.0f));
    cylinderModel->rotateMesh(Qt3DCore::QTransform::fromEulerAngles(90.0,0,0));
    cylinderModel->scaleMesh(QVector3D(2,5,2));

    Qt3DExtras::QCuboidMesh *meshBox1 = new Qt3DExtras::QCuboidMesh();
    meshBox1->setObjectName(QString("Muon \n"));
    meshBox1->setProperty("maxLength", 2);
    GeneralMeshModel *cuboidModel1 = new GeneralMeshModel(m_rootEntity, meshBox1);
    cuboidModel1->translateMesh(QVector3D(0.0f, 0.0f, 1.5f));
    cuboidModel1->scaleMesh(QVector3D(2,2,2));

    Qt3DExtras::QCuboidMesh *meshBox2 = new Qt3DExtras::QCuboidMesh();
    meshBox2->setObjectName(QString("Calorimeter \n"));
    meshBox2->setProperty("maxLength", 2);
    GeneralMeshModel *cuboidModel2 = new GeneralMeshModel(m_rootEntity, meshBox2);
    cuboidModel2->translateMesh(QVector3D(0.0f, 0.0f, -1.5f));
    cuboidModel2->scaleMesh(QVector3D(2,2,2));

    Qt3DExtras::QCuboidMesh *meshBox3 = new Qt3DExtras::QCuboidMesh();
    meshBox3->setObjectName(QString("one daughter of Muon"));
    meshBox3->setProperty("maxLength", 1);
    GeneralMeshModel *cuboidModel3 = new GeneralMeshModel(m_rootEntity, meshBox3);
    cuboidModel3->translateMesh(QVector3D(0.0f, 0.0f, 1.5f));

    Qt3DExtras::QSphereMesh *meshSphere = new Qt3DExtras::QSphereMesh();
    meshSphere->setObjectName(QString("one daughter of Calorimeter"));
    meshSphere->setProperty("maxLength", 1);
    GeneralMeshModel *sphereModel = new GeneralMeshModel(m_rootEntity, meshSphere);
    sphereModel->translateMesh(QVector3D(0.0f, 0.0f, -1.5f));

    cylinderModel->addSubModel(cuboidModel1);
    cylinderModel->addSubModel(cuboidModel2);
    cuboidModel1->addSubModel(cuboidModel3);
    cuboidModel2->addSubModel(sphereModel);
    /*Qt3DRender::QMesh *mesh = new Qt3DRender::QMesh();
    mesh->setSource(QUrl("qrc:///mesh/Detector.obj"));
    mesh->setObjectName(QString("ATLAS detector volume"));
    mesh->setProperty("Vertices", QVariant(37216));
    mesh->setProperty("Edges", QVariant(58416));
    mesh->setProperty("Faces", QVariant(29208));
    GeneralMeshModel *detectorModel = new GeneralMeshModel(rootEntity, mesh);
    detectorModel->scaleMesh(QVector3D(0.006f, 0.006f, 0.006f));
    Qt3DRender::QMesh *meshLeft = new Qt3DRender::QMesh();
    meshLeft->setSource(QUrl("qrc:///mesh/left_part.obj"));
    meshLeft->setProperty("Vertices", QVariant(3));
    meshLeft->setProperty("Edges", QVariant(5));
    meshLeft->setProperty("Faces", QVariant(29));
    MeshModel *subModelLeft = new MeshModel(rootEntity, meshLeft);
    subModelLeft->scaleMesh(QVector3D(0.006f, 0.006f, 0.006f));

    Qt3DRender::QMesh *meshRight = new Qt3DRender::QMesh();
    meshRight->setSource(QUrl("qrc:///mesh/right_part.obj"));
    meshRight->setProperty("Vertices", QVariant(32));
    meshRight->setProperty("Edges", QVariant(58));
    meshRight->setProperty("Faces", QVariant(2));
    MeshModel *subModelRight = new MeshModel(rootEntity, meshRight);
    subModelRight->scaleMesh(QVector3D(0.006f, 0.006f, 0.006f));

    Qt3DRender::QMesh *meshMiddle = new Qt3DRender::QMesh();
    meshMiddle->setSource(QUrl("qrc:///mesh/middle_part.obj"));
    meshMiddle->setProperty("Vertices", QVariant(16));
    meshMiddle->setProperty("Edges", QVariant(56));
    meshMiddle->setProperty("Faces", QVariant(8));
    MeshModel *subModelMiddle = new MeshModel(rootEntity, meshMiddle);
    subModelMiddle->scaleMesh(QVector3D(0.006f, 0.006f, 0.006f));

    detectorModel->add_subModel(subModelLeft);
    detectorModel->add_subModel(subModelRight);
    detectorModel->add_subModel(subModelMiddle);
    */
    return cylinderModel;
}
