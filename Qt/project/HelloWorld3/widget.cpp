#include "widget.h"
#include "ui_widget.h"

#include <QPushButton>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    button = new QPushButton(this);

    button->setText("关闭");

    button->move(400,300);


    connect(button, &QPushButton::clicked, this, &Widget::close);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::handleClick()
{
    if(button->text() == QString("hello world"))
    {
        button->setText("hello Qt");
    }
    else
    {
        button->setText("hello wrold");
    }
}

