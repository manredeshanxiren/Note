#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //添加一个默认的选项
    ui->radioButton_male->setChecked(true);
    ui->label->setText("您选择的性别为：男");

    //禁用其他选项
    //只是能够让按钮不被选中，但是可以响应单击事件
    //ui->radioButton_other->setCheckable(false);

    //如下的方式都是可以的
    ui->radioButton_other->setEnabled(false);
    //ui->radioButton_other->setDisabled(true);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_radioButton_male_clicked()
{
    //把界面上的label内容进行修改
    ui->label->setText("您选择的性别为：男");
}


void Widget::on_radioButton_female_clicked()
{
    //把界面上的label内容进行修改
    ui->label->setText("您选择的性别为：女");
}

void Widget::on_radioButton_other_clicked()
{
    //把界面上的label内容进行修改
    ui->label->setText("您选择的性别为：其他");
}
