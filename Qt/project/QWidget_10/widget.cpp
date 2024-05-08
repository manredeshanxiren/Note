#include "widget.h"
#include "ui_widget.h"

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


void Widget::on_pushButton_light_clicked()
{
    //背景设置为白色
    this->setStyleSheet("background-color:rgb(240,240,240);");
    //设置输入框的样式
    ui->textEdit->setStyleSheet("background-color:white; color:black;");
    //设置按钮的样式--调整字体颜色为黑色
    ui->pushButton_light->setStyleSheet("color:black;");
    ui->pushButton_dark->setStyleSheet("color:black;");
}

void Widget::on_pushButton_dark_clicked()
{
    //背景设置为白色
    this->setStyleSheet("background-color:rgb(0,0,0);");
    //设置输入框的样式
    ui->textEdit->setStyleSheet("backgroud-color:black; color:grey;");
    //设置按钮的样式--调整字体颜色为黑色
    ui->pushButton_light->setStyleSheet("color:grey;");
    ui->pushButton_dark->setStyleSheet("color:grey;");
}
