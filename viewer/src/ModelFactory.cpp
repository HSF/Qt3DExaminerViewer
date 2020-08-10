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
ModelFactory *ModelFactory::m_singleton = nullptr;
ModelFactory *ModelFactory::GetInstance(Qt3DCore::QEntity *rootEntity){
    if(m_singleton == nullptr)
        m_singleton = new ModelFactory(rootEntity);
    return m_singleton;
}

ModelFactory::ModelFactory(Qt3DCore::QEntity *rootEntity): m_rootEntity(rootEntity){
}

void ModelFactory::setMaxSize(float size){
    if(size > m_maxSize)
        m_maxSize = size;
}

float ModelFactory::MaxSize(){
    return m_maxSize;
}

GeneralMeshModel **ModelFactory::build3DText(){

    QColor qColor = QColor(147, 147, 147, 147);
    GeneralMeshModel **textModel = new GeneralMeshModel*[6];
    QString text[6] = {"Z+", "Z-", "Y+", "Y-", "X-", "X+"};
    QVector3D position[6] = {QVector3D(0.0f, 0.0f, 1.3f), QVector3D(0.0f, 0.0f, -1.3f),
                             QVector3D(0.0f, 1.3f, 0.0f), QVector3D(0.0f, -1.3f, 0.0f),
                             QVector3D(1.3f, 0.0f, 0.0f),QVector3D(-1.3f, 0.0f, 0.0f)};

    for(int i = 0; i < 6; i++){
        Qt3DExtras::QExtrudedTextMesh *textMesh = new Qt3DExtras::QExtrudedTextMesh();
        textMesh->setObjectName(text[i]);
        textMesh->setText(text[i]);
        textModel[i] = new GeneralMeshModel(m_rootEntity, textMesh);
        textModel[i]->translateMesh(m_maxSize * position[i]);
        textModel[i]->scaleMesh(m_maxSize * QVector3D(0.1f, 0.1f, 0.02f));
        textModel[i]->enablePickAll(false);
        textModel[i]->setColor(qColor);
    }
    return textModel;
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
    lineOne->translateMesh(m_maxSize * QVector3D(-1, 0, -1));
    lineOne->scaleMesh(m_maxSize * QVector3D(2, 2, 2));
    lineOne->enablePickAll(false);
    return lineOne;
}


GeneralMeshModel *ModelFactory::buildCoordinateLine()
{
    Qt3DRender::QGeometryRenderer *mesh = new Qt3DRender::QGeometryRenderer();

    // initilise with 0.0f
    float vertex_array[3 * 6];

    int ix = 0;
    vertex_array[ix++] = 0.0f;
    vertex_array[ix++] = -1.0f;
    vertex_array[ix++] = 0.0f;

    vertex_array[ix++] = 0.0f;
    vertex_array[ix++] = 1.0f;
    vertex_array[ix++] = 0.0f;



    vertex_array[ix++] = -1.0f;
    vertex_array[ix++] = 0.0f;
    vertex_array[ix++] = 0.0f;

    vertex_array[ix++] = 1.0f;
    vertex_array[ix++] = 0.0f;
    vertex_array[ix++] = 0.0f;


    vertex_array[ix++] = 0.0f;
    vertex_array[ix++] = 0.0f;
    vertex_array[ix++] = -1.0f;

    vertex_array[ix++] = 0.0f;
    vertex_array[ix++] = 0.0f;
    vertex_array[ix++] = 1.0f;

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
    lineTwo->scaleMesh(m_maxSize * QVector3D(1.3,1.3,1.3));
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

    QBuffer *vertexDataBuffer = new QBuffer();
    QBuffer *indexDataBuffer = new QBuffer();

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

    customMeshRenderer->setGeometry(customGeometry);

    GeneralMeshModel *tetra = new GeneralMeshModel(m_rootEntity, customMeshRenderer, material);
    tetra->translateMesh(QVector3D(0, 50, 0));
    tetra->scaleMesh(QVector3D(20,20,20));
    tetra->enablePickAll(false);
    return tetra;
}

GeneralMeshModel *ModelFactory::buildBox(double xHalf, double yHalf, double zHalf){
    Qt3DExtras::QCuboidMesh *meshBox = new Qt3DExtras::QCuboidMesh();
    meshBox->setObjectName(QString("GeoBox with xHalf:%1, yHalf:%2, zHalf:%3").arg(xHalf).arg(yHalf).arg(zHalf));
    meshBox->setXExtent(float(2*xHalf));
    meshBox->setYExtent(float(2*yHalf));
    meshBox->setZExtent(float(2*zHalf));
    GeneralMeshModel *cuboidModel = new GeneralMeshModel(m_rootEntity, meshBox);
    float maxSize = std::max(std::max(xHalf, yHalf), zHalf);
    setMaxSize(maxSize);
    return cuboidModel;
}

GeneralMeshModel *ModelFactory::buildTube(double rMin, double rMax, double zHalf){
    
    // the revolution shape is a rectangular: we need 4 vertexes per slice
    float maxSize = rMax > zHalf ? rMax : zHalf;
    setMaxSize(maxSize);
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
    customRenderer->setObjectName(QString("GeoTube with rMin:%1, rMax:%2, zHalf:%3").arg(rMin).arg(rMax).arg(zHalf));
    GeneralMeshModel *tube = new GeneralMeshModel(m_rootEntity, customRenderer);
    return tube;
}
GeneralMeshModel *ModelFactory::buildTubs(double rMin, double rMax, double zHalf, double SPhi, double DPhi){
    // TODO: ask the diff between tubs and tube, define Tubs
    // the revolution shape is a rectangular: we need 4 vertexes per slice
    float maxSize = rMax > zHalf ? rMax : zHalf;
    setMaxSize(maxSize);
    int numPerCircle = 40; // # of slices
    float delta = DPhi / numPerCircle; // length of a slice, in radians
    float vertex[numPerCircle * 4 * 3]; // 4 vertexes per slice: one top inner, one top outer, one bottom outer, one bottom inner
    int floatPerCircle = numPerCircle * 3;

    // Vertices
    for(int j = 0; j < numPerCircle; j++){

        int i = j * 3; // 3 coordinates per vertex: we set them by hand as i, i+1, i+2, so we use an offset of j*3 for each ietration over j

        // top inner
        vertex[i]   = rMin * qCos(SPhi + delta*j);
        vertex[i+1] = rMin * qSin(SPhi + delta*j);
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

    //for(int i = 0; i < numPerCircle*4*3; i+=3){
    //    qInfo() << i/3 << ") x:" << vertex[i] << "y:" << vertex[i+1] << "z:" << vertex[i+2];
    //}

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
        normal[i]   = -qCos(SPhi + delta*j);
        normal[i+1] = -qSin(SPhi + delta*j);
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
    //for(int i = 0; i < numPerCircle*4*3; i+=3){
    //    qInfo() << i/3 << ") x:" << normal[i] << "y:" << normal[i+1] << "z:" << normal[i+2];
    //}
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

    // faces around each slices plus four triangular faces at two ends
    unsigned int index[((numPerCircle-1) * 8 + 4) * 3];
    int num = (numPerCircle - 1) * 6;
    for(int j = 0; j < numPerCircle-1; j++){
        int i = j * 6;
        index[i] = j;
        index[i+1] = j+numPerCircle;
        index[i+2] = (j+1)%numPerCircle;
        index[i+3] = (j+1)%numPerCircle;
        index[i+4] = j+numPerCircle;
        index[i+5] = (j+1)%numPerCircle + numPerCircle;

        index[i+num] = j+numPerCircle;
        index[i+num+1] = j+3*numPerCircle;
        index[i+num+2] = (j+1)%numPerCircle + numPerCircle;
        index[i+num+3] = (j+1)%numPerCircle + numPerCircle;
        index[i+num+4] = j+3*numPerCircle;
        index[i+num+5] = (j+1)%numPerCircle + 3*numPerCircle;

        index[i+2*num] = j+3*numPerCircle;
        index[i+2*num+1] = j+2*numPerCircle;
        index[i+2*num+2] = (j+1)%numPerCircle + 3*numPerCircle;
        index[i+2*num+3] = (j+1)%numPerCircle + 3*numPerCircle;
        index[i+2*num+4] = j+2*numPerCircle;
        index[i+2*num+5] = (j+1)%numPerCircle + 2*numPerCircle;

        index[i+3*num] = j+2*numPerCircle;
        index[i+3*num+1] = j;
        index[i+3*num+2] = (j+1)%numPerCircle + 2*numPerCircle;
        index[i+3*num+3] = (j+1)%numPerCircle + 2*numPerCircle;
        index[i+3*num+4] = j;
        index[i+3*num+5] = (j+1)%numPerCircle;
    }
    int endFace = (numPerCircle-1) * 8 * 3;
    index[endFace] = 0;
    index[endFace+1] = 3 * numPerCircle;
    index[endFace+2] = numPerCircle;

    index[endFace+3] = 0;
    index[endFace+4] = 2 * numPerCircle;
    index[endFace+5] = 3 * numPerCircle;

    index[endFace+6] = numPerCircle - 1;
    index[endFace+7] = 2 * numPerCircle - 1;
    index[endFace+8] = 4 * numPerCircle - 1;

    index[endFace+9] = numPerCircle - 1;
    index[endFace+10] = 4 * numPerCircle - 1;
    index[endFace+11] = 3 * numPerCircle - 1;

    //for(int i= 0; i < numPerCircle * 8 * 3; i+=3){
    //    qInfo() << i/3 <<" 1)"<< index[i] << " 2) " << index[i+1] << " 3) "<< index[i+2];
    //}

    QByteArray indexBytes;
    indexBytes.resize((8 * (numPerCircle-1) + 4) * 3 * sizeof(quint32));

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
    indexAttribute->setCount((8 * (numPerCircle-1) + 4) * 3);

    QGeometryRenderer *customRenderer = new QGeometryRenderer;
    Qt3DRender::QGeometry *geometry = new Qt3DRender::QGeometry(customRenderer);
    geometry->addAttribute(positionAttribute);
    geometry->addAttribute(indexAttribute);
    geometry->addAttribute(normalAttribute);
    customRenderer->setGeometry(geometry);
    customRenderer->setObjectName(QString("GeoTubs with rMin:%1, rMax:%2, zHalf:%3, SPhi:%4, DPhi:%5")
                                  .arg(rMin).arg(rMax).arg(zHalf).arg(SPhi).arg(DPhi));
    GeneralMeshModel *tubs = new GeneralMeshModel(m_rootEntity, customRenderer);
    return tubs;
}

GeneralMeshModel *ModelFactory::buildPcon(double SPhi, double DPhi, unsigned int nPlanes, Pcon *planes){
    // TODO: make sure what is Pcon
    return nullptr;
}
