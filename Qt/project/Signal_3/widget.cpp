#include "widget.h"
#include "ui_widget.h"

#include <QPushButton>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    QPushButton* button = new QPushButton(this);
    button->setText("按钮");
    //button->setEnabled(false);
    button->move(200, 200);

    connect(button, &QPushButton::clicked, this, [=]()
    {
       qDebug() << "lambda被执行了!";
       this->move(100,100);
       button->move(300, 300);
    });
}

Widget::~Widget()
{
    delete ui;
}

