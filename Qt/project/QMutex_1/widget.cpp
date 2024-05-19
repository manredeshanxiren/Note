#include "widget.h"
#include "ui_widget.h"

#include "thread.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //创建两个线程
    Thread t1;
    Thread t2;

    t1.start();
    t2.start();

    t1.wait();
    t2.wait();

    qDebug() << Thread::num;
}

Widget::~Widget()
{
    delete ui;


}

