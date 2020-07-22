// Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

/*
 * geoloader.cpp
 *
 *  Author:     Riccardo Maria BIANCHI @ CERN
 *  Created on: Jul, 2020
 *
 */

// GeoModel includes
#include "GeoModelDBManager/GMDBManager.h"
#include "GeoModelRead/ReadGeoModel.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
// GeoModel shapes
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoTube.h"
#include "GeoModelKernel/GeoTubs.h"
#include "GeoModelKernel/GeoPcon.h"

// C++ includes
#include <iostream>
#include <fstream>
#include <cstdlib> // EXIT_FAILURE


// Units
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits // so we will get, e.g., 'GeoModelKernelUnits::cm'



GeoLoaderQt::GeoLoaderQt(Qt3DCore::QEntity *rootEntity){

}

GMDBManager* GeoLoaderQt::checkPath(QString path){

  // check if DB file exists. If not, return.
  std::ifstream infile(path.toStdString().c_str());
  if ( ! infile.good() ) {
      std::cout << "\n\tERROR!! A '" << path << "' file does not exist!! Please, check the path of the input file before running this program. Exiting...";
      exit(EXIT_FAILURE);
  }
  infile.close();

  // open the DB
  GMDBManager* db = new GMDBManager(path);
  /* Open database */
  if (db->checkIsDBOpen()) {
    std::cout << "OK! Database is open!\n";
  }
  else {
    std::cout << "Database is not open!\n";
    // return;
    throw;
  }
  return db
}


GeoPhysVol *GeoLoaderQt::introWorld(GMDBManager *db){
  /* setup the GeoModel reader */
  GeoModelIO::ReadGeoModel readInGeo = GeoModelIO::ReadGeoModel(db);
  std::cout << "OK! ReadGeoModel is set." << std::endl;
  
  /* build the GeoModel geometry */
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

GeoVPhysVol *GeoLoaderQt::introChild(PVConstLink nodeLink){
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
  }
  return childVol;
}


GeneralMeshModel *GeoLoaderQt::loadCreate(QString path){

  std::cout << "Using this DB file:" << path << std::endl;
  GMDBManager *db = checkPath(path)

  // -- testing the input database
//  std::cout << "Printing the list of all GeoMaterial nodes" << std::endl;
//  db->printAllMaterials();
//  std::cout << "Printing the list of all GeoElement nodes" << std::endl;
//  db->printAllElements();
  GeoPhysVol *world = introWorld(db);


  // --- testing the imported Geometry
  // get number of children volumes
  unsigned int nChil = world->getNChildVols();
  std:: cout << "'world' number of children: " << nChil << std::endl;
  // loop over all children nodes
  std::cout << "Looping over all 'volume' children (i.e., GeoPhysVol and GeoFullPhysVol)..." << std::endl;
  for (unsigned int idx=0; idx<nChil; ++idx) {
    PVConstLink nodeLink = world->getChildVol(idx);

    if ( dynamic_cast<const GeoVPhysVol*>( &(*( nodeLink ))) ) {
      std::cout << "\t" << "the child n. " << idx << " ";
      GeoVPhysVol *childVol = introChild(nodeLink);

      // Get shape type
      const GeoShape* shapeIn = childVolV->getLogVol()->getShape();
      std::cout << "the shape used by the VPhysVol is of type: " << shapeIn->type() << std::endl;
      GeneralMeshModel *model;
      switch (shapeIn->type()){
        case "Box":
          model = createBox(shapeIn);
          break;
        case "Tube":
          model = createTube(shapeIn);
          break;
        case "Tubs":
          model = createTubs(shapeIn);
          break;
        case "Pcon":
          model = createPcon(shapeIn);
          break;
        default:
          std::cout << "Unknown shape";
      }
    }
  }
  std::cout << "Everything done." << std::endl;
  return model;
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

  return ;
}

GeneralMeshModel *GeoLoaderQt::createTube(const GeoShape* shapeIn){

  std::cout << "Tube parameters:\n";
  const GeoTube* shape = dynamic_cast<const GeoTube*>(shapeIn);
  //  Minimum (inner) tube radius.
  const double rMNin = shape->getRMin();
  //  Maximum (outer) tube radius.
  const double rMax = shape->getRMax();
  //  Tube half-length in the z direction.
  const double zHalf = shape->getZHalfLength();
  std::cout << "rMNin: " << rMNin << " , rMax: " << rMax << " , zHalf: " << zHalf << std::endl;
  return ;
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
  return ;
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
  for (int iP=0; iP < nPlanes; ++iP) {
    //  Get the Z Position of the specified plane.
    const double nZP = shape->getZPlane(iP);
    //  Get the RMin of the specified plane.
    const double nRmin = shape->getRMinPlane(iP);
    //  Get the Z Position of the specified plane.
    const double nRmax = shape->getRMaxPlane(iP);
    std::cout << "Plane # " << iP << " -- z: " << nZP << " , rMin: " << nRmin << " , rMax: " << nRmax << std::endl;
  }
  //  True if the polycone has at least two planes.  False otherwise.
  bool isValid = shape->isValid();
  std::cout << "Is this GeoPcon shape valid? " << isValid << std::endl;
  return ;
}

