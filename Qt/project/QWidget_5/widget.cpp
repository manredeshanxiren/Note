#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //访问到图片文件
    QPixmap pixmap(":/icon.jpg");

    pixmap = pixmap.scaled(30, 30);
    //构造光标文件
    QCursor cursor(pixmap, 10, 10);

    this->setCursor(cursor);
}

Widget::~Widget()
{
    delete ui;
}

