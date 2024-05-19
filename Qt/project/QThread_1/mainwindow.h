#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "thread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //信号槽函数
    void handle();


private:
    Ui::MainWindow *ui;
    Thread thread;
};
#endif // MAINWINDOW_H
