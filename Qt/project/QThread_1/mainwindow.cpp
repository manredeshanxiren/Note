#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //启动线程
    thread.start();

    //链接信号槽，通过信号槽来更新界面
    connect(&thread, &Thread::notify, this, &MainWindow::handle);
}

MainWindow::~MainWindow()
{
    delete ui;


}

void MainWindow::handle()
{
    //此处修改界面内容
     int val = ui->lcdNumber->intValue();
     val--;
     ui->lcdNumber->display(val);
}

