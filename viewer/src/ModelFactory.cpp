#include "../headers/ModelFactory.h"
#include <QColor>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QExtrudedTextMesh>
#include <Qt3DRender/QBuffer>
#include <Qt3DExtras/QPerVertexColorMaterial>
#include <Qt3DRender/QMesh>
#include <QGeometry>
#include <QAttribute>

using namespace Qt3DRender;

ModelFactory::ModelFactory(Qt3DCore::QEntity *rootEntity): m_rootEntity(rootEntity){
}

GeneralMeshModel **ModelFactory::build3DText(){

    QColor qColor = QColor(147, 147, 147, 147);
    Qt3DExtras::QExtrudedTextMesh *textMesh1 = new Qt3DExtras::QExtrudedTextMesh();
    textMesh1->setObjectName("Z+");
    textMesh1->setText("Z+");
    GeneralMeshModel *textModel1 = new GeneralMeshModel(m_rootEntity, textMesh1);
    textModel1->translateMesh(QVector3D(0.0f, 0.0f, 100.0f));
    textModel1->scaleMesh(QVector3D(10.0f, 10.0f, 2.0f));
    textModel1->enablePickAll(false);
    textModel1->setColor(qColor);

    Qt3DExtras::QExtrudedTextMesh *textMesh2 = new Qt3DExtras::QExtrudedTextMesh();
    textMesh2->setObjectName("Z-");
    textMesh2->setText("Z-");
    GeneralMeshModel *textModel2 = new GeneralMeshModel(m_rootEntity, textMesh2);
    textModel2->translateMesh(QVector3D(0.0f, 0.0f, -100.0f));
    textModel2->scaleMesh(QVector3D(10.0f, 10.0f, 2.0f));
    textModel2->enablePickAll(false);
    textModel2->setColor(qColor);


    Qt3DExtras::QExtrudedTextMesh *textMesh3 = new Qt3DExtras::QExtrudedTextMesh();
    textMesh3->setObjectName("Y+");
    textMesh3->setText("Y+");
    GeneralMeshModel *textModel3 = new GeneralMeshModel(m_rootEntity, textMesh3);
    textModel3->translateMesh(QVector3D(0.0f, 100.0f, 0.0f));
    textModel3->scaleMesh(QVector3D(10.0f, 10.0f, 2.0f));
    textModel3->enablePickAll(false);
    textModel3->setColor(qColor);

    Qt3DExtras::QExtrudedTextMesh *textMesh4 = new Qt3DExtras::QExtrudedTextMesh();
    textMesh4->setObjectName("Y-");
    textMesh4->setText("Y-");
    GeneralMeshModel *textModel4 = new GeneralMeshModel(m_rootEntity, textMesh4);
    textModel4->translateMesh(QVector3D(0.0f, -100.0f, 0.0f));
    textModel4->scaleMesh(QVector3D(10.0f, 10.0f, 2.0f));
    textModel4->enablePickAll(false);
    textModel4->setColor(qColor);


    Qt3DExtras::QExtrudedTextMesh *textMesh5 = new Qt3DExtras::QExtrudedTextMesh();
    textMesh5->setObjectName("X+");
    textMesh5->setText("X+");
    GeneralMeshModel *textModel5 = new GeneralMeshModel(m_rootEntity, textMesh5);
    textModel5->translateMesh(QVector3D(100.0f, 0.0f, 0.0f));
    textModel5->scaleMesh(QVector3D(10.0f, 10.0f, 2.0f));
    textModel5->enablePickAll(false);
    textModel5->setColor(qColor);

    Qt3DExtras::QExtrudedTextMesh *textMesh6 = new Qt3DExtras::QExtrudedTextMesh();
    textMesh6->setObjectName("X-");
    textMesh6->setText("X-");
    GeneralMeshModel *textModel6 = new GeneralMeshModel(m_rootEntity, textMesh6);
    textModel6->translateMesh(QVector3D(-100.0f, 0.0f, 0.0f));
    textModel6->scaleMesh(QVector3D(10.0f, 10.0f, 2.0f));
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

GeneralMeshModel *ModelFactory::buildTestVolume(){
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

GeneralMeshModel *ModelFactory::buildLineOne()
{
    Qt3DRender::QGeometryRenderer *meshRenderer = new Qt3DRender::QGeometryRenderer();
    Qt3DRender::QGeometry *geometry = new Qt3DRender::QGeometry(meshRenderer);

    Qt3DRender::QBuffer *vertexDataBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, geometry);
    Qt3DRender::QBuffer *indexDataBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer, geometry);

    int netX1 = 1, netX0 = 0, netZ1 = 1, netZ0 = 0, netY = 1;
    float netMajorStep = 0.1;
    int lineSize = 4;
    int hLineSize = ((qAbs(netX1 - netX0) / netMajorStep) + 1) * lineSize * 3;
    int vLineSize = ((qAbs(netZ1 - netZ0) / netMajorStep) + 1) * lineSize * 3;
    int vertexNum = hLineSize + vLineSize;

    float* vertexRawData = new float[vertexNum];
    int idx = 0;
    QColor majorColor = QColor(220,220,220);
    for(float x = netX0; x <= netX1; x += netMajorStep)
    {
        vertexRawData[idx++] = x; vertexRawData[idx++] = netY; vertexRawData[idx++] = netZ0;
        vertexRawData[idx++] = majorColor.redF(); vertexRawData[idx++] = majorColor.greenF(); vertexRawData[idx++] = majorColor.blueF();
        vertexRawData[idx++] = x; vertexRawData[idx++] = netY; vertexRawData[idx++] = netZ1;
        vertexRawData[idx++] = majorColor.redF(); vertexRawData[idx++] = majorColor.greenF(); vertexRawData[idx++] = majorColor.blueF();
    }

    for(float z = netZ0; z <= netZ1; z += netMajorStep)
    {
        vertexRawData[idx++] = netX0; vertexRawData[idx++] = netY; vertexRawData[idx++] = z;
        vertexRawData[idx++] = majorColor.redF(); vertexRawData[idx++] = majorColor.greenF(); vertexRawData[idx++] = majorColor.blueF();
        vertexRawData[idx++] = netX1; vertexRawData[idx++] = netY; vertexRawData[idx++] = z;
        vertexRawData[idx++] = majorColor.redF(); vertexRawData[idx++] = majorColor.greenF(); vertexRawData[idx++] = majorColor.blueF();
    }

    QByteArray ba;
    int bufferSize = vertexNum * sizeof(float);
    ba.resize(bufferSize);
    memcpy(ba.data(), reinterpret_cast<const char*>(vertexRawData), bufferSize);
    vertexDataBuffer->setData(ba);

    int stride = 6 * sizeof(float);

    // Attributes
    Qt3DRender::QAttribute *positionAttribute = new Qt3DRender::QAttribute();
    positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(vertexDataBuffer);
    //positionAttribute->setDataType(Qt3DRender::QAttribute::Float);
    //positionAttribute->setDataSize(3);
    positionAttribute->setByteOffset(0);
    positionAttribute->setByteStride(stride);
    positionAttribute->setCount(vertexNum / 2);
    positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());


    Qt3DRender::QAttribute *colorAttribute = new Qt3DRender::QAttribute();
    colorAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    colorAttribute->setBuffer(vertexDataBuffer);
    //colorAttribute->setDataType(Qt3DRender::QAttribute::Float);
    //colorAttribute->setDataSize(3);
    colorAttribute->setByteOffset(3 * sizeof(float));
    colorAttribute->setByteStride(stride);
    colorAttribute->setCount(vertexNum / 2);
    colorAttribute->setName(Qt3DRender::QAttribute::defaultColorAttributeName());

    geometry->addAttribute(positionAttribute);
    geometry->addAttribute(colorAttribute);

    //meshRenderer->setInstanceCount(1);
    //meshRenderer->setIndexOffset(0);
    //meshRenderer->setFirstInstance(0);
    //meshRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
    meshRenderer->setGeometry(geometry);
    //meshRenderer->setVertexCount(vertexNum / 2);
    GeneralMeshModel *lineOne = new GeneralMeshModel(m_rootEntity, meshRenderer);
    lineOne->translateMesh(QVector3D(0, 500, 0));
    lineOne->scaleMesh(QVector3D(1000,1000,1000));
    return lineOne;
}


GeneralMeshModel *ModelFactory::buildLineTwo()
{
    Qt3DRender::QGeometryRenderer *mesh = new Qt3DRender::QGeometryRenderer();

    float vertex_array[3 * 2];

    int ix = 0;
    vertex_array[ix++] = 0.0f;
    vertex_array[ix++] = 0.0f;
    vertex_array[ix++] = 0.0f;


    vertex_array[ix++] = 100.0f;
    vertex_array[ix++] = 100.0f;
    vertex_array[ix++] = 2323.0f;

    int index_array[2];

    ix = 0;
    index_array[ix++] = 0;
    index_array[ix++] = 1;

    Qt3DRender::QGeometry *geometry = new Qt3DRender::QGeometry(mesh);

    QByteArray bufferBytes;
    const int num_vertices = 2;
    const quint32 elementsize = 3;
    const quint32 stride = elementsize * sizeof(float);
    bufferBytes.resize(stride * num_vertices);

    memcpy(bufferBytes.data(), reinterpret_cast<const char*>(vertex_array), bufferBytes.size());

    Qt3DRender::QBuffer *buf = (new Qt3DRender::QBuffer());
    buf->setData(bufferBytes);
    Qt3DRender::QAttribute *positionAttribute = new Qt3DRender::QAttribute();
    positionAttribute->setAttributeType(QAttribute::VertexAttribute);
    positionAttribute->setBuffer(buf);
    positionAttribute->setByteOffset(0);
    positionAttribute->setByteStride(3 * sizeof(float));
    positionAttribute->setCount(2);
    geometry->addAttribute(positionAttribute);


    const int num_indices = 2;
    QByteArray indexBytes;
    indexBytes.resize(num_indices * sizeof(quint32));

    //reinterpret_cast<const char*>(index_array)
    memcpy(indexBytes.data(), reinterpret_cast<const char*>(index_array), indexBytes.size());
    Qt3DRender::QBuffer *indexBuffer(new QBuffer());
    indexBuffer->setData(indexBytes);

    QAttribute *indexAttribute = new QAttribute();
    indexAttribute->setAttributeType(QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexBuffer);
    indexAttribute->setByteOffset(0);
    indexAttribute->setByteStride(2 * sizeof(int));
    indexAttribute->setCount(1);
    geometry->addAttribute(indexAttribute);

    mesh->setGeometry(geometry);
    mesh->setPrimitiveType(QGeometryRenderer::Lines);

    GeneralMeshModel *lineTwo = new GeneralMeshModel(m_rootEntity, mesh);
    lineTwo->translateMesh(QVector3D(0, 500, 0));
    lineTwo->scaleMesh(QVector3D(1000,1000,1000));
    return lineTwo;
}

GeneralMeshModel *ModelFactory::buildTetrahedra(){

    // Material
    //QMaterial *material = new Qt3DExtras::QPhongMaterial(rootEntity);
    Qt3DRender::QMaterial *material = new Qt3DExtras::QPerVertexColorMaterial(m_rootEntity);

    // The Entity container for the custom mesh
    //Qt3DCore::QEntity *customMeshEntity = new Qt3DCore::QEntity(m_rootEntity);

    // Custom Mesh (TetraHedron)
    QGeometryRenderer *customMeshRenderer = new QGeometryRenderer;
    QGeometry *customGeometry = new QGeometry(customMeshRenderer);

    QBuffer *vertexDataBuffer = new QBuffer(QBuffer::VertexBuffer, customGeometry);
    QBuffer *indexDataBuffer = new QBuffer(QBuffer::IndexBuffer, customGeometry);

    // vec3 for position
    // vec3 for colors
    // vec3 for normals

    /*          2
               /|\
              / | \
             / /3\ \
             0/___\ 1
    */

    // 4 distinct vertices
    QByteArray vertexBufferData;
    vertexBufferData.resize(4 * (3 + 3 + 3) * sizeof(float));

    // Vertices
    QVector3D v0(-1.0f, 0.0f, -1.0f);
    QVector3D v1(1.0f, 0.0f, -1.0f);
    QVector3D v2(0.0f, 1.0f, 0.0f);
    QVector3D v3(0.0f, 0.0f, 1.0f);

    // Faces Normals
    QVector3D n023 = QVector3D::normal(v0, v2, v3);
    QVector3D n012 = QVector3D::normal(v0, v1, v2);
    QVector3D n310 = QVector3D::normal(v3, v1, v0);
    QVector3D n132 = QVector3D::normal(v1, v3, v2);

    // Vector Normals
    QVector3D n0 = QVector3D(n023 + n012 + n310).normalized();
    QVector3D n1 = QVector3D(n132 + n012 + n310).normalized();
    QVector3D n2 = QVector3D(n132 + n012 + n023).normalized();
    QVector3D n3 = QVector3D(n132 + n310 + n023).normalized();

    // Colors
    QVector3D red(1.0f, 0.0f, 0.0f);
    QVector3D green(0.0f, 1.0f, 0.0f);
    QVector3D blue(0.0f, 0.0f, 1.0f);
    QVector3D white(1.0f, 1.0f, 1.0f);
    QVector<QVector3D> vertices = QVector<QVector3D>()
            << v0 << n0 << red
            << v1 << n1 << blue
            << v2 << n2 << green
            << v3 << n3 << white;

    float *rawVertexArray = reinterpret_cast<float *>(vertexBufferData.data());
    int idx = 0;

    Q_FOREACH (const QVector3D &v, vertices) {
        rawVertexArray[idx++] = v.x();
        rawVertexArray[idx++] = v.y();
        rawVertexArray[idx++] = v.z();
    }

    // Indices (12)
    QByteArray indexBufferData;
    indexBufferData.resize(4 * 3 * sizeof(ushort));
    ushort *rawIndexArray = reinterpret_cast<ushort *>(indexBufferData.data());

    // Front
    rawIndexArray[0] = 0;
    rawIndexArray[1] = 1;
    rawIndexArray[2] = 2;
    // Bottom
    rawIndexArray[3] = 3;
    rawIndexArray[4] = 1;
    rawIndexArray[5] = 0;
    // Left
    rawIndexArray[6] = 0;
    rawIndexArray[7] = 2;
    rawIndexArray[8] = 3;
    // Right
    rawIndexArray[9] = 1;
    rawIndexArray[10] = 3;
    rawIndexArray[11] = 2;

    vertexDataBuffer->setData(vertexBufferData);
    indexDataBuffer->setData(indexBufferData);

    // Attributes
    QAttribute *positionAttribute = new QAttribute();
    positionAttribute->setAttributeType(QAttribute::VertexAttribute);
    positionAttribute->setBuffer(vertexDataBuffer);
    positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setByteOffset(0);
    positionAttribute->setByteStride(9 * sizeof(float));
    positionAttribute->setCount(4);
    positionAttribute->setName(QAttribute::defaultPositionAttributeName());

    QAttribute *normalAttribute = new QAttribute();
    normalAttribute->setAttributeType(QAttribute::VertexAttribute);
    normalAttribute->setBuffer(vertexDataBuffer);
    normalAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    normalAttribute->setVertexSize(3);
    normalAttribute->setByteOffset(3 * sizeof(float));
    normalAttribute->setByteStride(9 * sizeof(float));
    normalAttribute->setCount(4);
    normalAttribute->setName(QAttribute::defaultNormalAttributeName());

    QAttribute *colorAttribute = new QAttribute();
    colorAttribute->setAttributeType(QAttribute::VertexAttribute);
    colorAttribute->setBuffer(vertexDataBuffer);
    colorAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    colorAttribute->setVertexSize(3);
    colorAttribute->setByteOffset(6 * sizeof(float));
    colorAttribute->setByteStride(9 * sizeof(float));
    colorAttribute->setCount(4);
    colorAttribute->setName(QAttribute::defaultColorAttributeName());

    QAttribute *indexAttribute = new QAttribute();
    indexAttribute->setAttributeType(QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexDataBuffer);
    indexAttribute->setVertexBaseType(Qt3DRender::QAttribute::UnsignedShort);
    indexAttribute->setVertexSize(1);
    indexAttribute->setByteOffset(0);
    indexAttribute->setByteStride(0);
    indexAttribute->setCount(12);

    customGeometry->addAttribute(positionAttribute);
    customGeometry->addAttribute(normalAttribute);
    customGeometry->addAttribute(colorAttribute);
    customGeometry->addAttribute(indexAttribute);

    customMeshRenderer->setGeometry(customGeometry);

    GeneralMeshModel *tetra = new GeneralMeshModel(m_rootEntity, customMeshRenderer, material);
    tetra->translateMesh(QVector3D(0, 50, 0));
    tetra->scaleMesh(QVector3D(20,20,20));

    return tetra;
}
