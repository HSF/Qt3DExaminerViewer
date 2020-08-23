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
        textModel[i]->setPickMode(false);
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
    return cylinderModel;
}

GeneralMeshModel *ModelFactory::buildCoordinatePlane()
{
    Qt3DRender::QGeometryRenderer *meshRenderer = new Qt3DRender::QGeometryRenderer();
    Qt3DRender::QGeometry *geometry = new Qt3DRender::QGeometry(meshRenderer);

    Qt3DRender::QBuffer *vertexDataBuffer = new Qt3DRender::QBuffer();
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
    lineOne->setPickMode(false);
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

    Qt3DRender::QGeometry *geometry = new Qt3DRender::QGeometry();
    geometry->addAttribute(positionAttribute);
    mesh->setGeometry(geometry);
    mesh->setPrimitiveType(QGeometryRenderer::Lines);
    Qt3DExtras::QPerVertexColorMaterial *material = new Qt3DExtras::QPerVertexColorMaterial(m_rootEntity);
    GeneralMeshModel *lineTwo = new GeneralMeshModel(m_rootEntity, mesh, material);
    lineTwo->scaleMesh(m_maxSize * QVector3D(1.3,1.3,1.3));
    lineTwo->setPickMode(false);
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
    tetra->setPickMode(false);
    return tetra;
}

GeneralMeshModel *ModelFactory::buildBox(double xHalf, double yHalf, double zHalf){
    BoxPara box;
    box.xHalf = xHalf;
    box.yHalf = yHalf;
    box.zHalf = zHalf;
    Qt3DExtras::QCuboidMesh *meshBox = nullptr;

    for(BoxPara b : m_boxes){
        if(box.equal(b)){
            meshBox = b.mesh;
            qInfo() << "found same box";
            break;
        }
    }

    if(meshBox == nullptr){
        meshBox = new Qt3DExtras::QCuboidMesh();
        meshBox->setObjectName(QString("GeoBox with:\nxHalf:%1, yHalf:%2, zHalf:%3").arg(xHalf).arg(yHalf).arg(zHalf));
        meshBox->setXExtent(float(2*xHalf));
        meshBox->setYExtent(float(2*yHalf));
        meshBox->setZExtent(float(2*zHalf));
        float maxSize = std::max(std::max(xHalf, yHalf), zHalf);
        setMaxSize(maxSize);
        box.mesh = meshBox;
        m_boxes.push_back(box);
    }

    GeneralMeshModel *cuboidModel = new GeneralMeshModel(m_rootEntity, meshBox);
    cuboidModel->setObjectName("GeoBox");
    return cuboidModel;
}

GeneralMeshModel *ModelFactory::buildTube(double rMin, double rMax, double zHalf){
    TubePara tubePara;
    tubePara.rMin = rMin;
    tubePara.rMax = rMax;
    tubePara.zHalf = zHalf;

    for(TubePara t : m_tubes){
        if(tubePara.equal(t)){
            tubePara.mesh = t.mesh;
            GeneralMeshModel *tube = new GeneralMeshModel(m_rootEntity, tubePara.mesh);
            tube->setObjectName("GeoTube");
            qInfo() << "found same tube";
            return tube;
        }
    }

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
    positionAttribute->setCount(numPerCircle * 4);

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
    normalAttribute->setCount(numPerCircle * 4);

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
    customRenderer->setObjectName(QString("GeoTube with:\nrMin:%1, rMax:%2, zHalf:%3").arg(rMin).arg(rMax).arg(zHalf));
    // save to the list of built tubes
    tubePara.mesh = customRenderer;
    m_tubes.push_back(tubePara);
    GeneralMeshModel *tube = new GeneralMeshModel(m_rootEntity, customRenderer);
    tube->setObjectName("GeoTube");
    return tube;
}
GeneralMeshModel *ModelFactory::buildTubs(double rMin, double rMax, double zHalf, double SPhi, double DPhi){
    TubsPara tubsPara;
    tubsPara.rMin = rMin;
    tubsPara.rMax = rMax;
    tubsPara.zHalf = zHalf;
    tubsPara.SPhi = SPhi;
    tubsPara.DPhi = DPhi;

    for(TubsPara t : m_tubses){
        if(tubsPara.equal(t)){
            GeneralMeshModel *tubs = new GeneralMeshModel(m_rootEntity, t.mesh);
            tubs->setObjectName("GeoTubs");
            qInfo() << "found same tubs";
            return tubs;
        }
    }
    // the revolution shape is a rectangular: we need 4 vertexes per slice
    float maxSize = rMax > zHalf ? rMax : zHalf;
    setMaxSize(maxSize);

    int numPerCircle = 40; // # of slices
    float delta = DPhi / (numPerCircle-1); // length of a slice, in radians
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
    positionAttribute->setCount(numPerCircle * 4);

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
    normalAttribute->setCount(numPerCircle * 4);

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
    customRenderer->setObjectName(QString("GeoTubs with:\nrMin:%1, rMax:%2, \nzHalf:%3, SPhi:%4, DPhi:%5")
                                  .arg(rMin).arg(rMax).arg(zHalf).arg(SPhi).arg(DPhi));
    tubsPara.mesh = customRenderer;
    m_tubses.push_back(tubsPara);
    GeneralMeshModel *tubs = new GeneralMeshModel(m_rootEntity, customRenderer);
    tubs->setObjectName("GeoTubs");
    return tubs;
}

GeneralMeshModel *ModelFactory::buildPcon(double SPhi, double DPhi, unsigned int nPlanes, Pcon *planes){

    PconPara pconPara;
    pconPara.SPhi = SPhi;
    pconPara.DPhi = DPhi;
    pconPara.nPlanes = nPlanes;
    pconPara.planes = planes;
    for(PconPara p : m_pcons){
        if(pconPara.equal(p)){
            GeneralMeshModel *pcon = new GeneralMeshModel(m_rootEntity, p.mesh);
            pcon->setObjectName("GeoPcon");
            qInfo() << "found same pcon";
            return pcon;
        }
    }

    // the revolution shape is a rectangular: we need 4 vertexes per slice
    float maxSize = planes[nPlanes-1].ZPlane - planes[0].ZPlane;
    for(unsigned int i = 0; i < nPlanes; i++){
        if( maxSize < planes[i].RMaxPlane)
            maxSize = planes[i].RMaxPlane;
    }
    setMaxSize(maxSize);

    int numPerCircle = 40; // # of slices
    float delta = DPhi / (numPerCircle-1); // length of a slice, in radians
    float vertex[numPerCircle * 2 * nPlanes * 3]; // 4 vertexes per slice: one top inner, one top outer, one bottom outer, one bottom inner
    int floatPerCircle = numPerCircle * 3;

    // Vertices
    for(int j = 0; j < numPerCircle; j++){
        // 3 coordinates per vertex: we set them by hand as i, i+1, i+2,
        //so we use an offset of j*3 for each ietration over j
        int i = j * 3;
        float u = SPhi + delta * j;
        float cu = qCos(u);
        float su = qSin(u);
        for(unsigned int z = 0; z < nPlanes; z++){
            // z-th layer inner
            vertex[i+2*z*floatPerCircle]   = planes[z].RMinPlane * cu;
            vertex[i+2*z*floatPerCircle+1] = planes[z].RMinPlane * su;
            vertex[i+2*z*floatPerCircle+2] = planes[z].ZPlane;

            // z-th layer outer
            vertex[i+(2*z+1)*floatPerCircle]   = vertex[i+2*z*floatPerCircle] / planes[z].RMinPlane * planes[z].RMaxPlane;
            vertex[i+(2*z+1)*floatPerCircle+1] = vertex[i+2*z*floatPerCircle+1] / planes[z].RMinPlane * planes[z].RMaxPlane;
            vertex[i+(2*z+1)*floatPerCircle+2] = planes[z].ZPlane;
        }
    }
    /*for(unsigned int i = 0; i < numPerCircle * 2 * nPlanes * 3; i+=3){
        qInfo() << i/3 << ") x:" << vertex[i] << "y:" << vertex[i+1] << "z:" << vertex[i+2];
    }*/

    // 4 vertexes per slice; each vertex has 3 'float' coordinates ==> 4 * nSlices * 3 * size(float)
    QByteArray bufferBytes;
    bufferBytes.resize(numPerCircle * 2 * nPlanes * 3 * sizeof(float));
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
    positionAttribute->setCount(numPerCircle * 2 * nPlanes);


    // Inner sides, Normals per vertex
    float normal[numPerCircle * 2 * nPlanes * 3];
    for(int j = 0; j < numPerCircle; j++){
        int i = j * 3; // 3 coordinates per vertex, so we use an offset of j*3 at the start of each iteration
        float u = SPhi + delta * j;
        float cu = qCos(u);
        float su = qSin(u);
        for(unsigned int z = 0; z < nPlanes; z++){
            // z-th layer inner
            normal[i+2*z*floatPerCircle]   = -cu;
            normal[i+2*z*floatPerCircle+1] = -su;
            normal[i+2*z*floatPerCircle+2] = 0;

            // z-th layer outer
            normal[i+(2*z+1)*floatPerCircle]   = cu;
            normal[i+(2*z+1)*floatPerCircle+1] = su;
            normal[i+(2*z+1)*floatPerCircle+2] = 0;
        }
        // specical direction for top layer
        normal[i+2] = -1;
        normal[i+floatPerCircle+2] = -1;
        // specical direction for bottom layer
        normal[i+2*(nPlanes-1)*floatPerCircle+2] = 1;
        normal[i+2*(nPlanes+1)*floatPerCircle+2] = 1;
    }
    /*for(unsigned int i = 0; i < numPerCircle * 2 * nPlanes * 3; i+=3){
        qInfo() << i/3 << ") x:" << normal[i] << "y:" << normal[i+1] << "z:" << normal[i+2];
    }*/

    QByteArray normalBytes;
    normalBytes.resize(numPerCircle * 2 * nPlanes * 3 * sizeof(float));
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
    normalAttribute->setCount(numPerCircle * 2 * nPlanes);

    // faces around each slices plus four triangular faces at two ends
    unsigned int index[( (numPerCircle-1) * ( (nPlanes-1)*4 + 4 ) + 4 *(nPlanes-1) ) * 3];
    int num = (nPlanes - 1) * 4 * 3;
    for(int j = 0; j < numPerCircle-1; j++){
        int i = j * (12+num);
        // top layer upward face
        index[i] = j;
        index[i+1] = j+1;
        index[i+2] = j+numPerCircle;

        index[i+3] = j+1;
        index[i+4] = j+numPerCircle+1;
        index[i+5] = j+numPerCircle;

        // bottom layer downward face
        index[i+6] = (2*nPlanes-2)*numPerCircle+j;
        index[i+7] = (2*nPlanes-1)*numPerCircle+j;
        index[i+8] = (2*nPlanes-2)*numPerCircle+j+1;

        index[i+9] = (2*nPlanes-1)*numPerCircle+j+1;
        index[i+10] = (2*nPlanes-2)*numPerCircle+j+1;
        index[i+11] = (2*nPlanes-1)*numPerCircle+j;

        for(unsigned int z = 0; z < nPlanes-1; z++){
            // z-th layer inner face
            index[i+12*z+12] = j+numPerCircle*2*z;
            index[i+12*z+13] = j+numPerCircle*2*(z+1);
            index[i+12*z+14] = j+numPerCircle*2*z+1;

            index[i+12*z+15] = j+numPerCircle*2*(z+1)+1;
            index[i+12*z+16] = j+numPerCircle*2*z+1;
            index[i+12*z+17] = j+numPerCircle*2*(z+1);

            // z-th layer outer face
            index[i+12*z+18] = j+numPerCircle*(2*z+1);
            index[i+12*z+19] = j+numPerCircle*(2*z+1)+1;
            index[i+12*z+20] = j+numPerCircle*(2*z+3);

            index[i+12*z+21] = j+numPerCircle*(2*z+3)+1;
            index[i+12*z+22] = j+numPerCircle*(2*z+3);
            index[i+12*z+23] = j+numPerCircle*(2*z+1)+1;
        }
    }
    int endFace = (numPerCircle-1) * ( (nPlanes-1)*4 + 4 ) * 3;
    int stride = 12;
    for(unsigned int z = 0; z < nPlanes-1; z++){
       //beginning side face
       index[endFace+stride*z] = numPerCircle*2*z;
       index[endFace+stride*z+1] = numPerCircle*(2*z+1);
       index[endFace+stride*z+2] = numPerCircle*(2*z+2);

       index[endFace+stride*z+3] = numPerCircle*(2*z+3);
       index[endFace+stride*z+4] = numPerCircle*(2*z+2);
       index[endFace+stride*z+5] = numPerCircle*(2*z+1);

       // ending side face
       index[endFace+stride*z+6] = numPerCircle*(2*z+1)-1;
       index[endFace+stride*z+7] = numPerCircle*(2*z+3)-1;
       index[endFace+stride*z+8] = numPerCircle*(2*z+2)-1;

       index[endFace+stride*z+9]  = numPerCircle*(2*z+4)-1;
       index[endFace+stride*z+10] = numPerCircle*(2*z+2)-1;
       index[endFace+stride*z+11] = numPerCircle*(2*z+3)-1;
    }

    /*for(unsigned int i= 0; i < ( (numPerCircle-1) * ( (nPlanes-1)*4 + 4 ) + 4 *(nPlanes-1) ) * 3; i+=3){
        qInfo() << i/3 <<" 1)"<< index[i] << " 2) " << index[i+1] << " 3) "<< index[i+2];
    }*/

    QByteArray indexBytes;
    indexBytes.resize(( (numPerCircle-1) * ( (nPlanes-1)*4 + 4 ) + 4 *(nPlanes-1) ) * 3 * sizeof(quint32));
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
    indexAttribute->setCount(( (numPerCircle-1) * ( (nPlanes-1)*4 + 4 ) + 4 *(nPlanes-1) ) * 3);

    QGeometryRenderer *customRenderer = new QGeometryRenderer;
    Qt3DRender::QGeometry *geometry = new Qt3DRender::QGeometry(customRenderer);
    geometry->addAttribute(positionAttribute);
    geometry->addAttribute(indexAttribute);
    geometry->addAttribute(normalAttribute);
    customRenderer->setGeometry(geometry);
    //customRenderer->setPrimitiveType(QGeometryRenderer::Lines);
    QString info;
    info = QString("GeoPcon with:\n");
    for(unsigned int z = 0; z < nPlanes; z++){
        QString subInfo = QString("Plane #%1: z:%2, rMin:%3, rMax:%4\n").arg(z)
                .arg(planes[z].ZPlane).arg(planes[z].RMinPlane).arg(planes[z].RMaxPlane);
        info += subInfo;
    }
    customRenderer->setObjectName(info);
    pconPara.mesh = customRenderer;
    m_pcons.push_back(pconPara);
    GeneralMeshModel *pcon = new GeneralMeshModel(m_rootEntity, customRenderer);
    pcon->setObjectName("GeoPcon");
    return pcon;
}

GeneralMeshModel *ModelFactory::buildCons(double rMin1, double rMin2, double rMax1, double rMax2, double zHalf, double SPhi, double DPhi){


    Pcon planes[2];
    planes[0].ZPlane = -zHalf;
    planes[0].RMinPlane = rMin1;
    planes[0].RMaxPlane = rMax1;

    planes[1].ZPlane = zHalf;
    planes[1].RMinPlane = rMin2;
    planes[1].RMaxPlane = rMax2;

    PconPara pconPara;
    pconPara.SPhi = SPhi;
    pconPara.DPhi = DPhi;
    pconPara.nPlanes = 2;
    pconPara.planes = planes;

    for(PconPara c : m_pcons){
        if(pconPara.equal(c)){
            GeneralMeshModel *cons = new GeneralMeshModel(m_rootEntity, c.mesh);
            cons->setObjectName("GeoCons");
            qInfo() << "found same cons";
            return cons;
        }
    }

    GeneralMeshModel *cons = buildPcon(SPhi, DPhi, 2, planes);
    cons->m_mesh->setObjectName(QString("GeoCons with:\nrMin1:%1, rMin2:%2, \nrMax1:%3, rMax2:%4, \nzHalf:%5, SPhi:%6, DPhi:%7")
                                .arg(rMin1).arg(rMin2).arg(rMax1).arg(rMax2).arg(zHalf).arg(SPhi).arg(DPhi));
    cons->setObjectName("GeoCons");
    return cons;
}

GeneralMeshModel *ModelFactory::buildTorus(double rMin, double rMax, double rTor, double SPhi, double DPhi){
    TorusPara torusPara;
    torusPara.rMin = rMin;
    torusPara.rMax = rMax;
    torusPara.rTor = rTor;
    torusPara.SPhi = SPhi;
    torusPara.DPhi = DPhi;
    for(TorusPara t : m_toruses){
        if(torusPara.equal(t)){
            GeneralMeshModel *torus = new GeneralMeshModel(m_rootEntity, t.mesh);
            torus->setObjectName("GeoTorus");
            qInfo() << "found same torus";
            return torus;
        }
    }

    float maxSize = rTor + rMax;
    setMaxSize(maxSize);

    int numSlice = 40; // # of slices
    float deltaSlice = 2 * M_PI / numSlice; // length of a slice, in radians
    int numRing = 30;
    float deltaRing = DPhi /(numRing - 1);
    float vertex[2 * numRing * numSlice * 3];
    float normal[2 * numRing * numSlice * 3];

    // Vertices
    for(int j = 0; j < numRing; j++){
        // 3 coordinates per vertex: we set them by hand as i, i+1, i+2,
        //so we use an offset of j*3 for each ietration over j
        float u = SPhi + deltaRing * j;
        float cu = qCos(u);
        float su = qSin(u);
        for(int z = 0; z < numSlice; z++){
            // z-th ring inner
            float v = deltaSlice * z;
            float cv = qCos(v);
            float sv = qSin(v);
            float rInner = rTor + rMin * cv;
            float zInner = rMin * sv;
            vertex[j*numSlice*6 + z*3]   = rInner * cu;
            vertex[j*numSlice*6 + z*3 + 1] = rInner * su;
            vertex[j*numSlice*6 + z*3 + 2] = zInner;
            normal[j*numSlice*6 + z*3] = -cu * cv;
            normal[j*numSlice*6 + z*3 + 1] = -su * cv;
            normal[j*numSlice*6 + z*3 + 2] = -sv;

            // z-th ring outer
            float rOuter = rTor + rMax * cv;
            float zOuter = rMax * sv;
            vertex[j*numSlice*6 + (z+numSlice)*3]   = rOuter * cu;
            vertex[j*numSlice*6 + (z+numSlice)*3 + 1] = rOuter * su;
            vertex[j*numSlice*6 + (z+numSlice)*3 + 2] = zOuter;
            normal[j*numSlice*6 + (z+numSlice)*3] = cu * cv;
            normal[j*numSlice*6 + (z+numSlice)*3 + 1] = su * cv;
            normal[j*numSlice*6 + (z+numSlice)*3 + 2] = sv;
        }
    }

    QByteArray bufferBytes;
    bufferBytes.resize(numSlice * 2 * numRing * 3 * sizeof(float));
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
    positionAttribute->setCount(numSlice * 2 * numRing);

    QByteArray normalBytes;
    normalBytes.resize(numSlice * 2 * numRing * 3 * sizeof(float));
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
    normalAttribute->setCount(numSlice * 2 * numRing);


    unsigned int index[numSlice*(numRing-1)*12 + (numSlice)*12];
    for(int j = 0; j < numRing-1; j++){
        for(int z = 0; z < numSlice; z++){
            // z-th ring inner face
            index[j*numSlice*12 + 12*z] = 2*numSlice*j + z;
            index[j*numSlice*12 + 12*z + 1] = 2*numSlice*j + (z+1) % numSlice;
            index[j*numSlice*12 + 12*z + 2] = 2*numSlice*(j+1) + z;
            index[j*numSlice*12 + 12*z + 3] = 2*numSlice*j + (z+1) % numSlice;
            index[j*numSlice*12 + 12*z + 4] = 2*numSlice*(j+1) + (z+1) % numSlice;
            index[j*numSlice*12 + 12*z + 5] = 2*numSlice*(j+1) + z;

            // z-th ring outer face
            index[j*numSlice*12 + 12*z + 6] = (2*j+1)*numSlice + z;
            index[j*numSlice*12 + 12*z + 7] = (2*j+3)*numSlice + z;
            index[j*numSlice*12 + 12*z + 8] = (2*j+1)*numSlice + (z+1) % numSlice;
            index[j*numSlice*12 + 12*z + 9] = (2*j+1)*numSlice + (z+1) % numSlice;
            index[j*numSlice*12 + 12*z + 10] = (2*j+3)*numSlice + z;
            index[j*numSlice*12 + 12*z + 11] = (2*j+3)*numSlice + (z+1) % numSlice;
        }
    }
    int endFace = numSlice*(numRing-1)*12;
    int stride = 6;
    for(int z = 0; z < numSlice; z++){
       //beginning side face
       index[endFace+stride*z] = z;
       index[endFace+stride*z+1] = numSlice + z;
       index[endFace+stride*z+2] = (z+1)%numSlice;

       index[endFace+stride*z+3] = numSlice + z;
       index[endFace+stride*z+4] = numSlice + (z+1)%numSlice;
       index[endFace+stride*z+5] = (z+1)%numSlice;

       // ending side face
       index[endFace+6*numSlice+stride*z] = numSlice*2*(numRing-1) + z;
       index[endFace+6*numSlice+stride*z+1] = numSlice*2*(numRing-1) + (z+1)%numSlice;
       index[endFace+6*numSlice+stride*z+2] = numSlice*(2*numRing-1) + z;

       index[endFace+6*numSlice+stride*z+3] = numSlice*2*(numRing-1) + (z+1)%numSlice;
       index[endFace+6*numSlice+stride*z+4] = numSlice*(2*numRing-1) + (z+1)%numSlice;
       index[endFace+6*numSlice+stride*z+5] = numSlice*(2*numRing-1) + z;
    }

    QByteArray indexBytes;
    indexBytes.resize((numSlice*(numRing-1)*12 + (numSlice)*12) * sizeof(quint32));
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
    indexAttribute->setCount(numSlice*(numRing-1)*12 + (numSlice)*12);

    QGeometryRenderer *customRenderer = new QGeometryRenderer;
    Qt3DRender::QGeometry *geometry = new Qt3DRender::QGeometry(customRenderer);
    geometry->addAttribute(positionAttribute);
    geometry->addAttribute(normalAttribute);
    geometry->addAttribute(indexAttribute);
    customRenderer->setGeometry(geometry);
    customRenderer->setObjectName(QString("GeoTorus with:\nrMin: %1, rMax: %2, rTor: %3\nSPhi: %4, DPhi: %5")
                                  .arg(rMin).arg(rMax).arg(rTor).arg(SPhi).arg(DPhi));
    torusPara.mesh = customRenderer;
    m_toruses.push_back(torusPara);
    GeneralMeshModel *torus = new GeneralMeshModel(m_rootEntity, customRenderer);
    torus->setObjectName("GeoTorus");
    return torus;
}

GeneralMeshModel *ModelFactory::buildTessellatedSolid(size_t num, GeoFacet **faces){
    TessellatedSolidPara tesPara;
    tesPara.num = num;
    tesPara.faces = faces;
    for(TessellatedSolidPara t : m_tess){
        if(tesPara.equal(t)){
            GeneralMeshModel *tessellatedSolid = new GeneralMeshModel(m_rootEntity, t.mesh);
            tessellatedSolid->setObjectName("GeoTessellatedSolid");
            qInfo() << "found same tesselledatedSolid";
            return tessellatedSolid;
        }
    }

    float vertex[num*4*3]; // max 4 vertices per face, 3 float per vertex
    int sumVert = 0;
    QVector<GeoFacetVertex> vertexVec;
    for (size_t i = 0; i < num; i++){
        int n = faces[i]->getNumberOfVertices();
        for (int j = 0; j < n; j++){
            vertex[sumVert*3+j*3] = faces[i]->getVertex(j).x();
            vertex[sumVert*3+j*3+1] = faces[i]->getVertex(j).y();
            vertex[sumVert*3+j*3+2] = faces[i]->getVertex(j).z();
            vertexVec.push_back(faces[i]->getVertex(j));
        }
        sumVert += n;
    }
    float maxSize = 0;
    for (int i = 0; i < sumVert; i++){
        for (int j = 0; j < sumVert; j++){
            float length = (vertexVec.at(i) - vertexVec.at(j)).norm();
            if (maxSize < length)
                maxSize = length;
        }
    }
    setMaxSize(maxSize);
    QByteArray bufferBytes;
    bufferBytes.resize(sumVert * 3 * sizeof(float));

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
    positionAttribute->setCount(sumVert);


    float normal[num*4*3]; // max 4 vertices per face, 3 float per vertex
    sumVert = 0;
    for (size_t i = 0; i < num; i++){
        GeoFacetVertex faceNormal =
        (faces[i]->getVertex(1) - faces[i]->getVertex(0)).cross(faces[i]->getVertex(2) - faces[i]->getVertex(0));
        int n = faces[i]->getNumberOfVertices();
        for (int j = 0; j < n; j++){
            normal[sumVert*3+j*3] = faceNormal.x();
            normal[sumVert*3+j*3+1] = faceNormal.y();
            normal[sumVert*3+j*3+2] = faceNormal.z();
        }
        sumVert += n;
    }
    QByteArray normalBytes;
    normalBytes.resize(sumVert * 3 * sizeof(float));
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
    normalAttribute->setCount(sumVert);


    unsigned int index[2 * 4 * num]; // max 4 vertex per face, two layer of faces: one outward and one inward
    int indexBase = 0;
    int vertexBase = 0;
    for (size_t i = 0; i < num; i++){
        int n = faces[i]->getNumberOfVertices();
        if(n == 3){
            index[indexBase] = vertexBase;
            index[indexBase+1] = vertexBase+1;
            index[indexBase+2] = vertexBase+2;
            indexBase += 3;
            vertexBase += 3;
        } else if(n == 4){
            // change Quadrangular face to Triangular face
            index[indexBase] = vertexBase;
            index[indexBase+1] = vertexBase+1;
            index[indexBase+2] = vertexBase+2;

            index[indexBase+3] = vertexBase;
            index[indexBase+4] = vertexBase+2;
            index[indexBase+5] = vertexBase+3;
            indexBase += 6;
            vertexBase += 4;
        }
    }
    vertexBase = 0;
    // define second layer of face with inversed vertex sequence.
    for (size_t i = 0; i < num; i++){
        int n = faces[i]->getNumberOfVertices();
        if(n == 3){
            index[indexBase] = vertexBase;
            index[indexBase+1] = vertexBase+2;
            index[indexBase+2] = vertexBase+1;
            indexBase += 3;
            vertexBase += 3;
        } else if(n == 4){
            // change Quadrangular face to Triangular face
            index[indexBase] = vertexBase;
            index[indexBase+1] = vertexBase+2;
            index[indexBase+2] = vertexBase+1;

            index[indexBase+3] = vertexBase;
            index[indexBase+4] = vertexBase+3;
            index[indexBase+5] = vertexBase+2;
            indexBase += 6;
            vertexBase += 4;
        }
    }

    QByteArray indexBytes;
    indexBytes.resize(indexBase * sizeof(quint32));

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
    indexAttribute->setCount(indexBase);

    QGeometryRenderer *customRenderer = new QGeometryRenderer;
    Qt3DRender::QGeometry *geometry = new Qt3DRender::QGeometry(customRenderer);
    geometry->addAttribute(positionAttribute);
    geometry->addAttribute(normalAttribute);
    geometry->addAttribute(indexAttribute);
    customRenderer->setGeometry(geometry);
    customRenderer->setObjectName(QString("GeoTessellatedSolid with:\nnumFacets: %1")
                                  .arg(num));
    tesPara.mesh = customRenderer;
    m_tess.push_back(tesPara);
    GeneralMeshModel *tessellatedSolid = new GeneralMeshModel(m_rootEntity, customRenderer);
    tessellatedSolid->setObjectName("GeoTessellatedSolid");
    return tessellatedSolid;
}
