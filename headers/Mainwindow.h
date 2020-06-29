#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <Qt3DRender/QCamera>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DInput/QLogicalDevice>
class MainWindow : public Qt3DExtras::Qt3DWindow
{
    Q_OBJECT
public:
    explicit MainWindow();
private:
    void resizeEvent(QResizeEvent* event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    Qt3DInput::QLogicalDevice *m_logicalDevice;
    Qt3DInput::QAxis *m_axisX;
    Qt3DInput::QAxis *m_axisY;
};

#endif // MAINWINDOW_H
