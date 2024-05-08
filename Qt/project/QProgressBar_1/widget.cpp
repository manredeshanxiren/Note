#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &Widget::handle);

    //启动定时器
    timer->start(100);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::handle()
{
    //获取到当前数值
    int val = ui->progressBar->value();
    if(val >= 100)
    {
        timer->stop();
        return ;
    }

    ui->progressBar->setValue(val+1);

}

