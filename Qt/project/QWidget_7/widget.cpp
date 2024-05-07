#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //设置这两个按钮的tooltip
    ui->pushButton_yes->setToolTip("这是一个yes按钮");
    ui->pushButton_yes->setToolTipDuration(1000);
    ui->pushButton_no->setToolTip("这是一个no按钮");
    ui->pushButton_no->setToolTipDuration(1000);

}

Widget::~Widget()
{
    delete ui;
}

