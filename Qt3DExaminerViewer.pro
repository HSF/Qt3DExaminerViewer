message(Qt $$QT_VERSION is used.)
message($$_PRO_FILE_PWD_)

QT += 3dcore 3drender 3dinput 3dextras core 3danimation
QT += widgets quick

SOURCES += main.cpp \
    viewer/src/CameraWrapper.cpp \
    viewer/src/GeneralMeshModel.cpp \
    viewer/src/MainWindow.cpp \
    viewer/src/ModelFactory.cpp \
    viewer/src/ExaminerViewer.cpp \
    loader/src/GeoLoaderQt.cpp \

HEADERS += \
    viewer/headers/CameraWrapper.h \
    viewer/headers/GeneralMeshModel.h \
    viewer/headers/MainWindow.h \
    viewer/headers/ModelFactory.h \
    viewer/headers/ExaminerViewer.h \
    loader/headers/GeoLoaderQt.h \


CONFIG += c++17 release

VERSION = 1.0.0

GMEX_PATH = $${_PRO_FILE_PWD_}/loader/lib
EIGEN_PATH = /usr/local/include/eigen3

LIBS += $${GMEX_PATH}/libGeoModelRead.3.2.0.dylib \
        $${GMEX_PATH}/libGeoModelDBManager.3.2.0.dylib \
        $${GMEX_PATH}/libGeoModelKernel.3.2.0.dylib \
        $${GMEX_PATH}/libGeoGenericFunctions.3.2.0.dylib \

RESOURCES += resources.qrc \

INCLUDEPATH += /usr/local/include/  \
        $${EIGEN_PATH}


