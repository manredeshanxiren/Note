#include "widget.h"
#include "ui_widget.h"


#include <QIcon>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //创建icon对象
    QIcon icon(":/ikun.png");
    //设置图标
    ui->pushButton->setIcon(icon);
    //设置图标的尺寸
    ui->pushButton->setIconSize(QSize(50,50));
}

Widget::~Widget()
{
    delete ui;
}

