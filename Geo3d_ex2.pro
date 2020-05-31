message(Qt $$QT_VERSION is used.)

QT += 3dcore 3drender 3dinput 3dextras core
QT += widgets

SOURCES += main.cpp \
    meshModel.cpp

HEADERS += \
    meshModel.h

RESOURCES += \
    TrackML-PixelDetector.qrc
