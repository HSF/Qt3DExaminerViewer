message(Qt $$QT_VERSION is used.)

QT += 3dcore 3drender 3dinput 3dextras core 3danimation
QT += widgets

SOURCES += main.cpp \
    src/CameraWrapper.cpp \
    src/GeneralMeshModel.cpp \
    src/MainWindow.cpp \
    src/ModelFactory.cpp \
    src/ExaminerViewer.cpp

HEADERS += \
    headers/CameraWrapper.h \
    headers/GeneralMeshModel.h \
    headers/MainWindow.h \
    headers/ModelFactory.h \
    headers/ExaminerViewer.h

RESOURCES += \
    res/resources.qrc

LIBS += -I($INSTALL_PATH)/libGeoModelRead.3.2.0.dylib \
        -I($INSTALL_PATH)/libGeoModelDBManager.3.2.0.dylib \
        -I($INSTALL_PATH)/libGeoModelKernel.3.2.0.dylib \
        -I($INSTALL_PATH)/libGeoGenericFunctions.3.2.0.dylib

INSTALL_PATH = ~/install/lib
EIGEN_PATH = /usr/local/include/eigen3

INCLUDEPATH += EIGEN_PATH

