#include "widget.h"
#include "ui_widget.h"

#include <QButtonGroup>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);


    //使用QButtonGroup对button进行分组
    QButtonGroup* group1 = new QButtonGroup(this);
    QButtonGroup* group2 = new QButtonGroup(this);
    QButtonGroup* group3 = new QButtonGroup(this);

    //把单旋按钮添加到各自的分组
    group1->addButton(ui->radioButton);
    group1->addButton(ui->radioButton_2);
    group1->addButton(ui->radioButton_3);

    group2->addButton(ui->radioButton_4);
    group2->addButton(ui->radioButton_5);
    group2->addButton(ui->radioButton_6);

    group3->addButton(ui->radioButton_8);
    group3->addButton(ui->radioButton_9);

}

Widget::~Widget()
{
    delete ui;
}

