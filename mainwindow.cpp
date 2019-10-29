#include "mainwindow.h"
#include "ui_mainwindowform.h"

#include <QDockWidget>

std::vector<double> convert_GUI_input_to_velocity(int angleX,int angleZ,int power)
{
    double newXVelocity = static_cast<double>(power)*sin(static_cast<double>(angleX)*3.14/180.0);
    double newYVelocity = static_cast<double>(power)*cos(static_cast<double>(angleX)*3.14/180.0);
    double newZVelocity = static_cast<double>(power)*cos(static_cast<double>(angleZ)*3.14/180.0);
    std::vector<double> newVelocity{newXVelocity,newYVelocity,newZVelocity};

    return newVelocity;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow{parent},
    mMainWindowUI{new Ui::MainWindowForm}
{
    mMainWindowUI->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete mMainWindowUI;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_startButton_clicked()
{
   int angleX = mMainWindowUI->xDirectionBar->value();
   int angleZ = mMainWindowUI->zDirectionBar->value();
   int power = mMainWindowUI->powerDial->value();
   std::vector<double> newVelocity = convert_GUI_input_to_velocity(angleX,power,angleZ);
    //value to actual value using a function
    mMainWindowUI->powerDial->value();
    mMainWindowUI->graphics->Fire(newVelocity);
}
