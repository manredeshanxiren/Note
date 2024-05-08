#include "widget.h"
#include "ui_widget.h"

#include <QResizeEvent>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //qlabel设置的和窗口一样大，并且把qlabel的左上角设置到对应的窗口的左上角
    QRect windowRect = this->geometry();
    ui->label->setGeometry(0,0, windowRect.width(), windowRect.height());

    QPixmap image(":/ikun.png");
    ui->label->setPixmap(image);
    //启动自动拉伸
    ui->label->setScaledContents(true);
}

Widget::~Widget()
{
    delete ui;
}

//这里的event包含了触发这个resizeEvent的事件的这一时刻，窗口的尺寸的数值
void Widget::resizeEvent(QResizeEvent *event)
{
    qDebug() << event->size();
    ui->label->setGeometry(0,0, event->size().width(), event->size().height());
}

