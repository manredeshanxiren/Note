#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //初始化，将两个输入框的模式调为输入密码的模式
    ui->lineEdit->setEchoMode(QLineEdit::Password);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);

    ui->label->setText("密码为空");
}

Widget::~Widget()
{
    delete ui;
}



void Widget::on_lineEdit_textEdited(const QString &arg1)
{
    (void)arg1;
    this->compare();
}

void Widget::on_lineEdit_2_textEdited(const QString &arg1)
{
    (void)arg1;
    this->compare();
}

void Widget::compare()
{
    const QString& s1 = ui->lineEdit->text();
    const QString& s2 = ui->lineEdit_2->text();
    if(s1.isEmpty() && s1.isEmpty())
    {
        ui->label->setText("密码为空");
    }
    else if(s1 == s2)
    {
        ui->label->setText("密码一致");
    }
    else
    {
        ui->label->setText("密码不一致");
    }
}


void Widget::on_checkBox_toggled(bool checked)
{
    if(checked)
    {
        ui->lineEdit->setEchoMode(QLineEdit::Normal);
        ui->lineEdit_2->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui->lineEdit->setEchoMode(QLineEdit::Password);
        ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    }
}
