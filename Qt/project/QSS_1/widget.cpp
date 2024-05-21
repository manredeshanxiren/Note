#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

//    //ui->pushButton->setStyleSheet("QPushButton { color : #faaa11 }");
//    this->setStyleSheet("QPushButton { color : #faaa11 }");
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{

}
