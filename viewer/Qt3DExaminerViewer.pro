message(Qt $$QT_VERSION is used.)

QT += 3dcore 3drender 3dinput 3dextras core 3danimation
QT += widgets

SOURCES += src/main.cpp \
    src/CameraWrapper.cpp \
    src/GeneralMeshModel.cpp \
    src/MainWindow.cpp \
    src/ModelFactory.cpp \
    src/ExaminerViewer.cpp
    ../

HEADERS += \
    headers/CameraWrapper.h \
    headers/GeneralMeshModel.h \
    headers/MainWindow.h \
    headers/ModelFactory.h \
    headers/ExaminerViewer.h

RESOURCES += \
    res/resources.qrc
