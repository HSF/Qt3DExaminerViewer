# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
################################################################################
# Package: GeoModelRead
# author: Riccardo Maria BIANCHI <rbianchi@cern.ch> - 2017
# major updates: rbianchi@cern.ch, 2018
################################################################################

# Find the header and source files.
file( GLOB SOURCES src/*.cpp )
file( GLOB HEADERS GeoModelRead/*.h )

# Set up the library.
add_library( GeoModelRead SHARED ${HEADERS} ${SOURCES} )
target_link_libraries( GeoModelRead PUBLIC
GeoModelCore::GeoModelKernel GeoModelDBManager TFPersistification )
target_include_directories( GeoModelRead PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<INSTALL_INTERFACE:include> )
source_group( "GeoModelRead" FILES ${HEADERS} )
source_group( "src" FILES ${SOURCES} )
set_target_properties( GeoModelRead PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )

# Install the library.
install( TARGETS GeoModelRead
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Runtime
   NAMELINK_SKIP )
install( TARGETS GeoModelRead
   EXPORT ${PROJECT_NAME}-export
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   COMPONENT Development
   NAMELINK_ONLY )
install( FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GeoModelRead
   COMPONENT Development )
