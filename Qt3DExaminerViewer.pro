message(Qt $$QT_VERSION is used.)

QT += 3dcore 3drender 3dinput 3dextras core 3danimation
QT += widgets

SOURCES += main.cpp \
    viewer/src/CameraWrapper.cpp \
    viewer/src/GeneralMeshModel.cpp \
    viewer/src/MainWindow.cpp \
    viewer/src/ModelFactory.cpp \
    viewer/src/ExaminerViewer.cpp \

HEADERS += \
    viewer/headers/CameraWrapper.h \
    viewer/headers/GeneralMeshModel.h \
    viewer/headers/MainWindow.h \
    viewer/headers/ModelFactory.h \
    viewer/headers/ExaminerViewer.h

RESOURCES += \
    resources.qrc

INSTALL_PATH=~/install/lib
EIGEN_PATH=/usr/local/include/eigen3

LIBS += -I$(INSTALL_PATH)/libGeoModelRead.3.2.0.dylib \
        -I$(INSTALL_PATH)/libGeoModelDBManager.3.2.0.dylib \
        -I$(INSTALL_PATH)/libGeoModelKernel.3.2.0.dylib \
        -I$(INSTALL_PATH)/libGeoGenericFunctions.3.2.0.dylib

INCLUDEPATH += $(EIGEN_PATH) \

