#include "widget.h"
#include "ui_widget.h"
#include "mylabel.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    MyLabel* mylable = new MyLabel(this);

    mylable->setText("HelloWorld");
}

Widget::~Widget()
{
    delete ui;
}

