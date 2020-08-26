#ifndef EXAMVIEWER_H
#define EXAMVIEWER_H
#include "GeneralMeshModel.h"
#include <QtWidgets/QVBoxLayout>
#include <QSequentialAnimationGroup>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>

class ExaminerViewer{
public:
    ExaminerViewer(GeneralMeshModel *worldModel, CameraWrapper *cameraWrapper);
    void setupControlPanel(QVBoxLayout *vLayout, QWidget *mainWindow);

private:
    void setUpInfoWindow();
    void setUpSliderController(QLabel *label, QSlider *slider, QString tip, int initalPos);
    QSequentialAnimationGroup *getRoute1Tour();
    QSequentialAnimationGroup *getRoute2Tour();
    GeneralMeshModel *m_worldModel;
    CameraWrapper *m_cameraWrapper;
};

#endif // EXAMVIEWER_H
