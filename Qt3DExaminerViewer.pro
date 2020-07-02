message(Qt $$QT_VERSION is used.)

QT += 3dcore 3drender 3dinput 3dextras core 3danimation
QT += widgets

SOURCES += src/main.cpp \
    src/CameraWrapper.cpp \
    src/GeneralMeshModel.cpp \
    src/MainWindow.cpp \

HEADERS += \
    headers/CameraWrapper.h \
    headers/GeneralMeshModel.h \
    headers/Mainwindow.h \

RESOURCES += \
    res/resources.qrc
