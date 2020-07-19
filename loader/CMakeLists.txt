# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration

################################################################################
# Package: GeoLoader
# author: Riccardo Maria BIANCHI @ CERN - Jul, 2020
################################################################################

cmake_minimum_required(VERSION 3.1.0)

project(GeoLoader)

find_package( GeoModelCore REQUIRED )
find_package( GeoModelIO REQUIRED )

# Set default build options.
set( CMAKE_BUILD_TYPE "Release" CACHE STRING "CMake build mode to use" )
set( CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard used for the build" )
set( CMAKE_CXX_EXTENSIONS FALSE CACHE BOOL "(Dis)allow using GNU extensions" )
set( CMAKE_CXX_STANDARD_REQUIRED TRUE CACHE BOOL
   "Require the specified C++ standard for the build" )

# Find includes in current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Populate a CMake variable with the sources
set(SRCS geoloader.cpp )

# Tell CMake to create the helloworld executable
add_executable( geoloader ${SRCS} )

# Link all needed libraries
target_link_libraries( geoloader GeoModelDBManager GeoModelRead GeoModelKernel)
