#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    //获取文件路径
    QString path = QFileDialog::getOpenFileName();
    //构造对应的QFileInfo对象
    QFileInfo fileinfo(path);
    //打印相关属性
    qDebug() << fileinfo.fileName();
    qDebug() << fileinfo.suffix();
    qDebug() << fileinfo.isDir();
    qDebug() << fileinfo.size();


}
