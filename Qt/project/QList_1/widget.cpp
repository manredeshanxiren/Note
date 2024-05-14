#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->listWidget->addItem("c++");
    ui->listWidget->addItem("java");
    ui->listWidget->addItem("go");
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    //获取输入框中的内容
    const QString text = ui->lineEdit->text();
    //将获取到的信息添加到list中
    ui->listWidget->addItem(text);
}


void Widget::on_pushButton_2_clicked()
{
    //查看list中有哪一行被选中了
    int row = ui->listWidget->currentRow();
    if(row < 0)
    {
        return ;
    }
    ui->listWidget->takeItem(row);
}




void Widget::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(current != nullptr) qDebug() << "当前选中的是：" << current->text();
    if(previous != nullptr) qDebug() << "之前选中的是：" << previous->text();
}
