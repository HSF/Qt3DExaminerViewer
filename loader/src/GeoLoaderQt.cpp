// Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

/*
 * geoloader.cpp
 *
 *  Author:     Riccardo Maria BIANCHI @ CERN
 *  Created on: Jul, 2020
 *
 */
#include "loader/headers/GeoLoaderQt.h"
#include <Qt3DExtras/QCuboidMesh>
// C++ includes
#include <iostream>
#include <fstream>
#include <cstdlib> // EXIT_FAILURE
#include <string>
// Units
#include <GeoModelKernel/Units.h>
#define SYSTEM_OF_UNITS GeoModelKernelUnits // so we will get, e.g., 'GeoModelKernelUnits::cm'

GeoLoaderQt::GeoLoaderQt(Qt3DCore::QEntity *rootEntity){
    m_builder = ModelFactory::GetInstance(rootEntity);
}

GMDBManager* GeoLoaderQt::checkPath(QString path){
  std::string pathStd = path.toStdString();

  std::size_t botDirPos = pathStd.find_last_of("/");
  // get file
  std::string file = pathStd.substr(botDirPos, path.length());
  std::cout << "reading current file: " << file << std::endl;

  // check if DB file exists. If not, return.
  std::ifstream infile(path.toStdString().c_str());
  if ( ! infile.good() ) {
      std::cout << "\n\tERROR!! A '" << pathStd << "' file does not exist!! Please, check the path of the input file before running this program. Exiting...";
      return nullptr;
  }
  infile.close();

  // open the DB
  GMDBManager* db = new GMDBManager(pathStd);
  // Open database
  if (db->checkIsDBOpen()) {
    std::cout << "OK! Database is open!\n";
  }
  else {
    std::cout << "Database is not open!\n";
    return nullptr;
  }
  return db;
}


GeoPhysVol *GeoLoaderQt::introWorld(GMDBManager *db){
  // setup the GeoModel reader
  GeoModelIO::ReadGeoModel readInGeo = GeoModelIO::ReadGeoModel(db);
  std::cout << "OK! ReadGeoModel is set." << std::endl;
  
  // build the GeoModel geometry
  GeoPhysVol* dbPhys = readInGeo.buildGeoModel(); // builds the whole GeoModel tree in memory
  std::cout << "ReadGeoModel::buildGeoModel() done." << std::endl;

  // create the world volume container and
  // get the 'world' volume, i.e. the root volume of the GeoModel tree
  std::cout << "Getting the 'world' GeoPhysVol, i.e. the root volume of the GeoModel tree" << std::endl;
  GeoPhysVol* world = createTheWorld(dbPhys);
  std::cout << "Getting the GeoLogVol used by the 'world' volume" << std::endl;
  const GeoLogVol* logVol = world->getLogVol();
  std::cout << "'world' GeoLogVol name: " << logVol->getName() << std::endl;
  std::cout << "'world' GeoMaterial name: " << logVol->getMaterial()->getName() << std::endl;
  return world;
} 

inline QMatrix4x4 toQMatrix(GeoTrf::Transform3D tr){
    QMatrix4x4 mtx;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++)
            mtx(i,j) = tr.matrix()(i, j);
    }
    return mtx;
}

void GeoLoaderQt::loadChildren(GeneralMeshModel *container, const GeoVPhysVol *parent, QMatrix4x4 parentTransform){
    unsigned int nChil = parent->getNChildVols();
    for (unsigned int idx = 0; idx < nChil; idx++){
        PVConstLink nodeLink = parent->getChildVol(idx);
        if ( dynamic_cast<const GeoVPhysVol*>( &(*( nodeLink ))) ) {
          std::cout << "\n" << "the child n. " << idx << " ";
          const GeoVPhysVol *childVolV = &(*( nodeLink ));
          if ( dynamic_cast<const GeoPhysVol*>(childVolV) ) {
            const GeoPhysVol *childVol = dynamic_cast<const GeoPhysVol*>(childVolV);
            std::cout << "is a GeoPhysVol, whose GeoLogVol name is: " << childVol->getLogVol()->getName();
            std::cout<< " and it has  "<<childVol->getNChildVols()<<" child volumes" << std::endl;
          }
          else if ( dynamic_cast<const GeoFullPhysVol*>(childVolV) ) {
            const GeoFullPhysVol *childVol = dynamic_cast<const GeoFullPhysVol*>(childVolV);
            std::cout << "is a GeoFullPhysVol, whose GeoLogVol name is: " << childVol->getLogVol()->getName();
            std::cout<< " and it has  "<<childVol->getNChildVols()<<" child volumes" << std::endl;
          } else {
              std::cout << "unknown object! " << std::endl;
              continue;
          }
          // Get shape type
          const GeoShape* shapeIn = childVolV->getLogVol()->getShape();
          std::cout << "the shape used by the VPhysVol is of type: " << shapeIn->type() << std::endl;
          GeneralMeshModel *model = nullptr;
          if(shapeIn->type() == "Box")
              model = createBox(shapeIn);
          else if(shapeIn->type() == "Tube")
              model = createTube(shapeIn);
          else if(shapeIn->type() == "Tubs")
              model = createTubs(shapeIn);
          else if(shapeIn->type() == "Pcon")
              model = createPcon(shapeIn);
          else if(shapeIn->type() == "Cons")
              model = createCons(shapeIn);
          else if(shapeIn->type() == "Torus")
              model = createTorus(shapeIn);
          else if(shapeIn->type() == "TessellatedSolid")
              model = createTessellatedSolid(shapeIn);
          else if(shapeIn->type() == "Trd")
              model = createTrd(shapeIn);
          else{
              std::cerr << "Unsupported shape: " << shapeIn->type();
          }
          if(model != nullptr){
              std::cout << "name: " << childVolV->getLogVol()->getName();
              std::cout << " material name: " << childVolV->getLogVol()->getMaterial()->getName()
                        << " material density: " << childVolV->getLogVol()->getMaterial()->getDensity()
                        << " material elementSize: " << childVolV->getLogVol()->getMaterial()->getNumElements()
                        << std::endl;
              QMatrix4x4 childTransform = parentTransform * toQMatrix(parent->getXToChildVol(idx));
              model->setTransformMatrix(childTransform);
              model->setVolume(childVolV);
              container->addSubModel(model);
              loadChildren(model, childVolV, childTransform);
          }
        }
    }
}

GeneralMeshModel *GeoLoaderQt::loadFromDB(QString path){

  std::cout << "Using this DB file:" << path.toStdString() << std::endl;

  GMDBManager *db = checkPath(path);
  if(db == nullptr)
      return nullptr;
  GeoPhysVol *world = introWorld(db);
  // --- testing the imported Geometry
  // get number of children volumes
  unsigned int nChil = world->getNChildVols();
  std:: cout << "'world' number of children: " << nChil << std::endl;
  GeneralMeshModel *container = new GeneralMeshModel(nullptr, nullptr);
  container->setPickMode(false);
  container->setObjectName("world");
  // loop over all children nodes
  std::cout << "Looping over all 'volume' children (i.e., GeoPhysVol and GeoFullPhysVol)..." << std::endl;
  // unit transfrom matrix, we resale the orginal geometry to its 0.01
  QMatrix4x4 transform = QMatrix4x4(0.01f,0.0f, 0.0f, 0.0f,
                                    0.0f, 0.01f,0.0f, 0.0f,
                                    0.0f, 0.0f, 0.01f,0.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f);
  loadChildren(container, world, transform);
  std::cout << "Everything done." << container->subModelCount() << std::endl;
  return container;
}

GeoPhysVol* GeoLoaderQt::createTheWorld(GeoPhysVol* world){
  if (world == nullptr){
  	//-----------------------------------------------------------------------------------//
    // Define the materials that we shall use.                                              //
    // ----------------------------------------------------------------------------------//

    // Define the units
    #define gr   SYSTEM_OF_UNITS::gram
    #define mole SYSTEM_OF_UNITS::mole
    #define cm3  SYSTEM_OF_UNITS::cm3

    // Define the chemical elements
    GeoElement*  Nitrogen = new GeoElement ("Nitrogen" ,"N"  ,  7.0 ,  14.0067 *gr/mole);
    GeoElement*  Oxygen   = new GeoElement ("Oxygen"   ,"O"  ,  8.0 ,  15.9995 *gr/mole);
    GeoElement*  Argon    = new GeoElement ("Argon"    ,"Ar" , 18.0 ,  39.948  *gr/mole);
    GeoElement*  Hydrogen = new GeoElement ("Hydrogen" ,"H"  ,  1.0 ,  1.00797 *gr/mole);

    // Define the materials
    double densityOfAir=0.001214 *gr/cm3;
    GeoMaterial *air = new GeoMaterial("Air", densityOfAir);
    air->add(Nitrogen  , 0.7494);
    air->add(Oxygen, 0.2369);
    air->add(Argon, 0.0129);
    air->add(Hydrogen, 0.0008);
    air->lock();

  	const GeoBox* worldBox = new GeoBox(1000*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm, 1000*SYSTEM_OF_UNITS::cm);
  	const GeoLogVol* worldLog = new GeoLogVol("WorldLog", worldBox, air);
  	world = new GeoPhysVol(worldLog);
  }
  return world;
}


GeneralMeshModel *GeoLoaderQt::createBox(const GeoShape* shapeIn){
  std::cout << "Box parameters:\n";
  const GeoBox* shape = dynamic_cast<const GeoBox*>(shapeIn);
  //  Half-length in the x direction.
  const double xHalf = shape->getXHalfLength();
  //  Half-length in the y direction.
  const double yHalf = shape->getYHalfLength();
  //  Half-length in the z direction.
  const double zHalf = shape->getZHalfLength();
  std::cout << "xHalf: " << xHalf << " , yHalf: " << yHalf << " , zHalf: " << zHalf << std::endl;
  std::cout << "Volume: " << shape->volume() << std::endl;
  return m_builder->buildBox(xHalf, yHalf, zHalf);
}

GeneralMeshModel *GeoLoaderQt::createTube(const GeoShape* shapeIn){

  std::cout << "Tube parameters:\n";
  const GeoTube* shape = dynamic_cast<const GeoTube*>(shapeIn);
  //  Minimum (inner) tube radius.
  const double rMin = shape->getRMin();
  //  Maximum (outer) tube radius.
  const double rMax = shape->getRMax();
  //  Tube half-length in the z direction.
  const double zHalf = shape->getZHalfLength();
  std::cout << "rMin: " << rMin << " , rMax: " << rMax << " , zHalf: " << zHalf << std::endl;
  std::cout << "Volume: " << shape->volume() << std::endl;
  if(rMin < TOL)
      return m_builder->buildTube(TOL, rMax, zHalf);
  else
      return m_builder->buildTube(rMin, rMax, zHalf);
}

GeneralMeshModel *GeoLoaderQt::createTubs(const GeoShape* shapeIn){
  std::cout << "Tubs parameters:\n";
  const GeoTubs* shape = dynamic_cast<const GeoTubs*>(shapeIn);
  //  Minimum (inner) tube section radius.
  const double rMin = shape->getRMin();
  //  Maximum (outer) tube section radius.
  const double rMax = shape->getRMax();
  //  Tube section half-length in the z direction.
  const double zHalf = shape->getZHalfLength();
  //  Starting angle of the tube section in radians.
  const double SPhi = shape->getSPhi();
  //  Delta angle of the tube section in radians.
  const double DPhi = shape->getDPhi();
  std::cout << "rMin: " << rMin << " , rMax: " << rMax << " , zHalf: " << zHalf << " , SPhi: " << SPhi << " , DPhi: " << DPhi << std::endl;
  std::cout << "Volume: " << shape->volume() << std::endl;
  if(std::abs(DPhi - 2 * M_PI) < TOL){
      if(rMin < TOL)
          return m_builder->buildTube(TOL, rMax, zHalf);
      else
          return m_builder->buildTube(rMin, rMax, zHalf);
  }
  else{
       // to prevent shape from disappering, this is a temporary fix for rendering bug
      if(rMin < TOL)
          return m_builder->buildTubs(TOL, rMax, zHalf, SPhi, DPhi);
      else
          return m_builder->buildTubs(rMin, rMax, zHalf, SPhi, DPhi);
  }
}

GeneralMeshModel *GeoLoaderQt::createPcon(const GeoShape* shapeIn){
  std::cout << "Pcon parameters:\n";
  const GeoPcon* shape = dynamic_cast<const GeoPcon*>(shapeIn);
  //  Starting angle of the segment in radians.
  const double SPhi = shape->getSPhi();
  //  Delta angle of the segment in radians.
  const double DPhi = shape->getDPhi();
  //  Returns the number of planes that have been created for the polycone.
  unsigned int nPlanes = shape->getNPlanes();
  std::cout << "SPhi: " << SPhi << " , DPhi: " << DPhi << " , nPlanes: " << nPlanes << std::endl;
  Pcon planes[nPlanes];
  for (uint iP=0; iP < nPlanes; ++iP) {
    //  Get the Z Position of the specified plane.
    const double nZP = shape->getZPlane(iP);
    planes[iP].ZPlane = nZP;
    //  Get the RMin of the specified plane.
    double nRmin = shape->getRMinPlane(iP);
    if(nRmin < TOL)
        nRmin = TOL;
    planes[iP].RMinPlane = nRmin;
    //  Get the RMax of the specified plane.
    const double nRmax = shape->getRMaxPlane(iP);
    planes[iP].RMaxPlane = nRmax;
    std::cout << "Plane # " << iP << " -- z: " << nZP << " , rMin: " << nRmin << " , rMax: " << nRmax << std::endl;
  }
  std::cout << "Volume: " << shape->volume() << std::endl;
  //  True if the polycone has at least two planes.  False otherwise.
  bool isValid = shape->isValid();
  std::cout << "Is this GeoPcon shape valid? " << isValid << std::endl;
  if(isValid)
    return m_builder->buildPcon(SPhi, DPhi, nPlanes, planes);
  else
    return nullptr;
}

GeneralMeshModel *GeoLoaderQt::createCons(const GeoShape* shapeIn){
  std::cout << "Cons parameters:\n";
  const GeoCons* shape = dynamic_cast<const GeoCons*>(shapeIn);
  //  Starting angle of the segment in radians.
  const double SPhi = shape->getSPhi();
  //  Delta angle of the segment in radians.
  const double DPhi = shape->getDPhi();
  //  Half-length in the z direction.
  const double zHalf = shape->getDZ();
  //  Returns the min radius of specified annulus at -zHalf position
  const double rMin1 = shape->getRMin1();
  //  Returns the min radius of specified annulus at zHalf position
  const double rMin2 = shape->getRMin2();
  //  Returns the max radius of specified annulus at -zHalf position
  const double rMax1 = shape->getRMax1();
  //  Returns the max radius of specified annulus at zHalf position
  const double rMax2 = shape->getRMax2();
  std::cout << "rMin1: " << rMin1 << " , rMin2: " << rMin2 << " , rMax1: " << rMax1  << " , rMax2: " << rMax2
          << " , zHalf: " << zHalf << ", SPhi: " << SPhi << " , DPhi: " << DPhi  << std::endl;
  std::cout << "Volume: " << shape->volume() << std::endl;
  if(rMin1 < TOL && rMin2 > TOL)
      return m_builder->buildCons(0, rMin2, rMax1, rMax2, zHalf, SPhi, DPhi);
  else if(rMin1 > TOL && rMin2 < TOL)
      return m_builder->buildCons(rMin1, TOL, rMax1, rMax2, zHalf, SPhi, DPhi);
  else if(rMin1 < TOL && rMin2 < TOL)
      return m_builder->buildCons(TOL, TOL, rMax1, rMax2, zHalf, SPhi, DPhi);
  else return m_builder->buildCons(rMin1, rMin2, rMax1, rMax2, zHalf, SPhi, DPhi);
}

GeneralMeshModel *GeoLoaderQt::createTorus(const GeoShape* shapeIn){
  std::cout << "Torus parameters:\n";
  const GeoTorus* shape = dynamic_cast<const GeoTorus*>(shapeIn);
  //  Starting angle of the segment in radians.
  const double SPhi = shape->getSPhi();
  //  Delta angle of the segment in radians.
  const double DPhi = shape->getDPhi();
  //  Returns the radius of torus
  const double rTor = shape->getRTor();
  //  Returns the min radius of annulus
  const double rMin = shape->getRMin();
  //  Returns the max radius of annulus
  const double rMax = shape->getRMax();
  std::cout << "rMin: " << rMin << " , rMax: " << rMax
          << " , rTor: " << rTor << ", SPhi: " << SPhi << " , DPhi: " << DPhi  << std::endl;
  std::cout << "Volume: " << shape->volume() << std::endl;
  return m_builder->buildTorus(rMin, rMax, rTor, SPhi, DPhi);
}

GeneralMeshModel *GeoLoaderQt::createTessellatedSolid(const GeoShape *shapeIn){
  std::cout << "Cons parameters:\n";
  const GeoTessellatedSolid* shape = dynamic_cast<const GeoTessellatedSolid*>(shapeIn);
  //  get the number of facets of shape.
  const size_t num = shape->getNumberOfFacets();
  GeoFacet *faces[num];
  for (size_t i = 0; i < num; i++){
      // get each facet
      faces[i] = shape->getFacet(i);
  }
  std::cout << "number of facets: " << num  << std::endl;
  std::cout << "Volume: " << shape->volume() << std::endl;
  return m_builder->buildTessellatedSolid(num, faces);
}

GeneralMeshModel *GeoLoaderQt::createTrd(const GeoShape *shapeIn){
    std::cout << "Trd parameters:\n";
    const GeoTrd* shape = dynamic_cast<const GeoTrd*>(shapeIn);
    //  Half-length along z axis
    const double zHalf = shape->getZHalfLength();
    //  Half-length along x at the surface positioned at -dz
    const double xHalf1 = shape->getXHalfLength1();
    //  Half-length along x at the surface positioned at +dz
    const double xHalf2 = shape->getXHalfLength2();
    //  Half-length along y at the surface positioned at -dz
    const double yHalf1 = shape->getYHalfLength1();
    //  Half-length along y at the surface positioned at +dz
    const double yHalf2 = shape->getYHalfLength2();
    std::cout << "zHalf: " << zHalf << " , xHalf1: " << xHalf1
              << " , xHalf2: " << xHalf2 << ", yHalf1: " << yHalf1 << " , yHalf2: " << yHalf2  << std::endl;
    std::cout << "Volume: " << shape->volume() << std::endl;
    return nullptr;
}
