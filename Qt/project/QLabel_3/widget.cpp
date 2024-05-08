#include "widget.h"
#include "ui_widget.h"

#include <QTimer>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //设置初始值
    ui->lcdNumber->display(10);

    //创建一个QTimer实例
    timer = new QTimer(this);

    //把timer的timeout信号和自己实现的槽函数进行链接
    connect(timer, &QTimer::timeout, this, &Widget::handle);
    //启动定时器，参数是触发timeout的周期,单位是ms
    timer->start(1000);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::handle()
{
    //先拿到lcdNumber的值
    int val = ui->lcdNumber->intValue();
    if(val <= 0)
    {
        //数字都减到零了，停止计时
        timer->stop();
        return ;
    }

    ui->lcdNumber->display(val - 1);
}

