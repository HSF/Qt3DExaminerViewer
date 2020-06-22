#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <Qt3DRender/QCamera>
#include <Qt3DExtras/Qt3DWindow>

class MainWindow : public Qt3DExtras::Qt3DWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void addCamera(Qt3DRender::QCamera *camera);
private:
    void resizeEvent(QResizeEvent* event);
    void mousePressEvent(QMouseEvent *event);
    Qt3DRender::QCamera *m_camera;
};

#endif // MAINWINDOW_H
