#include "widget.h"
#include "ui_widget.h"

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
    QDateTime timeOld = ui->dateTimeEdit->dateTime();
    QDateTime timeNew = ui->dateTimeEdit_2->dateTime();

    int sec = timeOld.secsTo(timeNew);
    int days = (sec / 3600) / 24;
    int hous = (sec / 3600) % 24;

    sec %= 60;

    ui->label->setText(QString("相差的时间为：") + QString::number(days)
                       +QString("天 ") + QString::number(hous)
                       +QString("小时 ") + QString::number(sec));
}
