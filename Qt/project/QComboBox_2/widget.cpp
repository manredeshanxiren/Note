#include "widget.h"
#include "ui_widget.h"

#include <fstream>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    std::ifstream ifs("C:/Users/jason/Desktop/config.txt");
    if(ifs.is_open() == false)
    {
        qDebug() << "文件打开失败";
        return ;
    }
    std::string line;
    while(std::getline(ifs, line) )
    {
        ui->comboBox->addItem(QString::fromStdString(line));
    }
}

Widget::~Widget()
{
    delete ui;
}

