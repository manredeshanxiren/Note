#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialog.h"

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
    //弹出一个自定义的对话框
    Dialog* dialog = new Dialog(this);
    dialog->exec();
    dialog->setAttribute(Qt::WA_DeleteOnClose);
}
