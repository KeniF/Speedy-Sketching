#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <display.h>
#include <QActionGroup>
namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Display * display;
    void adjustSliderH();
    static const int mode_freehand=1,mode_circle=6, mode_triangle=11,
        mode_rectangle=16,
        mode_cone=21,mode_cube=26,mode_cylinder_tube=31,
        mode_cylinder_drum=36,mode_sphere=41,mode_push_pull=46,
        mode_delete=51;
private:
    Ui::MainWindow *ui;
    QActionGroup *actionGroup;

private slots:
    void on_baseEnable_toggled(bool checked);
    void on_pushButton_clicked();
    void on_drawPanelButton_toggled(bool checked);
    void on_actionUndo_triggered();
    void on_actionDelete_triggered();
    void on_outButton_released();
    void on_inButton_released();
    void on_outButton_pressed();
    void on_inButton_pressed();
    void on_actionNew_triggered();
    void on_actionSquare_triggered();
    void on_actionPush_pull_triggered();
    void on_actionCylinder_drum_triggered();
    void on_actionCylinder_tube_triggered();
    void on_actionCube_triggered();
    void on_actionCone_triggered();
    void on_actionTriangle_triggered();
    void on_actionFreehand_triggered();
    void on_actionCircle_triggered();
    void on_actionSphere_triggered();
    void xRotation(int x);
    void yRotation(int y);
    void resetSliders();
    void zoomIn();
    void zoomOut();
};

#endif // MAINWINDOW_H
