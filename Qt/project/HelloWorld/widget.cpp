#include "widget.h"
#include "ui_widget.h"

#include <QLabel>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //在堆上创建
    QLabel * label = new QLabel(this);
    //在QString中也提供了C风格字符串构造QString对象
    label->setText("HelloWorld");


    //delete label;
}

Widget::~Widget()
{
    delete ui;
}

