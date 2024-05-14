#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->comboBox->addItem("巨无霸");
    ui->comboBox->addItem("香辣鸡腿堡");
    ui->comboBox->addItem("香辣酥脆牛堡");

    ui->comboBox_2->addItem("鸡翅");
    ui->comboBox_2->addItem("薯条");


    ui->comboBox_3->addItem("可乐");
    ui->comboBox_3->addItem("雪碧");


    //针对QSpinBox进行设置
    ui->spinBox->setRange(1, 5);
    ui->spinBox_2->setRange(1, 5);
    ui->spinBox_3->setRange(1, 5);

    ui->spinBox->setValue(1);
    ui->spinBox_2->setValue(1);
    ui->spinBox_3->setValue(1);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    qDebug() << "当前下单的内容："
             << ui->comboBox->currentText() << ":" << ui->spinBox->value()
             << ui->comboBox_2->currentText() << ":" << ui->spinBox_2->value()
             << ui->comboBox_3->currentText() << ":" << ui->spinBox_3->value();
}
