#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(this, &Widget::mySignal, this, &Widget::handleMySignal);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::handleMySignal(const QString& text)
{
    this->setWindowTitle(text);
}


void Widget::on_pushButton_clicked()
{
    //发送信号
    //发送信号可以在任何合适的位置，不一定非得在构造函数中
    //这里当我们按下按钮时就发送对应的信号
    emit mySignal("what can I say?");
    //emit mySignal(1);
}

void Widget::on_pushButton_2_clicked()
{
    emit mySignal("manba out!");
}
