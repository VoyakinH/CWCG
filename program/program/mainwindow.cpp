#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this_scene = new MWidget(ui->MWidget);
    this_scene->setMinimumSize(X_max, Y_max);
    this_scene->baseConfigModels();
    this_scene->update();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ButModelRotRight_clicked()
{
    this_scene->rotateFigure(12);
    this_scene->update();
    repaint();
}

void MainWindow::on_ButModelRotLeft_clicked()
{
    this_scene->rotateFigure(-12);
    this_scene->update();
    repaint();
}

void MainWindow::on_ButRenderTree_clicked()
{
    this_scene->choose_model = 0;
    this_scene->update();
    repaint();
}

void MainWindow::on_ButRenderGrid_clicked()
{
    this_scene->choose_model = 1;
    this_scene->update();
    repaint();
}
