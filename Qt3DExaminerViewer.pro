message(Qt $$QT_VERSION is used.)

QT += 3dcore 3drender 3dinput 3dextras core
QT += widgets

SOURCES += main.cpp \
    CameraWrapper.cpp \
    MeshModel.cpp \
    SwithButton.cpp

HEADERS += \
    CameraWrapper.h \
    MeshModel.h \
    SwithButton.h

RESOURCES += \
    resources.qrc
