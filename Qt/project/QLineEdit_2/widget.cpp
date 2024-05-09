#include "widget.h"
#include "ui_widget.h"


#include <QRegExpValidator>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //需要给单行输入框，设置验证器，基于正则表达式完成验证
    QRegExp regExp("^1\\d{10}$");
    //其中^表示以什么开头
    //\d表示数字,{10}表示前面的内容重复出现10次
    ui->lineEdit->setValidator(new QRegExpValidator(regExp));


}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_lineEdit_textEdited(const QString &text)
{
    QString content = text;
    int pos = 0;
    if(ui->lineEdit->validator()->validate(content, pos) == QValidator::Acceptable)
    {
        ui->pushButton->setEnabled(true);
    }
    else
    {
        ui->pushButton->setEnabled(false);
    }
}
