#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

using namespace std;
//int MainWindow::mode=MainWindow::mode_circle;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    display=new Display(this,MainWindow::mode_circle);
    display->setCursor(Qt::CrossCursor);
    ui->gridLayout->addWidget(display,0,0,1,1); //OpenGL window
    //Set action group so only one can be pressed at a time
    actionGroup=new QActionGroup(this);
    actionGroup->addAction(ui->actionCircle);
    actionGroup->addAction(ui->actionSquare);
    actionGroup->addAction(ui->actionTriangle);
    actionGroup->addAction(ui->actionFreehand);
    actionGroup->addAction(ui->actionCone);
    actionGroup->addAction(ui->actionCube);
    actionGroup->addAction(ui->actionCylinder_tube);
    actionGroup->addAction(ui->actionCylinder_drum);
    actionGroup->addAction(ui->actionSphere);
    actionGroup->addAction(ui->actionPush_pull);
    actionGroup->addAction(ui->actionDelete);
    ui->actionCircle->setChecked(true);
}

MainWindow::~MainWindow(){
    cout<<"Deleting main window..."<<endl;
    delete display;
    delete ui;
    delete actionGroup;
}

void MainWindow::adjustSliderH(){
    //ui->horizontalSpacer->changeSize((geometry().width()-60)/3,20,QSizePolicy::Fixed,QSizePolicy::Fixed);
}

void MainWindow::xRotation(int x){
    display->xRotation(x*-1);
    //cout<<"x rotation"<<endl;
}

void MainWindow::yRotation(int y){
    display->yRotation(y);
    //cout<<"y rotation"<<endl;
}

void MainWindow::resetSliders(){
    ui->horizontalSlider->setSliderPosition(0);
    ui->verticalSlider->setSliderPosition(0);
}
void MainWindow::on_actionSphere_triggered(){
    ui->inButton->setEnabled(true);
    ui->outButton->setEnabled(true);
    display->modeChanged(mode_sphere);
}

void MainWindow::on_actionCircle_triggered(){
    ui->inButton->setEnabled(true);
    ui->outButton->setEnabled(true);
    display->modeChanged(mode_circle);
}

void MainWindow::on_actionFreehand_triggered(){
    ui->inButton->setEnabled(true);
    ui->outButton->setEnabled(true);
    display->modeChanged(mode_freehand);
}

void MainWindow::on_actionTriangle_triggered(){
    ui->inButton->setEnabled(true);
    ui->outButton->setEnabled(true);
    display->modeChanged(mode_triangle);
}

void MainWindow::on_actionCone_triggered(){
    ui->inButton->setEnabled(true);
    ui->outButton->setEnabled(true);
    display->modeChanged(mode_cone);
}

void MainWindow::on_actionCube_triggered(){
    ui->inButton->setEnabled(true);
    ui->outButton->setEnabled(true);
    display->modeChanged(mode_cube);
}

void MainWindow::on_actionCylinder_tube_triggered(){
    ui->inButton->setEnabled(true);
    ui->outButton->setEnabled(true);
    display->modeChanged(mode_cylinder_tube);
}

void MainWindow::on_actionCylinder_drum_triggered(){
    ui->inButton->setEnabled(true);
    ui->outButton->setEnabled(true);
    display->modeChanged(mode_cylinder_drum);
}

void MainWindow::on_actionPush_pull_triggered(){
    ui->inButton->setEnabled(true);
    ui->outButton->setEnabled(true);
    display->modeChanged(mode_push_pull);
}

void MainWindow::on_actionSquare_triggered(){
    ui->inButton->setEnabled(true);
    ui->outButton->setEnabled(true);
    display->modeChanged(mode_rectangle);
}

void MainWindow::zoomIn(){
    display->zoomIn();
    resetSliders();
}
void MainWindow::zoomOut(){
    display->zoomOut();
    resetSliders();
}

void MainWindow::on_actionNew_triggered(){
    display->clearScene();
}

void MainWindow::on_inButton_pressed(){
    display->panelIn();
}

void MainWindow::on_outButton_pressed(){
    display->panelOut();
}

void MainWindow::on_inButton_released(){
    display->panelMoveStopped();
}

void MainWindow::on_outButton_released(){
    display->panelMoveStopped();
}

void MainWindow::on_actionDelete_triggered(){
    ui->inButton->setEnabled(false);
    ui->outButton->setEnabled(false);
    display->modeChanged(mode_delete);
}

void MainWindow::on_actionUndo_triggered(){
    display->undo();
}

void MainWindow::on_drawPanelButton_toggled(bool checked)
{
    display->enableDrawPanel(checked);
    ui->inButton->setEnabled(checked);
    ui->outButton->setEnabled(checked);
}

void MainWindow::on_pushButton_clicked(){
    display->initializeValues();
}

void MainWindow::on_baseEnable_toggled(bool checked){
    display->enableBase(checked);
}
