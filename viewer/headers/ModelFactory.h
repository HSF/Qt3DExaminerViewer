#ifndef MODOLFACTORY_H
#define MODOLFACTORY_H
#include "GeneralMeshModel.h"
#include <QEntity>
#include <Qt3DRender/QGeometryRenderer>
#include <GeoModelKernel/GeoTessellatedSolid.h>
#include <Qt3DExtras/QCuboidMesh>
#define TOL 1e-3  // tolerance threshold to consider two meshes as same one

struct Pcon{
    double ZPlane;
    double RMinPlane;
    double RMaxPlane;
};

struct BoxPara{
    float xHalf;
    float yHalf;
    float zHalf;
    bool equal(BoxPara another){
        return std::abs(xHalf - another.xHalf) < TOL
                && std::abs(yHalf - another.yHalf) < TOL
                && std::abs(zHalf - another.zHalf) < TOL;
    }
    Qt3DExtras::QCuboidMesh *mesh = nullptr;
};

struct TubePara{
    float rMin;
    float rMax;
    float zHalf;
    bool equal(TubePara another){
        return std::abs(rMin - another.rMin) < TOL
                && std::abs(rMax - another.rMax) < TOL
                && std::abs(zHalf - another.zHalf) < TOL;
    }
    Qt3DRender::QGeometryRenderer *mesh = nullptr;
};

struct TubsPara{
    float rMin;
    float rMax;
    float zHalf;
    float SPhi;
    float DPhi;
    bool equal(TubsPara another){
        return std::abs(rMin - another.rMin) < TOL
                && std::abs(rMax - another.rMax) < TOL
                && std::abs(zHalf - another.zHalf) < TOL
                && std::abs(SPhi - another.SPhi) < TOL
                && std::abs(DPhi - another.DPhi) < TOL;
    }
    Qt3DRender::QGeometryRenderer *mesh = nullptr;
};

struct PconPara{
    float SPhi;
    float DPhi;
    unsigned int nPlanes;
    Pcon *planes;
    bool equal(PconPara another){
        if(nPlanes != another.nPlanes)
            return false;
        if(std::abs(SPhi - another.SPhi) > TOL || std::abs(DPhi - another.DPhi) > TOL)
            return false;
        for(unsigned int i = 0; i < nPlanes; i++){
            if (planes[i].ZPlane != another.planes[i].ZPlane
                    || std::abs(planes[i].RMinPlane - another.planes[i].RMinPlane) < TOL
                    || std::abs(planes[i].RMaxPlane - another.planes[i].RMaxPlane) < TOL)
                return false;
        }
        return true;
    }
    Qt3DRender::QGeometryRenderer *mesh = nullptr;
};

struct TorusPara{
    float rMin;
    float rMax;
    float rTor;
    float SPhi;
    float DPhi;
    bool equal(TorusPara another){
        return std::abs(rMin - another.rMin) < TOL
                && std::abs(rMax - another.rMax) < TOL
                && std::abs(rTor - another.rTor) < TOL
                && std::abs(SPhi - another.SPhi) < TOL
                && std::abs(DPhi - another.DPhi) < TOL;
    }
    Qt3DRender::QGeometryRenderer *mesh = nullptr;
};

struct TessellatedSolidPara{
    size_t num;
    GeoFacet **faces;
    bool equal(TessellatedSolidPara another){
        if(num != another.num)
            return false;
        for(size_t i = 0; i < num; i++){
           int n = faces[i]->getNumberOfVertices();
           for(int j = 0; j < n; j++){
               if(std::abs((faces[i]->getVertex(j)).x() - (another.faces[i]->getVertex(j)).x()) > TOL)
                   return false;
               if(std::abs((faces[i]->getVertex(j)).y() - (another.faces[i]->getVertex(j)).y()) > TOL)
                   return false;
               if(std::abs((faces[i]->getVertex(j)).z() - (another.faces[i]->getVertex(j)).z()) > TOL)
                   return false;
           }

        }
        return true;
    }
    Qt3DRender::QGeometryRenderer *mesh = nullptr;
};


class ModelFactory{

protected:
    ModelFactory(Qt3DCore::QEntity *rootEntity);
    static ModelFactory* m_singleton;
    Qt3DCore::QEntity *m_rootEntity;
    float m_maxSize=10;
    QVector<BoxPara> m_boxes;
    QVector<TubePara> m_tubes;
    QVector<TubsPara> m_tubses;
    QVector<PconPara> m_pcons;
    QVector<TorusPara> m_toruses;
    QVector<TessellatedSolidPara> m_tess;

public:
    /**
     * Singletons should not be cloneable.
     */
    ModelFactory(ModelFactory &other) = delete;
    /**
     * Singletons should not be assignable.
     */
    void operator=(const ModelFactory &) = delete;
    /**
     * This is the static method that controls the access to the singleton
     * instance. On the first run, it creates a singleton object and places it
     * into the static field. On subsequent runs, it returns the client existing
     * object stored in the static field.
     */
    static ModelFactory *GetInstance(Qt3DCore::QEntity *rootEntity);

    void setMaxSize(float size);
    float MaxSize();
    GeneralMeshModel **build3DText();
    GeneralMeshModel *buildTestVolume();
    GeneralMeshModel *buildCoordinatePlane();
    GeneralMeshModel *buildCoordinateLine();
    GeneralMeshModel *buildTetrahedra();
    GeneralMeshModel *buildBox(double xHalf, double yHalf, double zHalf);
    GeneralMeshModel *buildTube(double rMin, double rMax, double zHalf);
    GeneralMeshModel *buildTubs(double rMin, double rMax, double zHalf, double SPhi, double DPhi);
    GeneralMeshModel *buildPcon(double SPhi, double DPhi, unsigned int nPlanes, Pcon *planes);
    GeneralMeshModel *buildCons(double rMin1, double rMin2, double rMax1, double rMax2, double zHalf, double SPhi, double DPhi);
    GeneralMeshModel *buildTorus(double rMin, double rMax, double rTor, double SPhi, double DPhi);
    GeneralMeshModel *buildTessellatedSolid(size_t num, GeoFacet **faces);
};

#endif // MODOLFACTORY_H
