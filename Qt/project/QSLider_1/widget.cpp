#include "widget.h"
#include "ui_widget.h"

#include <QShortcut>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //使用快捷键
    QShortcut* shortCut1 = new QShortcut(this);
    shortCut1->setKey(QKeySequence("-"));

    QShortcut* shortCut2 = new QShortcut(this);
    shortCut2->setKey(QKeySequence("="));

    //使用信号槽，感知到按钮被按下
    connect(shortCut1, &QShortcut::activated, this, &Widget::subValue);
    connect(shortCut2, &QShortcut::activated, this, &Widget::addValue);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::subValue()
{

    int val = ui->horizontalSlider->value();
    ui->horizontalSlider->setValue(val - 5);
    val = ui->horizontalSlider->value();
    ui->label->setText(QString("当前的值为: ") + QString::number(val));
}

void Widget::addValue()
{

    int val = ui->horizontalSlider->value();
    ui->horizontalSlider->setValue(val + 5);
    val = ui->horizontalSlider->value();
    ui->label->setText(QString("当前的值为: ") + QString::number(val));
}

