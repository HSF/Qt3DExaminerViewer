/*
 * GeoModelReadIn.cpp
 *
 *  Created on: May 20, 2016
 *      Author: riccardo.maria.bianchi@cern.ch
 *
 * major updates:
 *  - Feb 2019, R.M.Bianchi
 *  - Mar 2020, R.M.Bianchi
 *  - Mar 2020, boudreau
 *  - May 2020, R.M.Bianchi
 */


// local includes
#include "GeoModelRead/ReadGeoModel.h"

// TFPersistification includes
#include "TFPersistification/TransFunctionInterpreter.h"


// GeoModelKernel
#include "GeoModelKernel/GeoTransform.h"
#include "GeoModelKernel/GeoAlignableTransform.h"
#include "GeoModelKernel/GeoSerialTransformer.h"
#include "GeoModelKernel/GeoSerialDenominator.h"
#include "GeoModelKernel/GeoMaterial.h"
#include "GeoModelKernel/GeoElement.h"
#include "GeoModelKernel/GeoNameTag.h"
#include "GeoModelKernel/GeoLogVol.h"
#include "GeoModelKernel/GeoVPhysVol.h"
#include "GeoModelKernel/GeoPhysVol.h"
#include "GeoModelKernel/GeoFullPhysVol.h"
#include "GeoModelKernel/GeoGraphNode.h"

// GeoModel shapes
#include "GeoModelKernel/GeoShape.h"
#include "GeoModelKernel/GeoBox.h"
#include "GeoModelKernel/GeoCons.h"
#include "GeoModelKernel/GeoPara.h"
#include "GeoModelKernel/GeoPcon.h"
#include "GeoModelKernel/GeoPgon.h"
#include "GeoModelKernel/GeoSimplePolygonBrep.h"
#include "GeoModelKernel/GeoTessellatedSolid.h"
#include "GeoModelKernel/GeoGenericTrap.h"
#include "GeoModelKernel/GeoTrap.h"
#include "GeoModelKernel/GeoTrd.h"
#include "GeoModelKernel/GeoTube.h"
#include "GeoModelKernel/GeoTubs.h"
#include "GeoModelKernel/GeoTorus.h"
#include "GeoModelKernel/GeoShapeIntersection.h"
#include "GeoModelKernel/GeoShapeShift.h"
#include "GeoModelKernel/GeoShapeSubtraction.h"
#include "GeoModelKernel/GeoShapeUnion.h"
#include "GeoModelKernel/GeoUnidentifiedShape.h"


// Units
#include "GeoModelKernel/Units.h"
#define SYSTEM_OF_UNITS GeoModelKernelUnits // so we will get, e.g., 'GeoModelKernelUnits::cm'

//VP1Base
// TODO: we should get rid of VP1Base::VP1Msg dependency, since GeoModelRead should not depend on VP1 packages. Maybe we can move VP1Msg to a standalone package.
//#include "VP1Base/VP1Msg.h"

// C++ includes
#include <stdlib.h> /* exit, EXIT_FAILURE */
#include <stdexcept>
#include <future>
#include <mutex>
#include <chrono>   /* system_clock */
#include <ctime>    /* std::time */
#include <cstdlib>  /* std::getenv */
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <thread>


// mutexes for synchronized access to containers and output streams in multi-threading mode
std::mutex muxVPhysVol;
std::mutex muxCout;


using namespace GeoGenfun;
using namespace GeoXF;


namespace GeoModelIO {

ReadGeoModel::ReadGeoModel(GMDBManager* db, unsigned long* progress) : m_deepDebug(false),
  m_debug(false), m_timing(false), m_runMultithreaded(false),
  m_runMultithreaded_nThreads(0), m_progress(nullptr)
{
  // Check if the user asked for debug messages
  if ( "" != getEnvVar("GEOMODEL_ENV_IO_READ_DEBUG")) {
    m_debug = true;
    std::cout << "You defined the GEOMODEL_IO_DEBUG variable, so you will see a verbose output." << std::endl;
  }
  // Check if the user asked for verbose debug messages
  if ( "" != getEnvVar("GEOMODEL_ENV_IO_DEBUG_VERBOSE")) {
    m_deepDebug = true;
    std::cout << "You defined the GEOMODEL_IO_READ_DEBUG_VERBOSE variable, so you will see a verbose output." << std::endl;
  }
  // Check if the user asked for timing output
  if ( "" != getEnvVar("GEOMODEL_ENV_IO_READ_TIMING")) {
    m_timing = true;
    std::cout << "You defined the GEOMODEL_IO_READ_TIMING variable, so you will see a timing measurement in the output." << std::endl;
  }

	if ( progress != nullptr) {
	m_progress = progress;
	}

	// open the geometry file
	m_dbManager = db;
	if (m_dbManager->checkIsDBOpen()) {
    if (m_debug) std::cout << "OK! Database is open!";
	}
	else {
    std::cout << "ERROR!! Database is NOT open!";
		return;
	}
  // build caches
  m_dbManager->loadGeoNodeTypesAndBuildCache();
  

  // Check if the user asked for running in serial or multi-threading mode
  if ( "" != getEnvVar("GEOMODEL_ENV_IO_NTHREADS"))
  {
    int nThreads = std::stoi(getEnvVar("GEOMODEL_ENV_IO_NTHREADS"));
    if (nThreads == 0) {
      std::cout << "Info: You set the GEOMODEL_ENV_IO_NTHREADS to '" << nThreads << "'; thus, GeoModelIO will be run in serial mode." << std::endl;
      m_runMultithreaded_nThreads = nThreads;
      m_runMultithreaded = false;
    } else if( nThreads > 0 ) {
      std::cout << "Info: You set the GEOMODEL_ENV_IO_NTHREADS to '" << nThreads << "'; thus, GeoModelIO will use that number of worker threads." << std::endl;
      m_runMultithreaded_nThreads = nThreads;
      m_runMultithreaded = true;
    } else if (nThreads == -1) {
      std::cout << "Info: You set the GEOMODEL_ENV_IO_NTHREADS to '" << nThreads << "'; thus, GeoModelIO will use the number of threads supported by the platform." << std::endl;
      m_runMultithreaded_nThreads = nThreads;
      m_runMultithreaded = true;
    }
  }
  // if the user did not specify the number of threads, then we just use the hardware-supported number of threads on the platform
  else {
    m_runMultithreaded_nThreads = -1;
    m_runMultithreaded = true;
  }
}

ReadGeoModel::~ReadGeoModel() {
  // FIXME: some cleaning...??
}

  // FIXME: TODO: move to an utility class
std::string ReadGeoModel::getEnvVar( std::string const & key ) const
{
    char * val = std::getenv( key.c_str() );
    return val == NULL ? std::string("") : std::string(val);
}

GeoPhysVol* ReadGeoModel::buildGeoModel()
{
  if (m_deepDebug) std::cout << "ReadGeoModel::buildGeoModel()" << std::endl;

	GeoPhysVol* rootVolume = buildGeoModelPrivate();

  // warn the user if there are unknown/unhalded shapes
	if (m_unknown_shapes.size() > 0) {
		std::cout << "\tWARNING!! There were unknwon shapes:" << std::endl;
		for ( auto it = m_unknown_shapes.begin(); it != m_unknown_shapes.end(); it++ ) {
			std::cout << "\t\t---> " << *it << std::endl;
		}
		std::cout << "\tRemember: unknown shapes are rendered with a dummy cube of 30cm side length.\n\n" << std::endl;
		}

	return rootVolume;
}


GeoPhysVol* ReadGeoModel::buildGeoModelPrivate()
{
  // *** get all data from the DB ***
  std::chrono::system_clock::time_point start = std::chrono::system_clock::now(); // timing: get start time
	// get all GeoModel nodes from the DB
	m_logVols = m_dbManager->getTableFromNodeType("GeoLogVol");
	m_shapes = m_dbManager->getTableFromNodeType("GeoShape");
	m_materials = m_dbManager->getTableFromNodeType("GeoMaterial");
	m_elements = m_dbManager->getTableFromNodeType("GeoElement");
	m_functions = m_dbManager->getTableFromNodeType("Function");
  m_physVols = m_dbManager->getTableFromNodeType("GeoPhysVol");
  m_fullPhysVols = m_dbManager->getTableFromNodeType("GeoFullPhysVol");
  m_transforms = m_dbManager->getTableFromNodeType("GeoTransform");
  m_alignableTransforms = m_dbManager->getTableFromNodeType("GeoAlignableTransform");
  m_serialDenominators = m_dbManager->getTableFromNodeType("GeoSerialDenominator");
  m_serialTransformers = m_dbManager->getTableFromNodeType("GeoSerialTransformer");
  m_nameTags = m_dbManager->getTableFromNodeType("GeoNameTag");
  // get the children table from DB
  m_allchildren = m_dbManager->getChildrenTable();
	// get the root volume data
  m_root_vol_data = m_dbManager->getRootPhysVol();
  // get DB metadata
  m_tableID_toTableName = m_dbManager->getAll_TableIDsNodeTypes();
  m_tableName_toTableID = m_dbManager->getAll_NodeTypesTableIDs();
  
  auto end = std::chrono::system_clock::now(); // timing: get end time
  auto diff = std::chrono::duration_cast < std::chrono::seconds > (end - start).count();
  if (m_timing || m_debug || m_deepDebug) {
    std::cout << "*** Time taken to fetch GeoModel data from the database: " << diff << " [s]" << std::endl;
  }

  // *** build all nodes ***
  start = std::chrono::system_clock::now(); // timing: get start time
  // parallel mode:
  if (m_runMultithreaded) {
  std::cout << "Building nodes concurrently..." << std::endl;
  std::thread t2(&ReadGeoModel::buildAllElements, this);
    //  std::thread t7(&ReadGeoModel::buildAllFunctions, this); // FIXME: implement cache for Functions
  
  std::thread t8(&ReadGeoModel::buildAllTransforms, this);
  std::thread t9(&ReadGeoModel::buildAllAlignableTransforms, this);
  std::thread t10(&ReadGeoModel::buildAllSerialDenominators, this);
  std::thread t11(&ReadGeoModel::buildAllNameTags, this);
  
  t8.join(); // ok, all Transforms have been built
  t9.join(); // ok, all AlignableTransforms have been built
  // needs Transforms and AlignableTransforms for Shift boolean shapes
  std::thread t1(&ReadGeoModel::buildAllShapes, this);
  
  t2.join(); // ok, all Elements have been built
  // needs Elements
  std::thread t3(&ReadGeoModel::buildAllMaterials, this);

  t1.join(); // ok, all Shapes have been built
  t3.join(); // ok, all Materials have been built
  // needs Shapes and Materials
  std::thread t4(&ReadGeoModel::buildAllLogVols, this);

  t4.join(); // ok, all LogVols have been built
  // needs LogVols
  std::thread t5(&ReadGeoModel::buildAllPhysVols, this);
  std::thread t6(&ReadGeoModel::buildAllFullPhysVols, this);

  t5.join(); // ok, all PhysVols have been built
  t6.join(); // ok, all FullPhysVols have been built
//  t7.join(); // ok, all Functions have been built
  // needs Functions, PhysVols, FullPhysVols
  std::thread t12(&ReadGeoModel::buildAllSerialTransformers, this);

  t10.join(); // ok, all SerialDenominators have been built
  t11.join(); // ok, all NameTags have been built
  t12.join(); // ok, all SerialTransformers have been built
  }
  // serial mode:
  else {
    std::cout << "Building nodes serially..." << std::endl;
    buildAllElements();
    // buildAllFunctions();
    buildAllTransforms();
    buildAllAlignableTransforms();
    buildAllSerialDenominators();
    buildAllNameTags();
    buildAllShapes();
    buildAllMaterials();
    buildAllLogVols();
    buildAllPhysVols();
    buildAllFullPhysVols();
    buildAllSerialTransformers();
  }
  end = std::chrono::system_clock::now(); // timing: get end time
  diff = std::chrono::duration_cast < std::chrono::seconds > (end - start).count();
  if (m_timing || m_debug || m_deepDebug) {
    std::cout << "*** Time taken to build all GeoModel nodes: " << diff << " [s]" << std::endl;
  }
  
  // *** recreate all mother-daughter relatioships between nodes ***
  start = std::chrono::system_clock::now(); // timing: get start time
  loopOverAllChildrenInBunches();
  end = std::chrono::system_clock::now(); // timing: get end time
  diff = std::chrono::duration_cast < std::chrono::seconds > (end - start).count();
  if (m_timing || m_debug || m_deepDebug) {
    std::cout << "*** Time taken to recreate all mother-daughter relationships between nodes of the GeoModel tree: " << diff << " [s]" << std::endl;
  }
  
	return getRootVolume();
}


//----------------------------------------
// loop over parent-child relationship data
  void ReadGeoModel::loopOverAllChildrenRecords(std::vector<std::vector<std::string>> records) 
{
  
  int nChildrenRecords = records.size();
  
  if (m_debug || m_deepDebug) {
    muxCout.lock();
    std::cout << "Thread " << std::this_thread::get_id() << " - processing " << nChildrenRecords << " keys..." << std::endl;
    muxCout.unlock();
  }
  
//  // Get Start Time
//  std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
  
  for ( auto& record : records ) {
    processParentChild( record );
  }
  
//  // Get End Time
//  auto end = std::chrono::system_clock::now();
//  auto diff = std::chrono::duration_cast < std::chrono::seconds > (end - start).count();
//
//  if (m_timing || m_debug || m_deepDebug) {
//    muxCout.lock();
//    std::cout << "Time Taken to process " << nChildrenRecords << " parent-child relationships = " << diff << " Seconds" << std::endl;
//    muxCout.unlock();
//  }
}


//! Iterate over the list of shapes, build them all, and store their pointers
void ReadGeoModel::buildAllShapes()
{
  if (m_debug) std::cout << "Building all shapes...\n";
  size_t nSize = m_shapes.size();
  m_memMapShapes.reserve( nSize*2 ); // TODO: check if *2 is good or redundant...
  for (unsigned int ii=0; ii<nSize; ++ii) {
    const unsigned int shapeID = std::stoi(m_shapes[ii][0]);
    type_shapes_boolean_info shapes_info_sub; // tuple to store the boolean shapes to complete at a second stage
    buildShape(shapeID, &shapes_info_sub);
    createBooleanShapeOperands(&shapes_info_sub);
  }
  if (nSize>0) std::cout << "All " << nSize << " Shapes have been built!\n";
}

//! Iterate over the list of GeoSerialDenominator nodes, build them all, and store their pointers
void ReadGeoModel::buildAllSerialDenominators()
{
  if (m_debug) std::cout << "Building all SerialDenominator nodes...\n";
  size_t nSize = m_serialDenominators.size();
  m_memMapSerialDenominators.reserve( nSize*2 ); // TODO: check if *2 is good or redundant...
  for (unsigned int ii=0; ii<nSize; ++ii) {
    const unsigned int nodeID = std::stoi(m_serialDenominators[ii][0]);
    const std::string baseName = m_serialDenominators[ii][1];
    GeoSerialDenominator* nodePtr = new GeoSerialDenominator(baseName);
    storeBuiltSerialDenominator(nodeID, nodePtr);
  }
  if (nSize>0) std::cout << "All " << nSize << " SerialDenominators have been built!\n";
}
  
//! Iterate over the list of GeoSerialDenominator nodes, build them all, and store their pointers
void ReadGeoModel::buildAllNameTags()
{
  if (m_debug) std::cout << "Building all NameTag nodes...\n";
  size_t nSize = m_nameTags.size();
  m_memMapNameTags.reserve( nSize*2 ); // TODO: check if *2 is good or redundant...
  for (unsigned int ii=0; ii<nSize; ++ii) {
    const unsigned int nodeID = std::stoi(m_nameTags[ii][0]);
    const std::string baseName = m_nameTags[ii][1];
    GeoNameTag* nodePtr = new GeoNameTag(baseName);
    storeBuiltNameTag(nodeID, nodePtr);
  }
  if (nSize>0) std::cout << "All " << nSize << " NameTags have been built!\n";
}
  
//! Iterate over the list of nodes, build them all, and store their pointers
void ReadGeoModel::buildAllElements()
{
  if (m_debug) std::cout << "Building all Elements...\n";
  size_t nSize = m_elements.size();
  m_memMapElements.reserve( nSize*2 ); // TODO: check if *2 is good or redundant...
  for (unsigned int ii=0; ii<nSize; ++ii) {
    const unsigned int nodeID = std::stoi(m_elements[ii][0]);
    buildElement(nodeID); // nodes' IDs start from 1
  }
  if (nSize>0) std::cout << "All " << nSize << " Elements have been built!\n";
}
  
//! Iterate over the list of nodes, build them all, and store their pointers
void ReadGeoModel::buildAllMaterials()
{
  if (m_debug) std::cout << "Building all Materials...\n";
  size_t nSize = m_materials.size();
  m_memMapMaterials.reserve( nSize*2 ); // TODO: check if *2 is good or redundant...
  for (unsigned int ii=0; ii<nSize; ++ii) {
    const unsigned int nodeID = std::stoi(m_materials[ii][0]);
    buildMaterial(nodeID); // nodes' IDs start from 1
  }
  if (nSize>0) std::cout << "All " << nSize << " Materials have been built!\n";
}
  
//! Iterate over the list of nodes, build them all, and store their pointers
void ReadGeoModel::buildAllLogVols()
{
  if (m_debug) std::cout << "Building all LogVols...\n";
  size_t nSize = m_logVols.size();
  m_memMapLogVols.reserve( nSize*2 ); // TODO: check if *2 is good or redundant...
  for (unsigned int ii=0; ii<nSize; ++ii) {
    const unsigned int nodeID = std::stoi(m_logVols[ii][0]);
    buildLogVol(nodeID);
  }
  if (nSize>0) std::cout << "All " << nSize << " LogVols have been built!\n";
}

  //// FIXME: TODO: move to an utility class
//void printQHashQstringUInt(QHash<QString, unsigned int> qq) {
//  QHash<QString, unsigned int>::const_iterator i = qq.constBegin();
//  while (i != qq.constEnd()) {
//    std::cout << i.key().toStdString() << ": " << i.value() << std::endl;
//  }
//}

//! Iterate over the list of nodes, build them all, and store their pointers
void ReadGeoModel::buildAllPhysVols()
{
  if (m_debug) std::cout << "Building all PhysVols...\n";
  if (m_physVols.size() == 0) {
    std::cout << "ERROR!!! No input PhysVols found! Exiting..." << std::endl;
    exit(EXIT_FAILURE);
  }
  const unsigned int tableID = m_tableName_toTableID["GeoPhysVol"];
  size_t nSize = m_physVols.size();
  m_memMapPhysVols.reserve( nSize*2 ); // TODO: check if *2 is good or redundant...
  for (unsigned int ii=0; ii<nSize; ++ii) {
    const unsigned int volID = std::stoi(m_physVols[ii][0]);
    const unsigned int logVolID = std::stoi(m_physVols[ii][1]);
    // std::cout << "building PhysVol n. " << volID << " (logVol: " << logVolID << ")" << std::endl;
    buildVPhysVol(volID, tableID, logVolID);
  }
  if (nSize>0) std::cout << "All " << nSize << " PhysVols have been built!\n";
}
  
//! Iterate over the list of nodes, build them all, and store their pointers
void ReadGeoModel::buildAllFullPhysVols()
{
  if (m_debug) std::cout << "Building all FullPhysVols...\n";
  const unsigned int tableID = m_tableName_toTableID["GeoFullPhysVol"];
  size_t nSize = m_fullPhysVols.size();
  m_memMapFullPhysVols.reserve( nSize*2 ); // TODO: check if *2 is good or redundant...
  for (unsigned int ii=0; ii<nSize; ++ii) {
    const unsigned int volID = std::stoi(m_fullPhysVols[ii][0]);
    const unsigned int logVolID = std::stoi(m_fullPhysVols[ii][1]);
    // std::cout << "building PhysVol n. " << volID << " (logVol: " << logVolID << ")" << std::endl;
    buildVPhysVol(volID, tableID, logVolID);
  }
  if (nSize>0) std::cout << "All " << nSize << " FullPhysVols have been built!\n";
}
  
//! Iterate over the list of GeoAlignableTransforms nodes, build them all, and store their pointers
void ReadGeoModel::buildAllAlignableTransforms()
{
  if (m_debug) std::cout << "Building all AlignableTransforms...\n";
  size_t nSize = m_alignableTransforms.size();
  m_memMapAlignableTransforms.reserve( nSize*2 ); // TODO: check if *2 is good or redundant...
  for (unsigned int ii=0; ii<nSize; ++ii) {
    const unsigned int volID = std::stoi(m_alignableTransforms[ii][0]);
    buildAlignableTransform(volID);
  }
  if (nSize>0) std::cout << "All " << nSize << " AlignableTransforms have been built!\n";
}
  
//! Iterate over the list of GeoTransforms nodes, build them all, and store their pointers
void ReadGeoModel::buildAllTransforms()
{
  if (m_debug) std::cout << "Building all Transforms...\n";
  size_t nSize = m_transforms.size();
  m_memMapTransforms.reserve( nSize*2 ); // TODO: check if *2 is good or redundant...
  for (unsigned int ii=0; ii<nSize; ++ii) {
    const unsigned int volID = std::stoi(m_transforms[ii][0]);
    buildTransform(volID);
  }
  if (nSize>0) std::cout << "All " << nSize << " Transforms have been built!\n";
}

//! Iterate over the list of GeoTransforms nodes, build them all, and store their pointers
void ReadGeoModel::buildAllSerialTransformers()
{
  if (m_debug) std::cout << "Building all SerialTransformers...\n";
  size_t nSize = m_serialTransformers.size();
  m_memMapSerialTransformers.reserve( nSize*2 ); // TODO: check if 2 is good or redundant...
  for (unsigned int ii=0; ii<nSize; ++ii) {
    const unsigned int volID = std::stoi(m_serialTransformers[ii][0]);
    buildSerialTransformer(volID);
  }
  if (nSize>0) std::cout << "All " << nSize << " SerialTransformers have been built!\n";
}
  
  // FIXME: implement build function and cache for Functions
// //! Iterate over the list of nodes, build them all, and store their pointers
// void ReadGeoModel::buildAllFunctions()
// {
//   if (m_debug) std::cout << "Building all Functions...\n";
////   if (m_serialTransformers.size() == 0) {
////     std::cout << "ERROR!!! input SerialTransformers are empty! Exiting..." << std::endl;
////     exit(EXIT_FAILURE);
////   }
////   size_t nSize = m_functions.size();
////   m_memMapFunctions.reserve( nSize*2 ); // TODO: check if 2 is good or redundant...
////   for (unsigned int ii=0; ii<nSize; ++ii) {
////     buildFunction(ii+1); // nodes' IDs start from 1
////   }
//   QHash<unsigned int, QStringList>::const_iterator i = m_functions.constBegin();
//   while (i != m_functions.constEnd()) {
//     unsigned int id = i.key();
//     buildFunction(id);
//     ++i;
//   }
//   std::cout << "All Functions have been built!\n";
// }



void ReadGeoModel::loopOverAllChildrenInBunches()
{
    int nChildrenRecords = m_allchildren.size();
    if (m_debug) std::cout << "number of children to process: " << nChildrenRecords << std::endl;
    
    // If we have a few children, then process them serially
    std::cout << "Running concurrently? " << m_runMultithreaded << std::endl;
    if (true) // !(m_runMultithreaded) || nChildrenRecords <= 500)
    {
      std::cout << "Running serially...\n";
      loopOverAllChildrenRecords(m_allchildren);
    }
    // ...otherwise, let's spawn some threads to process them in bunches, parallelly!
    else {
      
      std::cout << "Running concurrently...\n";
      
      std::chrono::system_clock::time_point start, end;
      if (m_timing || m_debug || m_deepDebug) {
        // Get Start Time
        start = std::chrono::system_clock::now();
      }
      
      // set number of worker threads
      unsigned int nThreads = 0;
      if(m_runMultithreaded_nThreads > 0) {
        nThreads = m_runMultithreaded_nThreads;
      }
      else if (m_runMultithreaded_nThreads == -1) {
        unsigned int nThreadsPlatform = std::thread::hardware_concurrency();
        nThreads = nThreadsPlatform;
        if (m_debug || m_deepDebug) std::cout << "INFO - You have asked for hardware native parellelism. On this platform, " << nThreadsPlatform << " concurrent threads are supported. Thus, using " << nThreads << " threads.\n";
      }
      
      unsigned int nBunches = nChildrenRecords / nThreads;
      if (m_debug || m_deepDebug) std::cout << "Processing " << nThreads << " bunches, with " << nBunches << " children each, plus the remainder." << std::endl;
      
      // a vector to store the "futures" of async calls
      std::vector<std::future<void>> futures;
      
      for (unsigned int bb=0; bb<nThreads; ++bb ) {
        
        std::vector<std::vector<std::string>> bunch;
        
        unsigned int start = nBunches * bb;
        int len = nBunches;
        const unsigned int stop = start + len;
        std::vector<std::vector<std::string>>::const_iterator first = m_allchildren.begin() + start;
        std::vector<std::vector<std::string>>::const_iterator last  = m_allchildren.begin() + stop;
        std::vector<std::vector<std::string>>::const_iterator end   = m_allchildren.end();
        if ( bb == (nThreads - 1) ) { // last bunch
          bunch = std::vector<std::vector<std::string>>(first, end);
        }
        else { // all bunches but last one
           bunch = std::vector<std::vector<std::string>>(first, last);
        }
        
        if (m_debug || m_deepDebug) {
          muxCout.lock();
          std::cout << "Thread " << bb+1 << " - Start: " << start << ", len: " << len << "   ['len=-1' = all remaining items]" << std::endl;
          muxCout.unlock();
        }
        
        
        if (m_debug || m_deepDebug) {
          muxCout.lock();
          std::cout << "'bunch' size: " << bunch.size() << std::endl;
          muxCout.unlock();
        }
        
        futures.push_back( std::async(std::launch::async, &ReadGeoModel::loopOverAllChildrenRecords, this, bunch) );
      }
      
      // wait for all async calls to complete
      //retrieve and print the value stored in the 'std::future'
      if (m_debug || m_deepDebug) std::cout << "Waiting for the threads to finish...\n" << std::flush;
      for(auto &e : futures) {
        e.wait();
      }
      if (m_debug || m_deepDebug) std::cout << "Done!\n";
      
      if (m_timing || m_debug || m_deepDebug) {
        // Get End Time
        end = std::chrono::system_clock::now();
        auto diff = std::chrono::duration_cast < std::chrono::seconds > (end - start).count();
        std::cout << "(Total time taken to recreate all " << nChildrenRecords << " mother-children relationships: " << diff << " seconds)" << std::endl;
      }
    }
    return;
  }

  void ReadGeoModel::processParentChild(const std::vector<std::string> &parentchild)
  {
    // safety check
    if (parentchild.size() < 8) {
      std::cout <<  "ERROR!!! Probably you are using an old geometry file. Please, get a new one. Exiting..." << std::endl;
      exit(EXIT_FAILURE);
    }
    
    // get the parent's details
    const unsigned int parentId = std::stoi(parentchild[1]);
    const unsigned int parentTableId = std::stoi(parentchild[2]);
    const unsigned int parentCopyN = std::stoi(parentchild[3]);

    // get the child's position in the parent's children list
    //const unsigned int position = std::stoi(parentchild[4]); // unused
    
    // get the child's details
    const unsigned int childTableId = std::stoi(parentchild[5]);
    const unsigned int childId = std::stoi(parentchild[6]);
    const unsigned int childCopyN = std::stoi(parentchild[7]);
    
//    std::string childNodeType = m_tableID_toTableName[childTableId].toStdString();
    std::string childNodeType = m_tableID_toTableName[childTableId];
    
    if ( "" == childNodeType || 0 == childNodeType.size()) {
      std::cout << "ERROR!!! childNodeType is empty!!! Aborting..." << std::endl;
      exit(EXIT_FAILURE);
    }

    GeoVPhysVol* parentVol = nullptr;
    
    // build or get parent volume.
    // Using the parentCopyNumber here, to get a given instance of the parent volume
    parentVol = dynamic_cast<GeoVPhysVol*>( buildVPhysVolInstance(parentId, parentTableId, parentCopyN) );
    std::string parentName = parentVol->getLogVol()->getName();
	
	if (childNodeType == "GeoPhysVol") {
		GeoPhysVol* childNode = dynamic_cast<GeoPhysVol*>(buildVPhysVolInstance(childId, childTableId, childCopyN));
    volAddHelper(parentVol, childNode);
	}
	else if (childNodeType == "GeoFullPhysVol") {
		GeoFullPhysVol* childNode = dynamic_cast<GeoFullPhysVol*>(buildVPhysVolInstance(childId, childTableId, childCopyN));
    volAddHelper(parentVol, childNode);
	}
	else if (childNodeType == "GeoSerialDenominator") {
    GeoSerialDenominator* childNode = getBuiltSerialDenominator(childId);
		volAddHelper(parentVol, childNode);
	}
	else if (childNodeType == "GeoAlignableTransform") {
    GeoAlignableTransform* childNode = getBuiltAlignableTransform(childId);
		volAddHelper(parentVol, childNode);
	}
	else if (childNodeType == "GeoTransform") {
    GeoTransform* childNode = getBuiltTransform(childId);
		volAddHelper(parentVol, childNode);
	}
	else if (childNodeType == "GeoSerialTransformer") {
    GeoSerialTransformer* childNode = getBuiltSerialTransformer(childId);
		volAddHelper(parentVol, childNode);
	}
	else if (childNodeType == "GeoNameTag") {
    GeoNameTag* childNode = getBuiltNameTag(childId);
		volAddHelper(parentVol, childNode);
  }
	else {
    std::cout << "[" << childNodeType << "] ==> ERROR!!! - The conversion for this type of child node needs to be implemented." << std::endl;
    exit(EXIT_FAILURE);
	}
}

void ReadGeoModel::volAddHelper(GeoVPhysVol* vol, GeoGraphNode* volChild)
{
  checkNodePtr(vol, "vol", __func__, __PRETTY_FUNCTION__);
  checkNodePtr(volChild, "volChild", __func__, __PRETTY_FUNCTION__);
	if (dynamic_cast<GeoPhysVol*>(vol)) {
		GeoPhysVol* volume = dynamic_cast<GeoPhysVol*>(vol);
		volume->add(volChild);
	} else if (dynamic_cast<GeoFullPhysVol*>(vol)) {
		GeoFullPhysVol* volume = dynamic_cast<GeoFullPhysVol*>(vol);
		volume->add(volChild);
	}
}

//TODO: to be moved to an utility class
void ReadGeoModel::checkNodePtr(GeoGraphNode* nodePtr, std::string varName, std::string funcName, std::string funcSignature)
{
  if (!nodePtr) {
    muxCout.lock();
    std::cout << "ERROR! A pointer to a GeoGraphNode instance is NULL {"<<varName<<" "<<funcName<<" "<<"["<<funcSignature<<"]}. Exiting..." << std::endl;
    exit(EXIT_FAILURE);
    muxCout.unlock();
  }
}

  // TODO: move it to an utility class
//void ReadGeoModel::checkInputString(QString input)
//{
//  if (input.isEmpty() || input.isNull() || input == "NULL") {
//    std::cout << "ERROR!!! Input QString is empty or equal to 'NULL'!!! Aborting..." << std::endl;
//    exit(1);
//  }
//}


// Instantiate a PhysVol and get its children
GeoVPhysVol* ReadGeoModel::buildVPhysVolInstance(const unsigned int id, const unsigned int tableId, const unsigned int copyN)
{
	if (m_deepDebug) {
    muxCout.lock();
    std::cout << "ReadGeoModel::buildVPhysVolInstance() - " << id << ", " << tableId << ", " << copyN << std::endl;
    muxCout.unlock();
  }

	// A - if the instance has been previously built, return that
  if ( nullptr != getVPhysVol(id, tableId, copyN)) {
//  if (isVPhysVolBuilt(id, tableId, copyN)) {
		if (m_deepDebug) {
      muxCout.lock();
      std::cout << "getting the instance volume from memory..." << std::endl;
      muxCout.unlock();
    }
		return dynamic_cast<GeoVPhysVol*>(getVPhysVol(id, tableId, copyN));
	}
  
  // B - if not built already, then get the actual volume,
  // which should be already built by now,
  // get the logVol from it and build a new VPhysVol instance in the heap;
  // then, associate a copy number to it,
  // and store the new instance into the cache.
  GeoVPhysVol* vol = nullptr;
  bool volFound = true;
  if (1==tableId) {
    if(isBuiltPhysVol(id))
      vol = new GeoPhysVol( getBuiltPhysVol(id)->getLogVol() );
    else
      volFound = false;
  }
  else if (2==tableId) {
    if(isBuiltFullPhysVol(id))
      vol = new GeoFullPhysVol( getBuiltFullPhysVol(id)->getLogVol() );
    else
      volFound = false;
  }
  if (!volFound) {
    std::cout << "ERROR! VPhysVol not found! It should be already built, by now. Exiting...\n";
    exit(EXIT_FAILURE);
  }
  storeVPhysVol(id, tableId, copyN, vol);
  
	return vol;
}

//! Build the actual VPhysVol (GeoPhysVol or GeoFullPhysVol), which will then be used to create
//! instances of that volume by using the `copyNumber`.
//! Here, however, we do not need to specify
//! any copyNumber, because the actual GeoPVPhysVol is the same for all copy instances.
GeoVPhysVol* ReadGeoModel::buildVPhysVol(const unsigned int id, const unsigned int tableId, unsigned int /*defaults to "0"*/ logVol_ID)
{

  if (m_deepDebug) { muxCout.lock(); std::cout << "ReadGeoModel::buildVPhysVol() - " << id << ", " << tableId << std::endl; muxCout.unlock(); }

  std::string nodeType = m_tableID_toTableName[tableId];

  bool errorType = false;

  // get the actual VPhysVol volume, if built already
  if ( nodeType == "GeoPhysVol" && isBuiltPhysVol(id) ) {
    if (m_deepDebug) { muxCout.lock(); std::cout << "getting the actual PhysVol from cache...\n"; ; muxCout.unlock(); }
    return getBuiltPhysVol(id);
  } else if ( nodeType == "GeoFullPhysVol" && isBuiltFullPhysVol(id)){
    if (m_deepDebug) { muxCout.lock(); std::cout << "getting the actual FullPhysVol from cache...\n"; ; muxCout.unlock(); }
    return getBuiltFullPhysVol(id);
  }
  
  // if not built already, then get its parameters and build it now
  if (logVol_ID==0) {
    // get the volume's parameters
    std::vector<std::string> values;
    if (nodeType == "GeoPhysVol")
      values = m_physVols[id];
    else if (nodeType == "GeoFullPhysVol")
      values = m_fullPhysVols[id];
    
    logVol_ID = std::stoi(values[1]);
  }

	// GET LOGVOL
  GeoLogVol* logVol = getBuiltLog(logVol_ID);
  if (!logVol) {
    std::cout << "ERROR!!! LogVol is NULL!" << std::endl;
//    exit(EXIT_FAILURE);
  }

	// a pointer to the VPhysVol
	GeoVPhysVol* vol = nullptr;

	// BUILD THE PHYSVOL OR THE FULLPHYSVOL
	if (nodeType == "GeoPhysVol") {
		GeoPhysVol* pVol = new GeoPhysVol(logVol);
    storeBuiltPhysVol(id, pVol);
    vol = pVol;
  }
	else if (nodeType == "GeoFullPhysVol") {
		GeoFullPhysVol* fpVol = new GeoFullPhysVol(logVol);
    storeBuiltFullPhysVol(id, fpVol);
    vol = fpVol;
  }
  else
    errorType = true;

  if (errorType) {
    muxCout.lock();
    std::cout << "ERROR! Unkonwn node type. Exiting..." <<std::endl;
    muxCout.unlock();
    exit(EXIT_FAILURE);
  }

  return vol;
}



// Get the root volume
GeoPhysVol* ReadGeoModel::getRootVolume()
{
	if (m_deepDebug) {
    muxCout.lock();
    std::cout << "ReadGeoModel::getRootVolume()" << std::endl;
    muxCout.unlock();
  }
  const unsigned int id = std::stoi(m_root_vol_data[1]); // TODO: GeoModel GetRoot() should return integers instead of strings...
  const unsigned int tableId = std::stoi(m_root_vol_data[2]);
	const unsigned int copyNumber = 1; // the Root volume has only one copy by definition
  GeoPhysVol* root = dynamic_cast<GeoPhysVol*>(buildVPhysVolInstance(id, tableId, copyNumber));
  checkNodePtr(root, "root", __func__, __PRETTY_FUNCTION__);
  return root;
}


GeoMaterial* ReadGeoModel::buildMaterial(const unsigned int id)
{
  if ( isBuiltMaterial(id) ){
    return getBuiltMaterial(id);
  }

	if (m_deepDebug) {
    muxCout.lock();
    std::cout << "ReadGeoModel::buildMaterial()" << std::endl;
    muxCout.unlock();
  }
  std::vector<std::string> values = m_materials[id-1];

  const unsigned int matId = std::stoi(values[0]);
  const std::string matName = values[1];
  double matDensity = std::stod(values[2]);
  std::string matElements = values[3];

	if (m_deepDebug) {
    muxCout.lock();
    std::cout << "\tMaterial - ID:" << matId
		          << ", name:" << matName
		          << ", density:" << matDensity
		          << " ( " << matDensity / (SYSTEM_OF_UNITS::g/SYSTEM_OF_UNITS::cm3) << "[g/cm3] )"
		          << ", elements:" << matElements;
    muxCout.unlock();
  }

	GeoMaterial* mat = new GeoMaterial(matName, matDensity);

	if (matElements.size() > 0) {
		// get parameters from DB string
    const std::vector<std::string> elements = splitString(matElements, ';');
    for( auto& par : elements) {

		  if (m_deepDebug) {
        muxCout.lock();
        std::cout << "par:" << par;
        muxCout.unlock();
      }
      std::vector<std::string> vars = splitString(par, ':');
      const unsigned int elId = std::stoi(vars[0]);
      double elFraction = std::stod(vars[1]);
//      GeoElement* el = buildElement(elId);
      GeoElement* el = getBuiltElement(elId);
			mat->add(el, elFraction);
		}
		mat->lock();
	}
  storeBuiltMaterial(id, mat);
	return mat;
}

GeoElement* ReadGeoModel::buildElement(const unsigned int id)
{
  if( isBuiltElement(id) ) {
    return getBuiltElement(id);
  }

	if (m_deepDebug) {
    muxCout.lock();
    std::cout << "ReadGeoModel::buildElement()" << std::endl;
    muxCout.unlock();
  }

	if (m_elements.size() == 0)
  std::cout << "ERROR! 'm_elements' is empty! Did you load the 'Elements' table? \n\t ==> Aborting...\n" << std::endl;

  std::vector<std::string> values = m_elements[id-1];

  const unsigned int elId = std::stoi(values[0]);
  std::string elName = values[1];
  std::string elSymbol = values[2];
  double elZ = std::stod(values[3]);
  double elA = std::stod(values[4]);

	if (m_deepDebug) {
    muxCout.lock();
    std::cout << "\tElement - ID:" << elId
			        << ", name:" << elName
			        << ", symbol:" << elSymbol
			        << ", Z:" << elZ
		       	  << ", A:" << elA
			        << " ( " << elA / (SYSTEM_OF_UNITS::g/SYSTEM_OF_UNITS::mole) << "[g/mole] )"
              << std::endl;
    muxCout.unlock();
  }

	GeoElement* elem = new GeoElement(elName, elSymbol, elZ, elA);
  storeBuiltElement(id, elem);
  return elem;
}


std::string ReadGeoModel::getShapeType(const unsigned int shapeId)
{
    if (shapeId > m_shapes.size()) {
      std::cout << "ERROR!! Shape ID is larger than the container size. Exiting..." << std::endl;
      exit(EXIT_FAILURE);
    }
  std::vector<std::string> paramsShape = m_shapes[ shapeId-1 ];//remember: shapes' IDs start from 1
  std::string type = paramsShape[1];
  return type;
}

  

  //TODO: move shapes in different files, so code here is more managable
/// Recursive function, to build GeoShape nodes
GeoShape* ReadGeoModel::buildShape(const unsigned int shapeId, type_shapes_boolean_info* shapes_info_sub)
{
  if (isBuiltShape(shapeId)) {
    return getBuiltShape(shapeId);
  }

	if (m_deepDebug) {
     muxCout.lock();
     std::cout << "ReadGeoModel::buildShape()" << std::endl;
     muxCout.unlock();
  }

//   try // TODO: implement try/catch
//   {
  std::vector<std::string> paramsShape = m_shapes[ shapeId-1 ]; // remember: nodes' IDs start from 1

//  const unsigned int id = std::stoi(paramsShape[0]); // unused
  std::string type = paramsShape[1];
  std::string parameters = paramsShape[2];

  // Get shape's parameters from the stored string.
  // This will be interpreted differently according to the shape.
  std::vector<std::string> shapePars = splitString(parameters, ';');
  
  GeoShape* shape = nullptr;

	if (type == "Box") {
			// shape parameters
			double XHalfLength = 0.;
			double YHalfLength = 0.;
			double ZHalfLength = 0.;
			// get parameters from DB string
      for( auto& par : shapePars) {
        std::vector<std::string> vars = splitString(par, '=');
        std::string varName = vars[0];
        std::string varValue = vars[1];
        if (varName == "XHalfLength") XHalfLength = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
        if (varName == "YHalfLength") YHalfLength = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
        if (varName == "ZHalfLength") ZHalfLength = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
			}
		shape = new GeoBox(XHalfLength, YHalfLength, ZHalfLength);
	}
	else if (type == "Cons") {
		// shape parameters
		double RMin1 = 0.;
		double RMin2 = 0.;
		double RMax1 = 0.;
		double RMax2 = 0.;
		double DZ = 0.;
		double SPhi = 0.;
		double DPhi = 0.;
		// get parameters from DB string
    for( auto& par : shapePars) {
      std::vector<std::string> vars = splitString(par, '=');
      std::string varName = vars[0];
      std::string varValue = vars[1];
      // std::cout << "varValue Cons:" << varValue;
			if (varName == "RMin1") RMin1 = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
			if (varName == "RMin2") RMin2 = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
			if (varName == "RMax1") RMax1 = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
			if (varName == "RMax2") RMax2 = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
			if (varName == "DZ")    DZ = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
			if (varName == "SPhi")  SPhi = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
			if (varName == "DPhi")  DPhi = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
		}
		shape = new GeoCons (RMin1, RMin2, RMax1, RMax2, DZ, SPhi, DPhi);
	}
	else if (type == "Torus") {
		// Member Data:
		// * Rmax - outside radius of the torus tube
		// * Rmin - inside radius  of the torus tube (Rmin=0 if not hollow)
		// * Rtor - radius of the torus itself
		// *
		// * SPhi - starting angle of the segment in radians
		// * DPhi - delta angle of the segment in radians
		//
		// shape parameters
		double Rmin = 0.;
		double Rmax = 0.;
		double Rtor = 0.;
		double SPhi = 0.;
		double DPhi = 0.;
		// get parameters from DB string
    for( auto& par : shapePars) {
      std::vector<std::string> vars = splitString(par, '=');
      std::string varName = vars[0];
      std::string varValue = vars[1];
			if (varName == "Rmin") Rmin = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
			if (varName == "Rmax") Rmax = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
			if (varName == "Rtor") Rtor = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
			if (varName == "SPhi") SPhi = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
			if (varName == "DPhi") DPhi = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
		}
		shape = new GeoTorus (Rmin, Rmax, Rtor, SPhi, DPhi);
	}
	else if (type == "Para") {
		// shape parameters
		double XHalfLength = 0.;
		double YHalfLength = 0.;
		double ZHalfLength = 0.;
		double Alpha = 0.;
		double Theta = 0.;
		double Phi = 0.;
		// get parameters from DB string
    for( auto& par : shapePars) {
      std::vector<std::string> vars = splitString(par, '=');
      std::string varName = vars[0];
      std::string varValue = vars[1];
			if (varName == "XHalfLength") XHalfLength = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
			if (varName == "YHalfLength") YHalfLength = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
			if (varName == "ZHalfLength") ZHalfLength = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
			if (varName == "Alpha")       Alpha = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
			if (varName == "Theta")       Theta = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
			if (varName == "Phi")         Phi = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
		}
		shape = new GeoPara (XHalfLength, YHalfLength, ZHalfLength, Alpha, Theta, Phi);
	}
	else if (type == "Pcon") {
		// shape parameters
		double SPhi = 0.;
		double DPhi = 0.;
		unsigned int NZPlanes = 0;

		bool error = 0;
    std::string par;
    std::vector<std::string> vars;
    std::string varName;
    std::string varValue;

		GeoPcon* pcon = nullptr;

		int sizePars = shapePars.size();
		// check if we have more than 3 parameters
		if (sizePars > 3) {

			// get the three first GeoPcon parameters: the SPhi and DPhi angles, plus the number of Z planes
			for( int it=0; it < 3; it++) {
				par = shapePars[it];
				vars = splitString(par, '=');
				varName = vars[0];
				varValue = vars[1];
				if (varName == "SPhi")     SPhi = std::stod(varValue);
				if (varName == "DPhi")     DPhi = std::stod(varValue);
				if (varName == "NZPlanes") NZPlanes = std::stoi(varValue);
			}
			// build the basic GeoPcon shape
			pcon = new GeoPcon(SPhi, DPhi);

			// and now loop over the rest of the list, to get the parameters of all Z planes
			for (int it=3; it < sizePars; it++)
			{
				par = shapePars[it];
				vars = splitString(par, '=');
				varName = vars[0];
				varValue = vars[1];

				if (varName == "ZPos") {

					double zpos = std::stod(varValue);
					double rmin=0., rmax=0.;

					it++; // go to next variable

					par = shapePars[it];
					vars = splitString(par, '=');
					varName = vars[0];
					varValue = vars[1];
					if (varName == "ZRmin") rmin = std::stod(varValue);
					else error = 1;
					it++; // go to next variable

					par = shapePars[it];
					vars = splitString(par, '=');
					varName = vars[0];
					varValue = vars[1];
					if (varName == "ZRmax") rmax = std::stod(varValue);
					else error = 1;

					if(error) {
            muxCout.lock();
            std::cout << "ERROR! GeoPcon 'ZRmin' and 'ZRmax' values are not at the right place! --> ";
            printStdVectorStrings(shapePars);
            muxCout.unlock();
          }

					// add a Z plane to the GeoPcon
					pcon->addPlane(zpos, rmin, rmax);
				} else {
					error = 1;
          muxCout.lock();
          std::cout << "ERROR! GeoPcon 'ZPos' value is not at the right place! --> ";
          printStdVectorStrings(shapePars);
          muxCout.unlock();
				}
			}

			// sanity check on the resulting Pcon shape
			if( pcon->getNPlanes() != NZPlanes) {
				error = 1;
        muxCout.lock();
        std::cout << "ERROR! GeoPcon number of planes: " << pcon->getNPlanes() << " is not equal to the original size! --> ";
        printStdVectorStrings(shapePars);
        muxCout.unlock();
			}
			if(!pcon->isValid()) {
				error = 1;
        muxCout.lock();
        std::cout << "ERROR! GeoPcon shape is not valid!! -- input: ";
        printStdVectorStrings(shapePars);
        muxCout.unlock();
			}
	  } // end if (size>3)
		else {
      muxCout.lock();
      std::cout << "ERROR!! GeoPcon has no Z planes!! --> shape input parameters: ";
      printStdVectorStrings(shapePars);
      muxCout.unlock();
			error = 1;
		}

		if(error) {
      muxCout.lock();
      std::cout << "FATAL ERROR!!! - GeoPcon shape error!!! Aborting..." << std::endl;
      muxCout.unlock();
      exit(EXIT_FAILURE);
    }

		shape = pcon;
	}
	else if (type == "Pgon") {
		// shape parameters
		double SPhi = 0.;
		double DPhi = 0.;
		unsigned int NSides = 0;
		unsigned int NZPlanes = 0;

		bool error = false;
		GeoPgon* pgon = nullptr;
    
    std::string par;
    std::vector<std::string> vars;
    std::string varName;
    std::string varValue;

		int sizePars = shapePars.size();
		// check if we have more than 3 parameters
		if (sizePars > 3) {

			// get the first four GeoPgon parameters: the SPhi and DPhi angles, plus the number of Z planes
			for( int it=0; it < 4; it++) {
				par = shapePars[it];
				vars = splitString(par, '=');
				varName = vars[0];
				varValue = vars[1];
				// qInfo() << "vars: " << vars; // for debug only
				if (varName == "SPhi")     SPhi = std::stod(varValue);
				if (varName == "DPhi")     DPhi = std::stod(varValue);
				if (varName == "NSides")   NSides = std::stoi(varValue);
				if (varName == "NZPlanes") NZPlanes = std::stoi(varValue);

			}
			// build the basic GeoPgon shape
			pgon = new GeoPgon(SPhi, DPhi, NSides);

			// and now loop over the rest of the list, to get the parameters of all Z planes
			for (int it=4; it < sizePars; it++)
			{
				par = shapePars[it];
				vars = splitString(par, '=');
				varName = vars[0];
				varValue = vars[1];

				if (varName == "ZPos") {

					double zpos = std::stod(varValue);
					double rmin=0., rmax=0.;

					it++; // go to next variable

					par = shapePars[it];
					vars = splitString(par, '=');
					varName = vars[0];
					varValue = vars[1];
					if (varName == "ZRmin") rmin = std::stod(varValue);
					else error = 1;
					it++; // go to next variable

					par = shapePars[it];
					vars = splitString(par, '=');
					varName = vars[0];
					varValue = vars[1];
					if (varName == "ZRmax") rmax = std::stod(varValue);
					else error = 1;

					if(error) {
            muxCout.lock();
            std::cout << "ERROR! GeoPgon 'ZRmin' and 'ZRmax' values are not at the right place! --> ";
            printStdVectorStrings(shapePars);
            muxCout.unlock();
          }

					// add a Z plane to the GeoPgon
					pgon->addPlane(zpos, rmin, rmax);
				} else {
					error = 1;
          muxCout.lock();
          std::cout << "ERROR! GeoPgon 'ZPos' value is not at the right place! --> ";
          printStdVectorStrings(shapePars);
          muxCout.unlock();
				}
			}

			// sanity check on the resulting Pgon shape
			if( pgon->getNPlanes() != NZPlanes) {
				error = 1;
        muxCout.lock();
        std::cout << "ERROR! GeoPgon number of planes: " << pgon->getNPlanes() << " is not equal to the original size! --> ";
        printStdVectorStrings(shapePars);
        muxCout.unlock();
			}
			if(!pgon->isValid()) {
				error = 1;
        muxCout.lock();
        std::cout << "ERROR! GeoPgon shape is not valid!! -- input: ";
        printStdVectorStrings(shapePars);
        muxCout.unlock();
			}
		} // end if (size>3)
		else {
      muxCout.lock();
      std::cout << "ERROR!! GeoPgon has no Z planes!! --> shape input parameters: ";
      printStdVectorStrings(shapePars);
      muxCout.unlock();
			error = 1;
		}
		if(error) {
      muxCout.lock();
      std::cout << "FATAL ERROR!!! - GeoPgon shape error!!! Aborting..." << std::endl;
      muxCout.unlock();
      exit(EXIT_FAILURE);
    }
		shape = pgon;
	}
	else if (type == "GenericTrap") {
		// shape parameters
		double ZHalfLength = 0.;
		unsigned int NVertices = 0;
		GeoGenericTrapVertices Vertices;
		bool error = false;
		GeoGenericTrap* gTrap = nullptr;
		
    std::string par;
    std::vector<std::string> vars;
    std::string varName;
    std::string varValue;

		int sizePars = shapePars.size();
		// check if we have more than 3 parameters
		if (sizePars > 3) {

			// get the two GeoGenericTrap parameters: the ZHalfLength plus the number of vertices
			for( int it=0; it < 2; it++) {
				par = shapePars[it];
				vars = splitString(par, '=');
				varName = vars[0];
				varValue = vars[1];
				// qInfo() << "vars: " << vars; // for debug only
				if (varName == "ZHalfLength") ZHalfLength = std::stod(varValue);
				if (varName == "NVertices")   NVertices = std::stoi(varValue);
			}



			// and now loop over the rest of the list, to get the parameters of all Z planes
			for (int it=2; it < NVertices; it++)
			{
				par = shapePars[it];
				vars = splitString(par, '=');
				varName = vars[0];
				varValue = vars[1];

				if (varName == "X") {

					double x = std::stod(varValue);

					it++; // go to next variable

					par = shapePars[it];
					vars = splitString(par, '=');
					varName = vars[0];
					varValue = vars[1];
					if (varName == "Y") {
					  double y = std::stod(varValue);
					  Vertices.push_back(GeoTwoVector(x,y));
					}
					else {
					  error = 1;
					}
					if(error) {
            muxCout.lock();
            std::cout << "ERROR! GeoGenericTrap 'X' and 'Y' values are not at the right place! --> ";
            printStdVectorStrings(shapePars);
            muxCout.unlock();
          }
				} else {
					error = 1;
          muxCout.lock();
          std::cout << "ERROR! GeoGenericTrap 'ZPos' value is not at the right place! --> ";
          printStdVectorStrings(shapePars);
          muxCout.unlock();
				}
			}
      if(error) {
        muxCout.lock();
        std::cout << "FATAL ERROR!!! - GeoGenericTrap shape error!!! Aborting..." << std::endl;
        muxCout.unlock();
        exit(EXIT_FAILURE);
      }
			// build the basic GenericTrap shape
			gTrap = new GeoGenericTrap(ZHalfLength,Vertices);
		} // end if (size>3)
		else {
      muxCout.lock();
      std::cout << "ERROR!! GeoGenericTrap has no Z vertices!! --> shape input parameters: ";
      printStdVectorStrings(shapePars);
       muxCout.unlock();
			error = 1;
		}
    if(error) {
      muxCout.lock();
      std::cout << "FATAL ERROR!!! - GeoGenericTrap shape error!!! Aborting..." << std::endl;
      muxCout.unlock();
      exit(EXIT_FAILURE);
    }
		shape = gTrap;
	}
	else if (type == "SimplePolygonBrep") {
		// shape parameters
		double DZ = 0.;
		unsigned int NVertices = 0;
		double xV = 0.;
		double yV = 0.;

		bool error = 0;
		GeoSimplePolygonBrep* sh = nullptr;
    
    std::string par;
    std::vector<std::string> vars;
    std::string varName;
    std::string varValue;

		int sizePars = shapePars.size();
		// check if we have more than 2 parameters
		if (sizePars > 2) {

			// get the first two GeoSimplePolygonBrep parameters: DZ and the number of vertices.
			for( int it=0; it < 2; it++) {
				par = shapePars[it];
				vars = splitString(par, '=');
				varName = vars[0];
				varValue = vars[1];
				if (varName == "DZ")        DZ = std::stod(varValue);
				if (varName == "NVertices") NVertices = std::stoi(varValue);
				//else if (varName == "NVertices") NVertices = varValue.toDouble();
				//else error = 1;
				//if(error) std::cout << "ERROR! GeoSimplePolygonBrep parameters are not correctly stored! -->" << vars;

			}
			// build the basic GeoSimplePolygonBrep shape
			sh = new GeoSimplePolygonBrep(DZ);

			// and now loop over the rest of the list, to get the parameters of all vertices
			for (int it=2; it < sizePars; it++)
			{
				par = shapePars[it];
				vars = splitString(par, '=');
				varName = vars[0];
				varValue = vars[1];
				if (varName == "xV") xV = std::stod(varValue);
				else error = 1;

				it++; // go to next variable (they come in pairs)

				par = shapePars[it];
				vars = splitString(par, '=');
				varName = vars[0];
				varValue = vars[1];
				if (varName == "yV") yV = std::stod(varValue);
				else error = 1;

				if(error) {
          muxCout.lock();
          std::cout << "ERROR! GeoSimplePolygonBrep 'xVertex' and 'yVertex' values are not at the right place! --> ";
          printStdVectorStrings(shapePars);
          muxCout.unlock();
        }

				// add a Z plane to the GeoSimplePolygonBrep
				sh->addVertex(xV, yV);
			}
			// sanity check on the resulting shape
			if( sh->getNVertices() != NVertices) {
				error = 1;
        muxCout.lock();
        std::cout << "ERROR! GeoSimplePolygonBrep number of planes: " << sh->getNVertices() << " is not equal to the original size! --> ";
        printStdVectorStrings(shapePars);
        muxCout.unlock();
			}
			if(!sh->isValid()) {
				error = 1;
        muxCout.lock();
        std::cout << "ERROR! GeoSimplePolygonBrep shape is not valid!! -- input: ";
        printStdVectorStrings(shapePars);
        muxCout.unlock();
			}
		} // end if (size>3)
		else {
      muxCout.lock();
      std::cout << "ERROR!! GeoSimplePolygonBrep has no vertices!! --> shape input parameters: ";
      printStdVectorStrings(shapePars);
      muxCout.unlock();
			error = 1;
		}
    if(error) {
      muxCout.lock();
      std::cout << "FATAL ERROR!!! - GeoSimplePolygonBrep shape error!!! Aborting..." << std::endl;
      muxCout.unlock();
      exit(EXIT_FAILURE);
    }
		shape = sh;

	}
	else if (type == "TessellatedSolid") {
		// Tessellated pars example: "nFacets=1;TRI;vT=ABSOLUTE;nV=3;xV=0;yV=0;zV=1;xV=0;yV=1;zV=0;xV=1;yV=0;zV=0"
    std::cout << "Reading-in: TessellatedSolid: "; // debug
		// Facet type
    std::string facetType = "";
		// shape parameters
		unsigned int nFacets = 0;

		bool error = 0;
		GeoTessellatedSolid* sh = nullptr;
		
    std::string par;
    std::vector<std::string> vars;
    std::string varName;
    std::string varValue;

		int sizePars = shapePars.size();
		// check if we have at least 13 parameters,
		// which is the minimum for a shape
		// with a single triangular facet
		if (sizePars >= 13) {

			// get the first parameter
			par = shapePars[0];
			vars = splitString(par, '=');
			varName = vars[0];
			varValue = vars[1];
			if (varName == "nFacets") nFacets = std::stoi(varValue);
			else {
        muxCout.lock();
//        qWarning("ERROR!! - GeoTessellatedSolid - nFacets is not defined!!");
        muxCout.unlock();
				error = true; // TODO: check "error.h" functionalities and replace with that, if useful
			}

			// build the basic GeoTessellatedSolid shape
			sh = new GeoTessellatedSolid();

			// and now loop over the rest of the list,
			// to get the parameters of the vertices of each facet
			// and to build the full GeoTessellatedSolid with all the facets
			for (int it=1; it < sizePars; it++)
			{
				// get facet type
				par = shapePars[it];
				vars = splitString(par, '=');
				varName = vars[0];
				if (varName == "QUAD") {
					facetType = "QUAD";
				}
				else if (varName == "TRI") {
					facetType = "TRI";
				}
				else {
          muxCout.lock();
					std::cout << "ERROR!! - GeoTessellatedSolid - Facet type is not defined! [got: '" << varName << "']" << std::endl;
          muxCout.unlock();
					error = true;
				}

				it++; // advance to the next parameter

				// get the type of the vertexes composing the facet
				bool isAbsolute = true;
				par = shapePars[it];
				vars = splitString(par, '=');
				varName = vars[0];
				varValue = vars[1];
				if (varName == "vT") {
					if (varValue == "ABSOLUTE") isAbsolute = true;
					else if (varValue == "RELATIVE") isAbsolute = false;
					else {
            muxCout.lock();
						std::cout << "ERROR! - GeoTessellatedSolid - Vertex type not defined!" << std::endl;
            muxCout.unlock();
						error=true;
					}
				}
				else error = 1;

				it++; // advance to the next parameter

				unsigned int nVertexes = 0;
				par = shapePars[it];
				vars = splitString(par, '=');
				varName = vars[0];
				varValue = vars[1];
				if (varName == "nV") nVertexes = std::stoi(varValue);
				else {
          muxCout.lock();
					std::cout << "ERROR! - GeoTessellatedSolid - nVertices not defined!" << std::endl;
          muxCout.unlock();
					error=true;
				}


				// if we get a QUAD ==> GeoQuadrangularFacet
				if (facetType=="QUAD") {

          muxCout.lock();
					std::cout << "Handling a QUAD facet..." << std::endl;
          muxCout.unlock();
					// to store the 4 vertices of the GeoQuadrangularFacet
					auto vV = std::vector<std::unique_ptr<GeoFacetVertex>>{};

					// we look for 4 vertices for QUAD;
					// for each of them, we get 3 coordinates
					//					vertStart = it;
					for (unsigned int iV=0; iV<4; ++iV) {

						it++; // advance to the first of the facet's vertices' coordinates

						double xV=0.;
						par = shapePars[it];
						vars = splitString(par, '=');
						varName = vars[0];
						varValue = vars[1];
						if (varName == "xV") xV = std::stod(varValue);
						else {
              muxCout.lock();
							std::cout << "ERROR! Got '" << varName << "' instead of 'xV'!" << std::endl;
              muxCout.unlock();
							error = 1;
						}

						it++; // go to the next coordinate

						double yV=0.;
						par = shapePars[it];
						vars = splitString(par, '=');
						varName = vars[0];
						varValue = vars[1];
						if (varName == "yV") yV = std::stod(varValue);
						else {
              muxCout.lock();
							std::cout << "ERROR! Got '" << varName << "' instead of 'yV'!" << std::endl;
              muxCout.unlock();
							error = 1;
						}

						it++; // go to the next coordinate

						double zV=0.;
						par = shapePars[it];
						vars = splitString(par, '=');
						varName = vars[0];
						varValue = vars[1];
						if (varName == "zV") zV = std::stod(varValue);
						else {
              muxCout.lock();
							std::cout << "ERROR! Got '" << varName << "' instead of 'zV'!" << std::endl;
              muxCout.unlock();
							error = 1;
						}

						if(error) {
              muxCout.lock();
              std::cout << "ERROR! GeoTessellatedSolid 'xV', 'yV', and 'zV' values are not at the right place! --> ";
              printStdVectorStrings(shapePars);
              muxCout.unlock();
            }

						// build the facet's vertex and store it
						vV.push_back(std::make_unique<GeoFacetVertex>( GeoFacetVertex(xV,yV,zV)) );
					}

					// build the facet and add it to the GeoTessellatedSolid
					GeoQuadrangularFacet* quadFacet = new GeoQuadrangularFacet(*vV[0], *vV[1], *vV[2], *vV[3], (isAbsolute ? GeoFacet::ABSOLUTE : GeoFacet::RELATIVE));
					sh->addFacet(quadFacet);
				}
				// if we get a TRI ==> GeoTriangularFacet
				else if (facetType=="TRI") {

          muxCout.lock();
					std::cout << "Handling a TRI facet..." << std::endl;
          muxCout.unlock();

					// std::vector<GeoFacetVertex*> vV(3, 0); // to store the 3 vertices of the GeoTriangularFacet
					auto vV = std::vector<std::unique_ptr<GeoFacetVertex>>{};

					// we look for 3 vertices for GeoTriangularFacet;
					// for each of them, we get 3 coordinates
					// vertStart = it;
					for (unsigned int iV=0; iV<3; ++iV) {

						it++; // advance to the first of the facet's vertices' coordinates

						double xV=0.;
						par = shapePars[it];
						vars = splitString(par, '=');
						varName = vars[0];
						varValue = vars[1];
						if (varName == "xV") xV = std::stod(varValue);
						else error = 1;

						it++; // go to the next coordinate

						double yV=0.;
						par = shapePars[it];
						vars = splitString(par, '=');
						varName = vars[0];
						varValue = vars[1];
						if (varName == "yV") yV = std::stod(varValue);
						else error = 1;

						it++; // go to the next coordinate

						double zV=0.;
						par = shapePars[it];
						vars = splitString(par, '=');
						varName = vars[0];
						varValue = vars[1];
						if (varName == "zV") zV = std::stod(varValue);
						else error = 1;

						if(error) {
              muxCout.lock();
              std::cout << "ERROR! GeoTessellatedSolid 'xV', 'yV', and 'zV' values are not at the right place! --> ";
              printStdVectorStrings(shapePars);
              muxCout.unlock();
            }

						// build the facet's vertex and store it
						vV.push_back(std::make_unique<GeoFacetVertex>( GeoFacetVertex(xV,yV,zV)) );
					}

					// build the facet and add it to the GeoTessellatedSolid
					GeoTriangularFacet* triFacet = new GeoTriangularFacet(*vV[0], *vV[1], *vV[2], (isAbsolute ? GeoFacet::ABSOLUTE : GeoFacet::RELATIVE));
					sh->addFacet(triFacet);
				}


			}

			// sanity check on the resulting shape
			if( sh->getNumberOfFacets() != nFacets) {
				error = 1;
        muxCout.lock();
        std::cout << "ERROR! GeoTessellatedSolid number of facets: " << sh->getNumberOfFacets() << " is not equal to the original size! --> ";
        printStdVectorStrings(shapePars);
        muxCout.unlock();
			}
			/*
			 * TODO: uncomment it, when the isValid() method will be implemented for GeoTessellatedSolid
			if(!sh->isValid()) {
				error = 1;
				std::cout << "ERROR! GeoTessellatedSolid shape is not valid!! -- input: " << shapePars;
			}
			*/
		} // END OF if (size>13)
		else {
      muxCout.lock();
      std::cout << "ERROR!! GeoTessellatedSolid has no facets!! --> shape input parameters: ";
      printStdVectorStrings(shapePars);
      muxCout.unlock();
			error = 1;
		}
		if(error) {
      muxCout.lock();
      std::cout << "GeoTessellatedSolid shape error!!! Aborting..." << std::endl;
      muxCout.unlock();
      exit(EXIT_FAILURE);
    }
		shape = sh;

	}
	else if (type == "Trap") {
			// shape constructor parameters
			double ZHalfLength = 0.;
			double Theta = 0.;
			double Phi = 0.;
			double Dydzn = 0.;
			double Dxdyndzn = 0.;
			double Dxdypdzn = 0.;
			double Angleydzn = 0.;
			double Dydzp = 0.;
			double Dxdyndzp = 0.;
			double Dxdypdzp = 0.;
			double Angleydzp = 0.;
			// get parameters
    for( auto& par : shapePars) {
      std::vector<std::string> vars = splitString(par, '=');
      std::string varName = vars[0];
      std::string varValue = vars[1];
					if (varName == "ZHalfLength") ZHalfLength = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
					if (varName == "Theta")       Theta = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
					if (varName == "Phi")         Phi = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
					if (varName == "Dydzn")       Dydzn = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
					if (varName == "Dxdyndzn")    Dxdyndzn = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
					if (varName == "Dxdypdzn")    Dxdypdzn = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
					if (varName == "Angleydzn")   Angleydzn = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
					if (varName == "Dydzp")       Dydzp = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
					if (varName == "Dxdyndzp")    Dxdyndzp = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
					if (varName == "Dxdypdzp")    Dxdypdzp = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
					if (varName == "Angleydzp")   Angleydzp = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
			}
		shape = new GeoTrap (ZHalfLength, Theta, Phi, Dydzn, Dxdyndzn, Dxdypdzn, Angleydzn, Dydzp, Dxdyndzp, Dxdypdzp, Angleydzp);
	}
	else if (type == "Trd") {
			// shape constructor parameters
			double XHalfLength1 = 0.;
			double XHalfLength2 = 0.;
			double YHalfLength1 = 0.;
			double YHalfLength2 = 0.;
			double ZHalfLength = 0.;
			// get parameters
    for( auto& par : shapePars) {
      std::vector<std::string> vars = splitString(par, '=');
      std::string varName = vars[0];
      std::string varValue = vars[1];
          // std::cout << "varValue:" << varValue;
					if (varName == "XHalfLength1") XHalfLength1 = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
					if (varName == "XHalfLength2") XHalfLength2 = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
					if (varName == "YHalfLength1") YHalfLength1 = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
					if (varName == "YHalfLength2") YHalfLength2 = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
					if (varName == "ZHalfLength")  ZHalfLength = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
			}
		shape = new GeoTrd (XHalfLength1, XHalfLength2, YHalfLength1, YHalfLength2, ZHalfLength);
	}
	else if (type == "Tube") {
		// shape parameters
		double RMin = 0.;
		double RMax = 0.;
		double ZHalfLength = 0.;
		// get parameters
    for( auto& par : shapePars) {
      std::vector<std::string> vars = splitString(par, '=');
      std::string varName = vars[0];
      std::string varValue = vars[1];
			if (varName == "RMin")        RMin = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
			if (varName == "RMax")        RMax = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
			if (varName == "ZHalfLength") ZHalfLength = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
		}
		shape = new GeoTube(RMin, RMax, ZHalfLength);
	}
	else if (type == "Tubs") {
		// shape parameters
		double RMin = 0.;
		double RMax = 0.;
		double ZHalfLength = 0.;
		double SPhi = 0.;
		double DPhi = 0.;
		// get parameters
    for( auto& par : shapePars) {
      std::vector<std::string> vars = splitString(par, '=');
      std::string varName = vars[0];
      std::string varValue = vars[1];
			if (varName == "RMin")        RMin = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
			if (varName == "RMax")        RMax = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
			if (varName == "ZHalfLength") ZHalfLength = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
			if (varName == "SPhi")        SPhi = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
			if (varName == "DPhi")        DPhi = std::stod(varValue);// * SYSTEM_OF_UNITS::mm;
		}
		shape = new GeoTubs (RMin, RMax, ZHalfLength, SPhi, DPhi);
	}
	else if (type == "Shift") {
		// shape parameters
		unsigned int shapeOpId = 0;
		unsigned int transfId = 0;
		// get parameters
    for( auto& par : shapePars) {
      std::vector<std::string> vars = splitString(par, '=');
      std::string varName = vars[0];
      std::string varValue = vars[1];
			if (varName == "A") shapeOpId = std::stoi(varValue);
			if (varName == "X") transfId = std::stoi(varValue);
		}
    if (shapeOpId == 0 || transfId == 0) {
      std::cout << "ERROR! Shift shape - input operand shapes' IDs are empty! (shapeId: " << shapeOpId << ", transfId:" << transfId << ")" << std::endl;
      exit(EXIT_FAILURE);
    }

    // if both operands are built already,
    // then get them from cache,
    // and build the operator shape with them,
    if ( isBuiltShape(shapeOpId) && isBuiltTransform(transfId) ) {
        const GeoShape* shapeOp = getBuiltShape(shapeOpId);
        const GeoTransform* transf = getBuiltTransform(transfId);
        // TODO: here we create a fake GeoTransform to get a GeoTrf::Transform3D.
        // TODO: ==> Perhaps we could keep a table for bare GeoTrf::Transform3D transforms used in GeoShift nodes.
        GeoTrf::Transform3D transfX = transf->getTransform();
        transf->unref(); // delete the transf from the heap, because we don't need the node, only the bare transformation matrix
        GeoShapeShift* shapeNew = new GeoShapeShift(shapeOp, transfX);
        storeBuiltShape(shapeId, shapeNew);
        shape = shapeNew;
    }
    // otherwise, build the operands
    else {

      // TODO: IMPORTANT!!! --> check how the transf used in shift are saved into the DB, because they are bare transf and not GeoTransform nodes...

      // first, check if the transform is built;
      // if so, use that;
      // if not, build the transform

      // get the referenced Transform, then get the bare transform matrix from it
      GeoTransform* transf = nullptr;
      GeoTrf::Transform3D transfX;
      if (isBuiltTransform(transfId)) {
        transf = getBuiltTransform(transfId);
      } else {
        transf = buildTransform(transfId);
      }
      // TODO: here we create a fake GeoTransform to get a GeoTrf::Transform3D.
      // TODO: ==> Perhaps we could keep a table for bare GeoTrf::Transform3D transforms used in GeoShift nodes.
      transfX = transf->getTransform();
      transf->unref(); // delete the transf from the heap, because we don't need the node, only the bare transformation matrix

      // then, check the type of the operand shape
      bool isAOperator = isShapeOperator( shapeOpId );

      // if operand shape is simple/actual shape (i.e., not boolean/operator),
      // then build it,
      // then build the boolean shape with that
      if ( !isAOperator ) {
        const GeoShape* shapeOp = buildShape( shapeOpId, shapes_info_sub );

        if ( shapeOp == nullptr || transf == nullptr ) {
          std::cout << "ERROR!!! Shift - shapeOp or transfX are NULL! Exiting..." << std::endl;
          exit(EXIT_FAILURE);
        }
        GeoShapeShift* shapeNew = new GeoShapeShift(shapeOp, transfX);
        shape = shapeNew;
      }
      // ...otherwise, build the Shift operator shape without operands
      // and save the needed pieces of information to build the actual
      // operands and shape later.
      else {
        GeoShapeShift* shapeNew = new GeoShapeShift();
        tuple_shapes_boolean_info tt (shapeId, shapeNew, shapeOpId, transfId);
        shapes_info_sub->push_back(tt); //! Push the information about the new boolean shape at the end of the very same container we are iterating over
        shape = shapeNew;
      }
    }
	}
	else if (type == "Subtraction" || type == "Union" || type == "Intersection") {

    // Check what shapes are subtracted/united/intersected:
    // - If they are actual shapes, build them and return
    // - If they are boolean/operator shapes, then store the shape for later and continue

		// shape's operands
		unsigned int opA = 0;
		unsigned int opB = 0;
		// get parameters
    for( auto& par : shapePars) {
      std::vector<std::string> vars = splitString(par, '=');
      std::string varName = vars[0];
      std::string varValue = vars[1];
			if (varName == "opA") opA = std::stoi(varValue);
			if (varName == "opB") opB = std::stoi(varValue);
		}
    if (opA == 0 || opB == 0) {
      std::cout << "ERROR! Subtraction/Union/Intersection shape - input operand shapes' IDs are empty! (opA: " << opA << ", opB:" << opB << ")" << std::endl;
      exit(EXIT_FAILURE);
    }

    // if both operands are built already,
    // then get them from cache,
    // and build the operator with them
    if ( isBuiltShape(opA) && isBuiltShape(opB) ) {
        // std::cout << "both operand shapes are built, build the operator shape..." << std::endl;
        GeoShape* shapeNew = nullptr;
        const GeoShape* shapeA = getBuiltShape(opA);
        const GeoShape* shapeB = getBuiltShape(opB);
        if ("Subtraction" == type) {
          shapeNew = new GeoShapeSubtraction(shapeA, shapeB);
        }
        else if ("Union" == type) {
          shapeNew = new GeoShapeUnion(shapeA, shapeB);
        }
        else if ("Intersection" == type) {
          shapeNew = new GeoShapeIntersection(shapeA, shapeB);
        }
        shape = shapeNew;
    }
    // otherwise, build the operand shapes...
    else {

      // first check the operands' type
      bool isAOperator = isShapeOperator( opA );
      bool isBOperator = isShapeOperator( opB );

      // if both are simple/actual shapes (i.e., not booleans),
      // then build them, then build the boolean shape with them, and store that.
      if ( !isAOperator && !isBOperator) {
        const GeoShape* shapeA = buildShape( opA, shapes_info_sub );
        const GeoShape* shapeB = buildShape( opB, shapes_info_sub );
        if ( shapeA == NULL || shapeB == NULL ) {
          std::cout << "ERROR!!! shapeA or shapeB are NULL!" << std::endl;
          exit(EXIT_FAILURE);
        }
        
        GeoShape* shapeNew = nullptr;
        if ("Subtraction" == type) {
          shapeNew = new GeoShapeSubtraction(shapeA, shapeB);
        }
        else if ("Union" == type) {
          shapeNew = new GeoShapeUnion(shapeA, shapeB);
        }
        else if ("Intersection" == type) {
          shapeNew = new GeoShapeIntersection(shapeA, shapeB);
        }
        
        shape = shapeNew;
      }
      // ...otherwise, build the Subtraction operator shape without operands
      // and save the needed pieces of information to build the actual
      // operands and shape later.
      else {
        GeoShape* shapeNew = nullptr;
        if ( "Subtraction" == type ) {
          shapeNew = new GeoShapeSubtraction;
        } else if ( "Union" == type ) {
          shapeNew = new GeoShapeUnion;
        } else if ( "Intersection" == type ) {
          shapeNew = new GeoShapeIntersection;
        }

        tuple_shapes_boolean_info tt (shapeId, shapeNew, opA, opB);
        shapes_info_sub->push_back(tt); //! Push the information about the new boolean shape at the end of the very same container we are iterating over

        shape = shapeNew;
      }
    }
	}
  //LAr custom shape
  else if(type == "CustomShape") {
    std::string name = "";
    // check parameters
    // this complex test is needed to handle null strings in the DB records
    bool okPars = false;
    for( auto& par : shapePars) {
    std::string str = par.substr( par.find("=") );     // get from "=" to the end
      if (str.size() > 0) okPars = true;
    }
  	//if ( shapePars.size() > 0 && ((shapePars.filter("=")).size() > 0) )  // this complex test is needed to handle null strings
    if ( shapePars.size() > 0 && okPars )
  	{
      // get parameters
      for( auto& par : shapePars) {
        std::vector<std::string> vars = splitString(par, '=');
        std::string varName = vars[0];
        std::string varValue = vars[1];
  				if (varName == "name") name = varValue;
  			}
  	} else {
      muxCout.lock();
      // throw std::invalid_argument("CustomShape parameters' list is empty!!");
      std::cout << "ERROR!!! --> CustomShape parameters' list is empty!! It seems the geometry file you are running on is corrupted." << std::endl;
      muxCout.unlock();
  		exit(EXIT_FAILURE);
  	}
      shape = new GeoUnidentifiedShape("LArCustomShape",name);
  }
  else if (type=="UnidentifiedShape") {
    std::string name = "";
    std::string asciiData = "";
    // check parameters
    // this complex test is needed to handle null strings in the DB records
    bool okPars = false;
    for( auto& par : shapePars) {
      std::string str = par.substr( par.find("=") );     // get from "=" to the end
      if (str.size() > 0) okPars = true;
    }
    //if ( shapePars.size() > 0 && ((shapePars.filter("=")).size() > 0) )  // this complex test is needed to handle null strings
    if ( shapePars.size() > 0 && okPars )
  	{
      // get parameters
      for( auto& par : shapePars) {
        std::vector<std::string> vars = splitString(par, '=');
        std::string varName = vars[0];
        std::string varValue = vars[1];
        if (varName == "name")      name = varValue;
        if (varName == "asciiData") asciiData = varValue;
      }
    } else {
      // throw std::invalid_argument("UnidentifiedShape parameters' list is empty!!");
      muxCout.lock();
      std::cout << "ERROR!!! --> UnidentifiedShape parameters' list is empty!! It seems the geometry file you are running on is corrupted." << std::endl;
      muxCout.unlock();
      exit(EXIT_FAILURE);
    }
    shape = new GeoUnidentifiedShape(name,asciiData);

  }
  else {
    m_unknown_shapes.insert(type); // save unknwon shapes for later warning message
    shape = buildDummyShape();
  }

  //! store into the cache the shape we have just built,
  //! for later use when referenced by another node
  storeBuiltShape(shapeId, shape);

  return shape; // FIXME: do we still need the return? probably not, because we now store all the shapes that have been built in this method

// }

// catch (std::invalid_argument& e)
// {
//     std::cerr << e.what() << std::endl;
//     exit(EXIT_FAILURE);
// }


}



// TODO: move to an untilities file/class
void printTuple(tuple_shapes_boolean_info tuple)
{
  std::apply([](auto&&... args) { ( (std::cout << args << ", "), ...); }, tuple); // needs C++17
  std::cout << std::endl;
}
// TODO: move to an untilities file/class
void inspectListShapesToBuild(type_shapes_boolean_info list)
{
  for (auto tuple : list) {
    printTuple(tuple);
    std::cout << std::endl;
  }
}


void ReadGeoModel::createBooleanShapeOperands(type_shapes_boolean_info* shapes_info_sub)
{
  if (shapes_info_sub->size() == 0) return;

  // debug
  // std::cout << "\ncreateBooleanShapeOperands() - start..." << std::endl;
  // inspectListShapesToBuild(shapes_info_sub);

	// Iterate over the list. The size may be incremented while iterating (therefore, we cannot use iterators)
  for (type_shapes_boolean_info::size_type ii = 0; ii < shapes_info_sub->size(); ++ii)
  {
    // get the tuple containing the data about the operand shapes to build
    tuple_shapes_boolean_info tuple = (*shapes_info_sub)[ii];
    // std::cout << "tuple: "; printTuple(tuple); // debug

      // Initializing variables for unpacking
      unsigned int shapeID = 0;       //std::get<0>(tuple);
      GeoShape* boolShPtr  = nullptr; //std::get<1>(tuple);
      unsigned int idA     = 0;       //std::get<2>(tuple);
      unsigned int idB     = 0;       //std::get<3>(tuple);

      // use 'tie' to unpack the tuple values into separate variables
      std::tie(shapeID, boolShPtr, idA, idB) = tuple;

      if (shapeID == 0 || boolShPtr == nullptr || idA == 0 || idB == 0) {
        muxCout.lock();
        std::cout << "ERROR! Boolean/Operator shape - shape is NULL or operands' IDs are not defined! (shapeID: " << shapeID << ", idA: " << idA << ", idB:" << idB << ")" << std::endl;
        muxCout.unlock();
        exit(EXIT_FAILURE);
      }

      if (isShapeBoolean(shapeID)) {

        GeoShape* shapeA = nullptr;
        GeoShape* shapeB = nullptr;

        // if both operands are built already...
        if ( isBuiltShape(idA) && isBuiltShape(idB) ) {
          // then build the operator shape...
          shapeA = getBuiltShape(idA);
          shapeB = getBuiltShape(idB); //TODO: customize for Shift as well
        } else {
          // otherwise, build the operand shapes
          shapeA = getBooleanReferencedShape(idA, shapes_info_sub);
          shapeB = getBooleanReferencedShape(idB, shapes_info_sub);
        }
        // Now, assign the new shapes to the boolean shape we're building
        if (dynamic_cast<GeoShapeIntersection*>(boolShPtr)) {
          GeoShapeIntersection* ptr = dynamic_cast<GeoShapeIntersection*>(boolShPtr);
          ptr->m_opA = shapeA;
          ptr->m_opB = shapeB;
          ptr->m_opA->ref();
          ptr->m_opB->ref();
        }
        else if (dynamic_cast<GeoShapeSubtraction*>(boolShPtr)) {
          GeoShapeSubtraction* ptr = dynamic_cast<GeoShapeSubtraction*>(boolShPtr);
          ptr->m_opA = shapeA;
          ptr->m_opB = shapeB;
          ptr->m_opA->ref();
          ptr->m_opB->ref();
        }
        else if (dynamic_cast<GeoShapeUnion*>(boolShPtr)) {
          GeoShapeUnion* ptr = dynamic_cast<GeoShapeUnion*>(boolShPtr);
          ptr->m_opA = shapeA;
          ptr->m_opB = shapeB;
          ptr->m_opA->ref();
          ptr->m_opB->ref();
        }
        else{
          // TODO: move to standard error message for all instances
          std::cout << "ERROR!!! shape is not boolean/operator! Write to 'geomodel-developers@cern.ch'. Exiting..." << std::endl;
          exit(EXIT_FAILURE);
        }
      } else if ("Shift" == getShapeType(shapeID)) {

        GeoShape* opShape = nullptr;
        GeoTrf::Transform3D shiftX;
        GeoTransform* shiftTransf = nullptr; // TODO: remove the need for a temp GeoTransform, store the bare transforms as well...

        // if both operands are built already...
        if ( isBuiltShape(idA) && isBuiltTransform(idB) ) {
          // then build the operator shape...
          opShape = getBuiltShape(idA);
          shiftTransf = getBuiltTransform(idB);
        } else {
          // otherwise, build the operand shapes
          opShape = getBooleanReferencedShape(idA, shapes_info_sub);
          shiftTransf = buildTransform(idB);
        }
        shiftX = shiftTransf->getTransform();
        shiftTransf->unref(); // delete from heap, we only needed to get the bare transform // TODO: remove that need, store the bare transforms as well...

        if (dynamic_cast<GeoShapeShift*>(boolShPtr)) {
        GeoShapeShift* ptr = dynamic_cast<GeoShapeShift*>(boolShPtr);
        ptr->m_op = opShape;
        ptr->m_shift = shiftX;
        ptr->m_op->ref();
        }
        else {
          std::cout << "ERROR!!! shape is not a Shift operator! Exiting..." << std::endl;
          exit(EXIT_FAILURE);
        }
      } else {
        std::cout << "ERROR! Undefined operator shape! This part of the code should not be reached! Exiting..." << std::endl;
        exit(EXIT_FAILURE);
      }
    // then, store the now completed shape and continue to the next item
    storeBuiltShape(shapeID, boolShPtr);
	}
}



GeoShape* ReadGeoModel::getBooleanReferencedShape(const unsigned int shapeID, type_shapes_boolean_info* shapes_info_sub)
{
  if (0 == shapeID) {
    std::cout << "ERROR!! ShapeID = 0!" << std::endl;
    exit(EXIT_FAILURE);
  }

  GeoShape* shape;
  // if A is built already, then take it from cache
  if (isBuiltShape(shapeID)) {
    if (m_deepDebug) std::cout << "operandA is built, taking it from cache..." << std::endl; // debug
    shape = getBuiltShape(shapeID);
  } else {
    // if not built and not a boolean shape, then build it
    if (!isShapeOperator(shapeID)) {
      if (m_deepDebug) std::cout << "operandA is not built and not an operator, build it..." << std::endl; // debug
      shape = buildShape( shapeID, shapes_info_sub );
      if ( shape == NULL ) {
        std::cout << "ERROR!!! shape is NULL!" << std::endl;
        exit(EXIT_FAILURE);
      }
    }
    // if A is a boolean shape, then create an empty shape,
    // store it for later completion, and use that
    else {
      if (m_deepDebug) std::cout << "operandA is not built and it is an operator, add it to build it later..." << std::endl; // debug
      shape = addEmptyBooleanShapeForCompletion(shapeID, shapes_info_sub);
      }
  }

//   inspectListShapesToBuild(m_shapes_info_sub); // debug
  return shape;
}



GeoShape* ReadGeoModel::addEmptyBooleanShapeForCompletion(const unsigned int shapeID, type_shapes_boolean_info* shapes_info_sub)
{
  // get the operands' IDs,
  // build an empty instance of the appropriate boolean/operator shape,
  // and store all of that together, by appending to this same container,
  // so it will be visited at a later time during this very same loop
  std::pair<unsigned int, unsigned int> ops = getBooleanShapeOperands(shapeID);
  unsigned int opA = ops.first;
  unsigned int opB = ops.second;

  // get the type and create an appropriate empty shape
  std::string shType = getShapeType(shapeID);
  GeoShape* shape = nullptr;
  if (shType == "Intersection"){
    shape = new GeoShapeIntersection();
  } else if (shType == "Shift") {
    shape = new GeoShapeShift();
  } else if (shType == "Subtraction") {
    shape = new GeoShapeSubtraction();
  } else if (shType == "Union") {
    shape = new GeoShapeUnion();
  }

  tuple_shapes_boolean_info tt (shapeID, shape, opA, opB);
  shapes_info_sub->push_back(tt); //! Push the information about the new boolean shape at the end of the very same container we are iterating over

  return shape;
}


// TODO: move this to utility class/file
std::vector<std::string> ReadGeoModel::splitString(const std::string& s, const char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

//  // TODO: move this to utility class/file
//QStringList ReadGeoModel::toQStringList(std::vector<std::string> vec)
//  {
//    QStringList ll;
//    for ( auto& str : vec ) {
//      ll << QString::fromStdString(str);
//    }
//    return ll;
//  }

//// TODO: move this to utility class/file
//std::vector<std::string> ReadGeoModel::toStdVectorStrings(QStringList qlist)
//{
//  std::vector<std::string> vec;
//  foreach(QString qstr, qlist) {
//    vec.push_back(qstr.toStdString());
//  }
//  return vec;
//}
  
  
  
// TODO: move this to utility class/file
void ReadGeoModel::printStdVectorStrings(std::vector<std::string> vec)
{
  for ( const auto& str : vec) {
    std::cout << str << " ";
  }
  std::cout << std::endl;
  return;
}


std::pair<unsigned int, unsigned int> ReadGeoModel::getBooleanShapeOperands(const unsigned int shapeID)
{
  std::pair<unsigned int, unsigned int> pair;

//  std::vector<std::string> paramsShape = toStdVectorStrings(m_shapes[ shapeID ]);
  std::vector<std::string> paramsShape = m_shapes[ shapeID-1 ];// remember: shapes' IDs start from 1
  
//  unsigned int id = std::stoi(paramsShape[0]); //! the ID of the boolean/operator shape
	std::string type = paramsShape[1]; //! the GeoModel type of the shape
	std::string parameters = paramsShape[2];  //! the parameters defining the shape, coming from the DB

  //! The Subtraction boolean shape has two operands, here we store their IDs
  unsigned int opA = 0;
  unsigned int opB = 0;
  // get parameters from DB string
  std::vector<std::string> shapePars = splitString( parameters, ';' );
  // std::cout << "shapePars size: " << shapePars.size() << std::endl; // debug only

  for (auto par : shapePars) {
    std::vector<std::string> vars = splitString(par, '=');
    std::string varName = vars[0];
    std::string varValue = vars[1];
    // std::cout << "vars ==> " << vars[0] << ", " << vars[1] << std::endl; // debug
    if ( isShapeBoolean(type) ) {
      if (varName == "opA") opA = std::stoi(varValue);
      if (varName == "opB") opB = std::stoi(varValue);
    } else if ("Shift" == type) {
      if (varName == "A") opA = std::stoi(varValue);
      if (varName == "X") opB = std::stoi(varValue);
    } else {
      std::cout << "ERROR!!! This type of shape cannot be handled here: " << type << ", --> Exiting..." << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  if (opA == 0 || opB == 0) {
    std::cout << "ERROR! Intersection/Subtraction/Union/Shift shape - operands' IDs are empty! (opA: " << opA << ", opB:" << opB << ")" << std::endl;
    exit(EXIT_FAILURE);
  }
  pair.first = opA;
  pair.second = opB;

  return pair;
}


bool ReadGeoModel::isShapeOperator(const unsigned int shapeId)
{
  return isShapeOperator( getShapeType(shapeId) );
}
bool ReadGeoModel::isShapeOperator(const std::string type)
{
  std::unordered_set<std::string> opShapes = {"Intersection", "Shift", "Subtraction", "Union"};
  return (opShapes.find(type) != opShapes.end());
}

bool ReadGeoModel::isShapeBoolean(const unsigned int shapeId)
{
  return isShapeBoolean( getShapeType(shapeId) );
}
bool ReadGeoModel::isShapeBoolean(const std::string type)
{
  std::unordered_set<std::string> opShapes = {"Intersection", "Subtraction", "Union"};
  return (opShapes.find(type) != opShapes.end());
}




GeoBox* ReadGeoModel::buildDummyShape() {
  return new GeoBox(30.0*SYSTEM_OF_UNITS::cm, 30*SYSTEM_OF_UNITS::cm, 30*SYSTEM_OF_UNITS::cm);
}


GeoLogVol* ReadGeoModel::buildLogVol(const unsigned int id)
{

  if (isBuiltLog(id)) {
    return getBuiltLog(id);
  }

	if (m_deepDebug) {
    muxCout.lock();
    std::cout << "ReadGeoModel::buildLogVol()" << std::endl;
    muxCout.unlock();
  }

	// get logVol properties from the DB
  std::vector<std::string> values = m_logVols[id-1];

	// get the parameters to build the GeoLogVol node
  std::string logVolName = values[1];

	// build the referenced GeoShape node
  const unsigned int shapeId = std::stoi(values[2]);
  GeoShape* shape = getBuiltShape(shapeId);
  if(!shape) {
    std::cout << "ERROR!! While building a LogVol, Shape is NULL! Exiting..." <<std::endl;
    exit(EXIT_FAILURE);
  }

	// build the referenced GeoMaterial node
  const unsigned int matId = std::stoi(values[3]);
	if (m_deepDebug) {
    muxCout.lock();
    std::cout << "buildLogVol() - material Id:" << matId << std::endl;
    muxCout.unlock();
  }
  GeoMaterial* mat = getBuiltMaterial(matId);
  if(!mat) {
    std::cout << "ERROR!! While building a LogVol, Material is NULL! Exiting..." <<std::endl;
    exit(EXIT_FAILURE);
  }
  
	GeoLogVol* logPtr = new GeoLogVol(logVolName, shape, mat);
  storeBuiltLog(id, logPtr);
	return logPtr;
}


// TODO: this should be moved to an Utilities class!
void ReadGeoModel::printTrf(GeoTrf::Transform3D t) {
  muxCout.lock();
	std::cout << "transformation: " << std::endl;
	std::cout << "[[" << t(0, 0) << " , ";
	std::cout <<         t(0, 1) << " , ";
	std::cout <<         t(0, 2) << " , ";
	std::cout <<         t(0, 3) << " ]";
	std::cout << "["  << t(1, 0) << " , ";
	std::cout <<         t(1, 1) << " , ";
	std::cout <<         t(1, 2) << " , ";
	std::cout <<         t(1, 3) << " ]";
	std::cout << "["  << t(2, 0) << " , ";
	std::cout <<         t(2, 1) << " , ";
	std::cout <<         t(2, 2) << " , ";
	std::cout <<         t(2, 3) << " ]";
	std::cout << "["  << t(3, 0) << " , ";
	std::cout <<         t(3, 1) << " , ";
	std::cout <<         t(3, 2) << " , ";
	std::cout <<         t(3, 3) << " ]]" << std::endl;
  muxCout.unlock();
}

//// TODO: should go in a QtUtils header-only class, to be used in other packages
//QList<double> ReadGeoModel::convertQstringListToDouble(QStringList listin) {
//  QList<double> listout;
//  foreach (const QString &s, listin) {
//      listout.append(s.toDouble());
//  }
//  return listout;
//}

//// TODO: move it to an utility class
//void ReadGeoModel::printTransformationValues(QStringList values) {
//  QList<double> t = convertQstringListToDouble(values);
//  muxCout.lock();
//  std::cout << "transformation input values: " << std::endl;
//  std::cout << "[[" << t[0] << "," << t[1] << "," << t[2] << "]["
//                     << t[3] << "," << t[4] << "," << t[5] << "]["
//                     << t[6] << "," << t[7] << "," << t[8] << "]["
//                     << t[9] << "," << t[10] << "," << t[11] << "]]" << std::endl;
//  muxCout.unlock();
//}


GeoAlignableTransform* ReadGeoModel::buildAlignableTransform(const unsigned int id)
{
  if (isBuiltAlignableTransform(id)) {
    return getBuiltAlignableTransform(id);
  }
  
  std::vector<std::string> values = m_alignableTransforms[id-1]; // nodes' IDs start from 1
  values.erase(values.begin()); // remove the first element, that is the 'id', leaving the other items in the list

	// get the 12 matrix elements
  double xx = std::stod(values[0]);
	double xy = std::stod(values[1]);
	double xz = std::stod(values[2]);

	double yx = std::stod(values[3]);
	double yy = std::stod(values[4]);
	double yz = std::stod(values[5]);

	double zx = std::stod(values[6]);
	double zy = std::stod(values[7]);
	double zz = std::stod(values[8]);

	double dx = std::stod(values[9]);
	double dy = std::stod(values[10]);
	double dz = std::stod(values[11]);

	GeoTrf::Transform3D txf;
	// build the rotation matrix with the first 9 elements
	txf(0,0)=xx;
	txf(0,1)=xy;
	txf(0,2)=xz;

	txf(1,0)=yx;
	txf(1,1)=yy;
	txf(1,2)=yz;

	txf(2,0)=zx;
	txf(2,1)=zy;
	txf(2,2)=zz;

	// build the translation matrix with the last 3 elements
	txf(0,3)=dx;
	txf(1,3)=dy;
	txf(2,3)=dz;

	// printTrf(txf); // DEBUG
  GeoAlignableTransform* tr = new GeoAlignableTransform(txf);
  storeBuiltAlignableTransform(id, tr);
  return tr;
}


GeoTransform* ReadGeoModel::buildTransform(const unsigned int id)
{
  if (isBuiltTransform(id)) {
    return getBuiltTransform(id);
  }
  
  std::vector<std::string> values = m_transforms[id-1]; // nodes' IDs start from 1
  values.erase(values.begin()); // remove the first element, that is the 'id', leaving the other items in the list
  
  // get the 12 matrix elements
  double xx = std::stod(values[0]);
  double xy = std::stod(values[1]);
  double xz = std::stod(values[2]);
  
  double yx = std::stod(values[3]);
  double yy = std::stod(values[4]);
  double yz = std::stod(values[5]);
  
  double zx = std::stod(values[6]);
  double zy = std::stod(values[7]);
  double zz = std::stod(values[8]);
  
  double dx = std::stod(values[9]);
  double dy = std::stod(values[10]);
  double dz = std::stod(values[11]);
  
	GeoTrf::Transform3D txf;
	// build the rotation matrix with the first 9 elements
	txf(0,0)=xx;
	txf(0,1)=xy;
	txf(0,2)=xz;

	txf(1,0)=yx;
	txf(1,1)=yy;
	txf(1,2)=yz;

	txf(2,0)=zx;
	txf(2,1)=zy;
	txf(2,2)=zz;

	// build the translation matrix with the last 3 elements
	txf(0,3) = dx;
	txf(1,3) = dy;
	txf(2,3) = dz;

	// printTrf(txf); // DEBUG
	GeoTransform* tr = new GeoTransform(txf);
  storeBuiltTransform(id, tr);
  return tr;
}


GeoSerialTransformer* ReadGeoModel::buildSerialTransformer(const unsigned int id)
{
  muxCout.lock();
	if (m_deepDebug) std::cout << "ReadGeoModel::buildSerialTransformer()" << std::endl;
  muxCout.unlock();

  const unsigned int nodeID = id-1; // nodes' IDs start from 1
  std::vector<std::string> values = m_serialTransformers[nodeID];

  const unsigned int functionId = std::stoi(values[1]);
  const unsigned int physVolId = std::stoi(values[2]);
  const unsigned int physVolTableId = std::stoi(values[3]);
  const unsigned int copies = std::stoi(values[4]);

	// GET THE REFERENCED FUNCTION
	TRANSFUNCTION func = buildFunction(functionId);

	// GET THE REFERENCED VPHYSVOL
	const GeoVPhysVol* vphysVol = buildVPhysVolInstance(physVolId, physVolTableId, 1); // we use "1" as default copyNumber: taking the first copy of the VPhysVol as the referenced volume

	// get PhysVol or FullPhysVol pointer and return the SerialTransformer
	if (dynamic_cast<const GeoFullPhysVol*>(vphysVol)) {
		const GeoFullPhysVol* vol = dynamic_cast<const GeoFullPhysVol*>(vphysVol);
		GeoSerialTransformer* nodePtr = new GeoSerialTransformer(vol, &func, copies );
    storeBuiltSerialTransformer(id, nodePtr);
    return nodePtr;
	}
	const GeoPhysVol* vol = dynamic_cast<const GeoPhysVol*>(vphysVol);
  GeoSerialTransformer* nodePtr = new GeoSerialTransformer(vol, &func, copies );
  storeBuiltSerialTransformer(id, nodePtr);
  return nodePtr;
}

TRANSFUNCTION ReadGeoModel::buildFunction(const unsigned int id)
{
  /* FIXME: see below
  if( isBuiltFunction(id) ) {
   GeoXF::Function* fPtr = getBuiltFunction(id);
   TRANSFUNCTION tf = (*fPtr); // Remember: "typedef const Function & TRANSFUNCTION"
   return tf;
  }
   */

  std::string expr = m_functions[id-1][1]; // nodes' IDs start from 1
  
	if (0==expr.size()) {
    muxCout.lock();
    std::cout << "FATAL ERROR!! Function expression is empty!! Aborting..." << std::endl;
    muxCout.unlock();
    exit(EXIT_FAILURE);
	}

  TransFunctionInterpreter interpreter;
	TFPTR func=interpreter.interpret( expr );
	TRANSFUNCTION tf = *(func.release()); // make func returns a pointer to the managed object and releases the ownership, then get the object dereferencing the pointer
  
  /* FIXME: At the moment, enabling this cache makes the app crash at the end, when calling the destructor. I suspect because the pointers are not correct and so removing them throws an error.
  // Get a non-const pointer to the Function object,
  // and store that into the cache.
  // Remember: "typedef const Function & TRANSFUNCTION"
  const GeoXF::Function* fPtrConst = &tf;
  GeoXF::Function* fPtr = const_cast <GeoXF::Function*>(fPtrConst);
  storeBuiltFunction(id, fPtr);
   */
  
  return tf;
}


// --- methods for caching GeoShape nodes ---
bool ReadGeoModel::isBuiltShape(const unsigned int id)
{
  return ( ! (m_memMapShapes.find(id) == m_memMapShapes.end()) );
}
void ReadGeoModel::storeBuiltShape(const unsigned int id, GeoShape* nodePtr)
{
  m_memMapShapes[id] = nodePtr;
}
GeoShape* ReadGeoModel::getBuiltShape(const unsigned int id)
{
  return m_memMapShapes[id]; // this is a map, and 'id' is the key
}
  
// --- methods for caching GeoLogVol nodes ---
bool ReadGeoModel::isBuiltLog(const unsigned int id)
{
  return (id <= m_memMapLogVols.size()); // vector: we exploit the fact that we built the vols ordered by their IDs
}
void ReadGeoModel::storeBuiltLog(const unsigned int id, GeoLogVol* nodePtr)
{
  m_memMapLogVols.push_back(nodePtr); 
}
GeoLogVol* ReadGeoModel::getBuiltLog(const unsigned int id)
{
	return m_memMapLogVols[id-1]; // nodes' IDs start from 1
}
  
// --- methods for caching GeoPhysVol nodes ---
bool ReadGeoModel::isBuiltPhysVol(const unsigned int id)
{
  return (id <= m_memMapPhysVols.size()); // vector: we exploit the fact that we built the vols ordered by their IDs
}
void ReadGeoModel::storeBuiltPhysVol(const unsigned int id, GeoPhysVol* nodePtr)
{
  m_memMapPhysVols.push_back(nodePtr);
}
GeoPhysVol* ReadGeoModel::getBuiltPhysVol(const unsigned int id)
{
	return m_memMapPhysVols[id-1]; // nodes' IDs start from 1
}
  
// --- methods for caching GeoFullPhysVol nodes ---
bool ReadGeoModel::isBuiltFullPhysVol(const unsigned int id)
{
  return (id <= m_memMapFullPhysVols.size()); // vector: we exploit the fact that we built the vols ordered by their IDs

}
void ReadGeoModel::storeBuiltFullPhysVol(const unsigned int id, GeoFullPhysVol* nodePtr)
{
  m_memMapFullPhysVols.push_back(nodePtr); // vector, we store them in the order of IDs
}
GeoFullPhysVol* ReadGeoModel::getBuiltFullPhysVol(const unsigned int id)
{
  return m_memMapFullPhysVols[id-1]; // nodes' IDs start from 1

}
  
// --- methods for caching GeoMaterial nodes ---
bool ReadGeoModel::isBuiltMaterial(const unsigned int id)
{
  return (id <= m_memMapMaterials.size());
}
void ReadGeoModel::storeBuiltMaterial(const unsigned int id, GeoMaterial* nodePtr)
{
  m_memMapMaterials.push_back(nodePtr);
}
GeoMaterial* ReadGeoModel::getBuiltMaterial(const unsigned int id)
{
	return m_memMapMaterials[id-1];
}
  
// --- methods for caching GeoElement nodes ---
bool ReadGeoModel::isBuiltElement(const unsigned int id)
{
  return (id <= m_memMapElements.size());
}
void ReadGeoModel::storeBuiltElement(const unsigned int id, GeoElement* nodePtr)
{
  m_memMapElements.push_back(nodePtr);
}
GeoElement* ReadGeoModel::getBuiltElement(const unsigned int id)
{
	return m_memMapElements[id-1];
}
  
// --- methods for caching GeoTransform nodes ---
bool ReadGeoModel::isBuiltTransform(const unsigned int id)
{
  return (id <= m_memMapTransforms.size()); // vector: we exploit the fact that we built the vols ordered by their IDs
}
void ReadGeoModel::storeBuiltTransform(const unsigned int id, GeoTransform* nodePtr)
{
  m_memMapTransforms.push_back(nodePtr); // vector, we store them in the order of IDs

}
GeoTransform* ReadGeoModel::getBuiltTransform(const unsigned int id)
{
  return m_memMapTransforms[id-1]; // nodes' IDs start from 1
}
  
// --- methods for caching GeoAlignableTransform nodes ---
bool ReadGeoModel::isBuiltAlignableTransform(const unsigned int id)
{
  return (id <= m_memMapAlignableTransforms.size()); // vector: we exploit the fact that we built the vols ordered by their IDs
}
void ReadGeoModel::storeBuiltAlignableTransform(const unsigned int id, GeoAlignableTransform* nodePtr)
{
  m_memMapAlignableTransforms.push_back(nodePtr); // vector, we store them in the order of IDs
  
}
GeoAlignableTransform* ReadGeoModel::getBuiltAlignableTransform(const unsigned int id)
{
  return m_memMapAlignableTransforms[id-1]; // nodes' IDs start from 1
  
}
  
// --- methods for caching GeoSerialDenominator nodes ---
bool ReadGeoModel::isBuiltSerialDenominator(const unsigned int id)
{
  return (id <= m_memMapSerialDenominators.size()); // vector: we exploit the fact that we built the vols ordered by their IDs
}
void ReadGeoModel::storeBuiltSerialDenominator(const unsigned int id, GeoSerialDenominator* nodePtr)
{
  m_memMapSerialDenominators.push_back(nodePtr); // vector, we store them in the order of IDs
  
}
GeoSerialDenominator* ReadGeoModel::getBuiltSerialDenominator(const unsigned int id)
{
  return m_memMapSerialDenominators[id-1]; // nodes' IDs start from 1
  
}
  
// --- methods for caching GeoNameTag nodes ---
bool ReadGeoModel::isBuiltNameTag(const unsigned int id)
{
  return (id <= m_memMapNameTags.size()); // vector: we exploit the fact that we built the vols ordered by their IDs
}
void ReadGeoModel::storeBuiltNameTag(const unsigned int id, GeoNameTag* nodePtr)
{
  m_memMapNameTags.push_back(nodePtr); // vector, we store them in the order of IDs
  
}
GeoNameTag* ReadGeoModel::getBuiltNameTag(const unsigned int id)
{
  return m_memMapNameTags[id-1]; // vector, but nodes' IDs start from 1
  
}
  
// --- methods for caching GeoSerialDenominator nodes ---
bool ReadGeoModel::isBuiltSerialTransformer(const unsigned int id)
{
  return (id <= m_memMapSerialTransformers.size()); // vector: we exploit the fact that we built the vols ordered by their IDs
}
void ReadGeoModel::storeBuiltSerialTransformer(const unsigned int id, GeoSerialTransformer* nodePtr)
{
  m_memMapSerialTransformers.push_back(nodePtr); // vector, we store them in the order of IDs
  
}
GeoSerialTransformer* ReadGeoModel::getBuiltSerialTransformer(const unsigned int id)
{
  return m_memMapSerialTransformers[id-1]; // remember: nodes' IDs start from 1
  
}
  /* FIXME: 
  // --- methods for caching Functions nodes ---
  bool ReadGeoModel::isBuiltFunction(const unsigned int id)
  {
    return (id <= m_memMapFunctions.size()); // vector: we exploit the fact that we built the vols ordered by their IDs
  }
  void ReadGeoModel::storeBuiltFunction(const unsigned int id, GeoXF::Function* nodePtr)
  {
    m_memMapFunctions.push_back(nodePtr); // vector, we store them in the order of IDs
    
  }
  GeoXF::Function* ReadGeoModel::getBuiltFunction(const unsigned int id)
  {
    return m_memMapFunctions[id-1]; // remember: nodes' IDs start from 1
  }
*/
  
// --- methods for caching GeoPhysVol/GeoFullPhysVol nodes ---
std::string getVPhysVolKey(const unsigned int id, const unsigned int tableId, const unsigned int copyNumber)
{
  std::string key = std::to_string(id) + ":" + std::to_string(tableId) + ":" + std::to_string(copyNumber);
  return key;
}
void ReadGeoModel::storeVPhysVol(const unsigned int id, const unsigned int tableId, const unsigned int copyN, GeoGraphNode* nodePtr)
{
  std::lock_guard<std::mutex> lk(muxVPhysVol);
  std::string key = getVPhysVolKey(id, tableId, copyN);
  m_memMap[key] = nodePtr;
}
GeoGraphNode* ReadGeoModel::getVPhysVol(const unsigned int id, const unsigned int tableId, const unsigned int copyN)
{
	std::lock_guard<std::mutex> lk(muxVPhysVol);
  std::string key = getVPhysVolKey(id, tableId, copyN);
  if (m_memMap.find(key) == m_memMap.end()) {
    return nullptr; // if volume is not found in cache
  }
  return m_memMap[key];
}

} /* namespace GeoModelIO */
