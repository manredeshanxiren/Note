#include "mylabel.h"
#include <iostream>
#include <QDebug>


MyLabel::MyLabel(QWidget* parent)
    :QLabel(parent)
{

}

MyLabel::~MyLabel()
{
    //std::cout << "~MyLabel()析构函数" << std::endl;

    qDebug() << "~MyLabel()析构函数";
}
