#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

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
    QMessageBox* messagebox = new QMessageBox(this);
    messagebox->setWindowTitle("对话框窗口标题");
    messagebox->setText("这个一个对话框");
    messagebox->setIcon(QMessageBox::Warning);
    messagebox->setStandardButtons(QMessageBox::Ok | QMessageBox::Save | QMessageBox::Cancel);

    //模态显示对话框
    messagebox->exec();
    //设置释放属性
    messagebox->setAttribute(Qt::WA_DeleteOnClose);
}
