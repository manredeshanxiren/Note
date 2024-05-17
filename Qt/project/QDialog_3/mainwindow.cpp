#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDialog>

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
    //创建对话框对象
    QDialog* dialog = new QDialog();
    //调整对话框大小
    dialog->resize(400, 250);
    //设置对话框标题
    dialog->setWindowTitle("我是一个对话框");
    //显示对话框
    dialog->show();
    //设置点击x，就释放
    dialog->setAttribute(Qt::WA_DeleteOnClose);
}
