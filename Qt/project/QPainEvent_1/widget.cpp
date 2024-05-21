#include "widget.h"
#include "ui_widget.h"

#include <QPainter>

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

void Widget::paintEvent(QPaintEvent *event)
{
    (void) event;

    //这里的this是为了指定所绘画的对象
    QPainter painter(this);

    painter.drawLine(0,0, 100, 100);
    painter.drawRect(100,100, 200, 200);
    painter.drawText(0, 100, "hello");
}

