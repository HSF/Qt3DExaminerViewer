message(Qt $$QT_VERSION is used.)

QT += 3dcore 3drender 3dinput 3dextras core
QT += widgets

SOURCES += src/main.cpp \
    src/CameraWrapper.cpp \
    src/MeshModel.cpp \
    src/SwithButton.cpp

HEADERS += \
    headers/CameraWrapper.h \
    headers/MeshModel.h \
    headers/SwithButton.h

RESOURCES += \
    res/resources.qrc
