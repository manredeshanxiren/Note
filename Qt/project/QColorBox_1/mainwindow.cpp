#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColorDialog>
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
    QColor color = QColorDialog::getColor(QColor(255, 0, 0), this, "选择颜色");
    qDebug() << color ;

    char style[1024] = {0};
    sprintf(style,"background-color: rgb(%d,%d,%d)", color.red(), color.green(), color.blue());
    this->setStyleSheet(style);
}
