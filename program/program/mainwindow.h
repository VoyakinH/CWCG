#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QDebug>
#include "widget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_ButModelRotRight_clicked();

    void on_ButModelRotLeft_clicked();

    void on_ButRenderTree_clicked();

    void on_ButRenderGrid_clicked();

private:
    Ui::MainWindow *ui;
    MWidget *this_scene;
};

#endif // MAINWINDOW_H
