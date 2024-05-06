#include "widget.h"
#include "ui_widget.h"

#include <QPushButton>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    if(windowTitle() != "hello world")
    {
        setWindowTitle("hello world");
    }
    else
    {
        setWindowTitle("hello Qt");
    }
}

void Widget::on_pushButton_2_clicked()
{
    bool enable = ui->pushButton->isEnabled();
    if(enable) ui->pushButton->setEnabled(false);
    else ui->pushButton->setEnabled(true);
}
