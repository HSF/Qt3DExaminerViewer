#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <Qt3DRender/QCamera>
#include <Qt3DExtras/Qt3DWindow>

class MainWindow : public Qt3DExtras::Qt3DWindow
{
    Q_OBJECT
public:
    explicit MainWindow();
private:
    void resizeEvent(QResizeEvent* event);
    void mousePressEvent(QMouseEvent *event);
};

#endif // MAINWINDOW_H
