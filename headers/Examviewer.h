#ifndef EXAMVIEWER_H
#define EXAMVIEWER_H
#include "headers/CameraWrapper.h"
#include "headers/GeneralMeshModel.h"
#include <QtWidgets/QVBoxLayout>
#include <QSequentialAnimationGroup>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>

class ExamViewer{
public:
    ExamViewer(GeneralMeshModel *cylinerModel, CameraWrapper *cameraWrapper);
    void setupControlPanel(QVBoxLayout *vLayout, QWidget *mainWindow);

private:
    void setUpInfoWindow();
    void setUpVolumePanel(QVBoxLayout *vLayout, QWidget *mainWindow);
    void setUpSliderController(QLabel *label, QSlider *slider, QString tip, int initalPos);
    QSequentialAnimationGroup *getRoute1Tour();
    GeneralMeshModel *m_cylinderModel;
    CameraWrapper *m_cameraWrapper;
};

#endif // EXAMVIEWER_H
