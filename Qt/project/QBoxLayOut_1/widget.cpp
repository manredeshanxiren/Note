#include "widget.h"
#include "ui_widget.h"

#include <QPushButton>
#include <QBoxLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    QPushButton* button1 = new QPushButton();
    QPushButton* button2 = new QPushButton();
    QPushButton* button3 = new QPushButton();
    button1->setText("按钮1");
    button2->setText("按钮2");
    button3->setText("按钮3");

    QBoxLayout* vlay = new QVBoxLayout(this);
    vlay->addWidget(button1);
    vlay->addWidget(button2);
    vlay->addWidget(button3);
}

Widget::~Widget()
{
    delete ui;
}

