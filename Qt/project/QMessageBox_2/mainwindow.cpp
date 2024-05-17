#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    int reslut = QMessageBox::critical(this, "对话框标题", "这是一个对话框", QMessageBox::Ok | QMessageBox::Cancel);
    if(reslut == QMessageBox::Ok)
    {
        qDebug() << "Ok";
    }
    else if(reslut == QMessageBox::Cancel)
    {
        qDebug() << "Cancel";
    }
}
