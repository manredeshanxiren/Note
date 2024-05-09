#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);


    //初始化第一个输入框，用来输入姓名
    ui->lineEdit_name->setPlaceholderText("请输入姓名");
    ui->lineEdit_name->setClearButtonEnabled(true);

    //初始化第二个输入框，用来输入密码
    ui->lineEdit_psword->setPlaceholderText("请输入密码");
    ui->lineEdit_psword->setClearButtonEnabled(true);
    ui->lineEdit_psword->setEchoMode(QLineEdit::Password);

    //初始化第三个输入框，用来输入电话号码
    ui->lineEdit_telephone->setPlaceholderText("请输入电话号码");
    ui->lineEdit_telephone->setClearButtonEnabled(true);
    //因为号码有固定的格式,可以对输入框的内容进行简单的校验
    ui->lineEdit_telephone->setInputMask("000-0000-0000");
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_submit_clicked()
{
    QString  gender =  ui->radioButton_female->isChecked() ? "女" : "男";
    qDebug() << "姓名 ：" << ui->lineEdit_name->text()
             << "密码 ：" << ui->lineEdit_psword->text()
             << "性别 ：" << gender
             << "电话 ：" << ui->lineEdit_telephone->text();
}
