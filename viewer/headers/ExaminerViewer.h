#ifndef EXAMVIEWER_H
#define EXAMVIEWER_H
#include "CameraWrapper.h"
#include "GeneralMeshModel.h"
#include <QtWidgets/QVBoxLayout>
#include <QSequentialAnimationGroup>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>

extern CameraWrapper *camera;

class ExaminerViewer{
public:
    ExaminerViewer(GeneralMeshModel *worldModel, CameraWrapper *cameraWrapper);
    void setupControlPanel(QVBoxLayout *vLayout, QWidget *mainWindow);

private:
    void setUpInfoWindow();
    void setUpVolumePanel(QVBoxLayout *vLayout, QWidget *mainWindow);
    void setUpSliderController(QLabel *label, QSlider *slider, QString tip, int initalPos);
    QSequentialAnimationGroup *getRoute1Tour();
    GeneralMeshModel *m_worldModel;
    CameraWrapper *m_cameraWrapper;
};

#endif // EXAMVIEWER_H
