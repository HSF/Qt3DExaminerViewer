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
#include <QtMath>

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

GeneralMeshModel *ModelFactory::buildCoordinatePlane()
{
    Qt3DRender::QGeometryRenderer *meshRenderer = new Qt3DRender::QGeometryRenderer();
    Qt3DRender::QGeometry *geometry = new Qt3DRender::QGeometry(meshRenderer);

    Qt3DRender::QBuffer *vertexDataBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, geometry);
    //Qt3DRender::QBuffer *indexDataBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer, geometry);

    float netX1 = 1.0f, netX0 = 0.0f, netZ1 = 1.0f, netZ0 = 0.0f, netY = 0.0f;
    float netMajorStep = 0.1;
    int lineSize = 4;
    int hLineSize = ((qAbs(netX1 - netX0) / netMajorStep) + 1) * lineSize * 3;
    int vLineSize = ((qAbs(netZ1 - netZ0) / netMajorStep) + 1) * lineSize * 3;
    int vertexNum = hLineSize + vLineSize;

    float* vertexRawData = new float[vertexNum];
    int idx = 0;
    QColor majorColor = QColor(220,220,220);
    QColor minorColor = QColor(243,243,243);
    for(float x = netX0; x < netX1 + netMajorStep; x += netMajorStep)
    {
        vertexRawData[idx++] = x; vertexRawData[idx++] = netY; vertexRawData[idx++] = netZ0;
        vertexRawData[idx++] = majorColor.redF(); vertexRawData[idx++] = majorColor.greenF(); vertexRawData[idx++] = majorColor.blueF();
        vertexRawData[idx++] = x; vertexRawData[idx++] = netY; vertexRawData[idx++] = netZ1;
        vertexRawData[idx++] = minorColor.redF(); vertexRawData[idx++] = minorColor.greenF(); vertexRawData[idx++] = minorColor.blueF();
    }

    for(float z = netZ0; z < netZ1 + netMajorStep; z += netMajorStep)
    {
        vertexRawData[idx++] = netX0; vertexRawData[idx++] = netY; vertexRawData[idx++] = z;
        vertexRawData[idx++] = majorColor.redF(); vertexRawData[idx++] = majorColor.greenF(); vertexRawData[idx++] = majorColor.blueF();
        vertexRawData[idx++] = netX1; vertexRawData[idx++] = netY; vertexRawData[idx++] = z;
        vertexRawData[idx++] = minorColor.redF(); vertexRawData[idx++] = minorColor.greenF(); vertexRawData[idx++] = minorColor.blueF();
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
    positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setByteOffset(0);
    positionAttribute->setByteStride(stride);
    positionAttribute->setCount(vertexNum);
    positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());


    Qt3DRender::QAttribute *colorAttribute = new Qt3DRender::QAttribute();
    colorAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    colorAttribute->setBuffer(vertexDataBuffer);
    colorAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    colorAttribute->setVertexSize(3);
    colorAttribute->setByteOffset(3 * sizeof(float));
    colorAttribute->setByteStride(stride);
    colorAttribute->setCount(vertexNum);
    colorAttribute->setName(Qt3DRender::QAttribute::defaultColorAttributeName());

    geometry->addAttribute(positionAttribute);
    geometry->addAttribute(colorAttribute);

   // meshRenderer->setInstanceCount(1);
   // meshRenderer->setIndexOffset(0);
    //meshRenderer->setFirstInstance(0);
    meshRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
    meshRenderer->setGeometry(geometry);
    //meshRenderer->setVertexCount(vertexNum / 2);

    Qt3DExtras::QPerVertexColorMaterial *material = new Qt3DExtras::QPerVertexColorMaterial(m_rootEntity);
    GeneralMeshModel *lineOne = new GeneralMeshModel(m_rootEntity, meshRenderer, material);
    lineOne->translateMesh(QVector3D(-50, 0, -50));
    lineOne->scaleMesh(QVector3D(100,100,100));
    lineOne->enablePickAll(false);
    return lineOne;
}


GeneralMeshModel *ModelFactory::buildCoordinateLine()
{
    Qt3DRender::QGeometryRenderer *mesh = new Qt3DRender::QGeometryRenderer();

    float vertex_array[3 * 6];

    int ix = 0;
    vertex_array[ix++] = 0.0f;
    vertex_array[ix++] = -10.0f;
    vertex_array[ix++] = 0.0f;

    vertex_array[ix++] = 0.0f;
    vertex_array[ix++] = 10.0f;
    vertex_array[ix++] = 0.0f;



    vertex_array[ix++] = -10.0f;
    vertex_array[ix++] = 0.0f;
    vertex_array[ix++] = 0.0f;

    vertex_array[ix++] = 10.0f;
    vertex_array[ix++] = 0.0f;
    vertex_array[ix++] = 0.0f;




    vertex_array[ix++] = 0.0f;
    vertex_array[ix++] = 0.0f;
    vertex_array[ix++] = -10.0f;

    vertex_array[ix++] = 0.0f;
    vertex_array[ix++] = 0.0f;
    vertex_array[ix++] = 10.0f;

    Qt3DRender::QGeometry *geometry = new Qt3DRender::QGeometry();

    QByteArray bufferBytes;
    const int num_vertices = 6;
    const quint32 elementsize = 3;
    const quint32 stride = elementsize * sizeof(float);
    bufferBytes.resize(stride * num_vertices);

    memcpy(bufferBytes.data(), reinterpret_cast<const char*>(vertex_array), bufferBytes.size());

    Qt3DRender::QBuffer *buf = (new Qt3DRender::QBuffer());
    buf->setData(bufferBytes);
    Qt3DRender::QAttribute *positionAttribute = new Qt3DRender::QAttribute();
    positionAttribute->setName(QAttribute::defaultPositionAttributeName());
    positionAttribute->setAttributeType(QAttribute::VertexAttribute);
    positionAttribute->setVertexBaseType(QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setBuffer(buf);
    positionAttribute->setByteOffset(0);
    positionAttribute->setByteStride(stride);
    positionAttribute->setCount(18);

    geometry->addAttribute(positionAttribute);

    mesh->setGeometry(geometry);
    mesh->setPrimitiveType(QGeometryRenderer::Lines);

    Qt3DExtras::QPerVertexColorMaterial *material = new Qt3DExtras::QPerVertexColorMaterial(m_rootEntity);
    GeneralMeshModel *lineTwo = new GeneralMeshModel(m_rootEntity, mesh, material);
    lineTwo->scaleMesh(QVector3D(10,10,10));
    lineTwo->enablePickAll(false);
    return lineTwo;
}

GeneralMeshModel *ModelFactory::buildTetrahedra(){

    // Material
    //Qt3DExtras::QPhongMaterial *material = new Qt3DExtras::QPhongMaterial(rootEntity);
    //material->setDiffuse(QColor(QRgb(0xbeb32b)));
    Qt3DRender::QMaterial *material = new Qt3DExtras::QPerVertexColorMaterial(m_rootEntity);

    // Custom Mesh (TetraHedron)
    QGeometryRenderer *customMeshRenderer = new QGeometryRenderer;
    QGeometry *customGeometry = new QGeometry(customMeshRenderer);

    // vec3 for position
    // vec3 for colors
    // vec3 for normals

    /*          2
               /|\
              / | \
             / /3\ \
             0/___\ 1
    */

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

    // 4 distinct vertices
    QByteArray vertexBufferData;
    vertexBufferData.resize(4 * (3 + 3 + 3) * sizeof(float));
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


    QBuffer *vertexDataBuffer = new QBuffer(QBuffer::VertexBuffer, customGeometry);
    QBuffer *indexDataBuffer = new QBuffer(QBuffer::IndexBuffer, customGeometry);

    vertexDataBuffer->setData(vertexBufferData);
    indexDataBuffer->setData(indexBufferData);

    // Attributes
    QAttribute *positionAttribute = new QAttribute();
    positionAttribute->setName(QAttribute::defaultPositionAttributeName());
    positionAttribute->setAttributeType(QAttribute::VertexAttribute);
    positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    positionAttribute->setBuffer(vertexDataBuffer);
    positionAttribute->setVertexSize(3);
    positionAttribute->setByteOffset(0);
    positionAttribute->setByteStride(9 * sizeof(float));
    positionAttribute->setCount(36);

    QAttribute *normalAttribute = new QAttribute();
    normalAttribute->setBuffer(vertexDataBuffer);
    normalAttribute->setName(QAttribute::defaultNormalAttributeName());
    normalAttribute->setAttributeType(QAttribute::VertexAttribute);
    normalAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    normalAttribute->setVertexSize(3);
    normalAttribute->setByteOffset(3 * sizeof(float));
    normalAttribute->setByteStride(9 * sizeof(float));
    normalAttribute->setCount(36);


    QAttribute *colorAttribute = new QAttribute();
    colorAttribute->setBuffer(vertexDataBuffer);
    colorAttribute->setName(QAttribute::defaultColorAttributeName());
    colorAttribute->setAttributeType(QAttribute::VertexAttribute);
    colorAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    colorAttribute->setVertexSize(3);
    colorAttribute->setByteOffset(6 * sizeof(float));
    colorAttribute->setByteStride(9 * sizeof(float));
    colorAttribute->setCount(36);


    QAttribute *indexAttribute = new QAttribute();
    indexAttribute->setBuffer(indexDataBuffer);
    indexAttribute->setAttributeType(QAttribute::IndexAttribute);
    indexAttribute->setVertexBaseType(Qt3DRender::QAttribute::UnsignedShort);
    indexAttribute->setVertexSize(3);
    indexAttribute->setByteOffset(0);
    indexAttribute->setByteStride(3);
    indexAttribute->setCount(12);

    customGeometry->addAttribute(positionAttribute);
    customGeometry->addAttribute(normalAttribute);
    customGeometry->addAttribute(colorAttribute);
    customGeometry->addAttribute(indexAttribute);

    //customMeshRenderer->setGeometry(customGeometry);

    GeneralMeshModel *tetra = new GeneralMeshModel(m_rootEntity, customMeshRenderer);
    tetra->translateMesh(QVector3D(0, 50, 0));
    tetra->scaleMesh(QVector3D(20,20,20));
    tetra->enablePickAll(false);
    return tetra;
}



GeneralMeshModel *ModelFactory::buildTube(double rMin, double rMax, double zHalf){
    
    // the revolution shape is a rectangular: we need 4 vertexes per slice

    int numPerCircle = 50; // # of slices
    float delta = 2 * M_PI / numPerCircle; // length of a slice, in radians
    float vertex[numPerCircle * 4 * 3]; // 4 vertexes per slice: one top inner, one top outer, one bottom outer, one bottom inner
    int floatPerCircle = numPerCircle * 3;
    
    // Vertices
    for(int j = 0; j < numPerCircle; j++){
        
        int i = j * 3; // 3 coordinates per vertex: we set them by hand as i, i+1, i+2, so we use an offset of j*3 for each ietration over j
 
        // top inner
        vertex[i]   = rMin * qCos(delta*j);
        vertex[i+1] = rMin * qSin(delta*j);           
        vertex[i+2] = zHalf;
        
        // top outer
        vertex[i+floatPerCircle]   = vertex[i]/rMin*rMax;
        vertex[i+1+floatPerCircle] = vertex[i+1]/rMin*rMax; 
        vertex[i+2+floatPerCircle] = zHalf;

        // bottom inner
        vertex[i+2*floatPerCircle]   = vertex[i];              
        vertex[i+1+2*floatPerCircle] = vertex[i+1];         
        vertex[i+2+2*floatPerCircle] = -zHalf;

        // bottom outer
        vertex[i+3*floatPerCircle]   = vertex[i+floatPerCircle];     
        vertex[i+1+3*floatPerCircle] = vertex[i+floatPerCircle+1];
        vertex[i+2+3*floatPerCircle] = -zHalf;
    }
    /*for(int i = 0; i < numPerCircle*4*3; i+=3){
        qInfo() << i/3 << ") x:" << vertex[i] << "y:" << vertex[i+1] << "z:" << vertex[i+2];
    }*/

    // 4 vertexes per slice; each vertex has 3 'float' coordinates ==> 4 * nSlices * 3 * size(float) 
    QByteArray bufferBytes;
    bufferBytes.resize(4 * numPerCircle * 3 * sizeof(float));

    memcpy(bufferBytes.data(), reinterpret_cast<const char*>(vertex), bufferBytes.size());

    Qt3DRender::QBuffer *buf = (new Qt3DRender::QBuffer());
    buf->setData(bufferBytes);
    Qt3DRender::QAttribute *positionAttribute = new Qt3DRender::QAttribute();
    positionAttribute->setName(QAttribute::defaultPositionAttributeName());
    positionAttribute->setAttributeType(QAttribute::VertexAttribute);
    positionAttribute->setVertexBaseType(QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setBuffer(buf);
    positionAttribute->setByteOffset(0);
    positionAttribute->setByteStride(3 * sizeof(float));
    positionAttribute->setCount(numPerCircle * 3 * 4);


    // Inner sides, Normals per vertex
    float normal[numPerCircle * 4 * 3];
    for(int j = 0; j < numPerCircle; j++){
        
        int i = j * 3; // 3 coordinates per vertex, so we use an offset of j*3 at the start of each iteration
	
        // top inner
        normal[i]   = -qCos(delta*j);         
        normal[i+1] = -qSin(delta*j);           
        normal[i+2] = 1;

        // top outer
        normal[i+floatPerCircle]   = -normal[i];
        normal[i+floatPerCircle+1] = -normal[i+1];
        normal[i+floatPerCircle+2] = 1;

        // bottom inner
        normal[i+2*floatPerCircle]   = normal[i];              
        normal[i+2*floatPerCircle+1] = normal[i+1];
        normal[i+2*floatPerCircle+2] = -1;
        
        // bottom outer
        normal[i+3*floatPerCircle]   = normal[i+floatPerCircle];
        normal[i+3*floatPerCircle+1] = normal[i+floatPerCircle+1];
        normal[i+3*floatPerCircle+2] = -1;
    }
    /*for(int i = 0; i < numPerCircle*4*3; i+=3){
        qInfo() << i/3 << ") x:" << normal[i] << "y:" << normal[i+1] << "z:" << normal[i+2];
    }*/
    QByteArray normalBytes;
    normalBytes.resize(4 * numPerCircle * 3 * sizeof(float));
    memcpy(normalBytes.data(), reinterpret_cast<const char*>(normal), normalBytes.size());
    Qt3DRender::QBuffer *normalBuf = (new Qt3DRender::QBuffer());
    normalBuf->setData(normalBytes);
    
    Qt3DRender::QAttribute *normalAttribute = new Qt3DRender::QAttribute();
    normalAttribute->setBuffer(normalBuf);
    normalAttribute->setName(QAttribute::defaultNormalAttributeName());
    normalAttribute->setAttributeType(QAttribute::VertexAttribute);
    normalAttribute->setVertexBaseType(QAttribute::Float);
    normalAttribute->setVertexSize(3);
    normalAttribute->setByteOffset(0);
    normalAttribute->setByteStride(3 * sizeof(float));
    normalAttribute->setCount(numPerCircle * 3 * 4);


    unsigned int index[numPerCircle * 8 * 3];
    int num = numPerCircle * 6;
    for(int j = 0; j < numPerCircle; j++){
        int i = j * 6;
        index[i] = j;                            
        index[i+2] = (j+1)%numPerCircle;       
        index[i+1] = j+numPerCircle;
        index[i+3] = (j+1)%numPerCircle;         
        index[i+4] = j+numPerCircle;
        index[i+5] = (j+1)%numPerCircle + numPerCircle;

        index[i+num] = j+numPerCircle;                       
        index[i+num+2] = (j+1)%numPerCircle + numPerCircle;
        index[i+num+1] = j+3*numPerCircle;
        index[i+num+3] = (j+1)%numPerCircle + numPerCircle;  
        index[i+num+4] = j+3*numPerCircle;
        index[i+num+5] = (j+1)%numPerCircle + 3*numPerCircle;

        index[i+2*num] = j+3*numPerCircle;       
        index[i+2*num+2] = (j+1)%numPerCircle + 3*numPerCircle;
        index[i+2*num+1] = j+2*numPerCircle;
        index[i+2*num+3] = (j+1)%numPerCircle + 3*numPerCircle;   
        index[i+2*num+4] = j+2*numPerCircle;
        index[i+2*num+5] = (j+1)%numPerCircle + 2*numPerCircle;

        index[i+3*num] = j+2*numPerCircle;       
        index[i+3*num+2] = (j+1)%numPerCircle + 2*numPerCircle;
        index[i+3*num+1] = j;
        index[i+3*num+3] = (j+1)%numPerCircle + 2*numPerCircle;   
        index[i+3*num+4] = j;
        index[i+3*num+5] = (j+1)%numPerCircle;
    }
    /*for(int i= 0; i < numPerCircle * 8 * 3; i+=3){
        qInfo() << i/3 <<" 1)"<< index[i] << " 2) " << index[i+1] << " 3) "<< index[i+2];
    }*/
    QByteArray indexBytes;
    indexBytes.resize(8 * numPerCircle * 3 * sizeof(quint32));

    memcpy(indexBytes.data(), reinterpret_cast<const char*>(index), indexBytes.size());
    Qt3DRender::QBuffer *indexBuffer(new QBuffer());
    indexBuffer->setData(indexBytes);

    QAttribute *indexAttribute = new QAttribute();
    indexAttribute->setAttributeType(QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexBuffer);
    indexAttribute->setVertexBaseType(QAttribute::UnsignedInt);
    indexAttribute->setVertexSize(3);
    indexAttribute->setByteOffset(0);
    indexAttribute->setByteStride(3 * sizeof(unsigned int));
    indexAttribute->setCount(8 * 3 * numPerCircle);
    
    QGeometryRenderer *customRenderer = new QGeometryRenderer;
    Qt3DRender::QGeometry *geometry = new Qt3DRender::QGeometry(customRenderer);
    geometry->addAttribute(positionAttribute);
    geometry->addAttribute(indexAttribute);
    geometry->addAttribute(normalAttribute);
    customRenderer->setGeometry(geometry);
    //customRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
    
    GeneralMeshModel *tube = new GeneralMeshModel(m_rootEntity, customRenderer);
    return tube;
}
GeneralMeshModel *ModelFactory::buildTubs(double rMin, double rMax, double zHalf, double SPhi, double DPhi){
    // TODO: ask the diff between tubs and tube, define Tubs
    return nullptr;
}
GeneralMeshModel *ModelFactory::buildPcon(double SPhi, double DPhi, unsigned int nPlanes, Pcon *planes){
    // TODO: make sure what is Pcon
    return nullptr;
}
