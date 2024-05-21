#include "widget.h"
#include "ui_widget.h"

#include <QPushButton>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //构造一个button的对象，并且将其显示在窗口
    QPushButton* button = new QPushButton(this);
    button->setText("按钮");

    //设置按钮对应的相对位置和大小
    button->setGeometry(0, 0, 100, 100);
    //设置button的QSS
    QString style = "QPushButton { border : 5px solid green; margin : 20px; color : blue; }";
    button->setStyleSheet(style);
    //输出按钮的相对位置和大小进行观察
    qDebug() << button->geometry();
}

Widget::~Widget()
{
    delete ui;
}

