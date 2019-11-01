#include "mainwindow.h"
#include "ui_mainwindowform.h"
#include <qmessagebox.h>
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
   std::vector<double> newVelocity = convert_GUI_input_to_velocity(angleX,angleZ,power);
    mMainWindowUI->graphics->Fire(newVelocity);
}

void MainWindow::on_resetButton_clicked()
{
    mMainWindowUI->graphics->reset_game();
}

void MainWindow::on_instructionsButton_clicked()
{
    QMessageBox instructionMessage;
    instructionMessage.setText("Score the ball in the cube. Use the sliders to aim and the dial to adjust power. If the screen turns red, press 'Reset' to try again.");
    instructionMessage.setWindowTitle("Instructions");
    instructionMessage.setStandardButtons(QMessageBox::Ok);
    instructionMessage.setDefaultButton(QMessageBox::Ok);
    instructionMessage.setModal(true);
    instructionMessage.exec();
}
