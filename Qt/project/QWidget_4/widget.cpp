#include "widget.h"
#include "ui_widget.h"

#include <QIcon>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //设置图标
    //QIcon icon("C:/Users/jason/Desktop/icon.jpg");
    QIcon icon(":/icon.jpg");
    this->setWindowIcon(icon);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_add_clicked()
{
    float opacity = this->windowOpacity();

    if(opacity >= 1.0) return ;

    opacity += 0.1;

    qDebug() << opacity;
    this->setWindowOpacity(opacity);
}

void Widget::on_pushButton_sub_clicked()
{
    float opacity = this->windowOpacity();
    if(opacity < 0) return ;

    opacity -= 0.1;

    qDebug() << opacity;

    this->setWindowOpacity(opacity);
}
