#include "widget.h"
#include "ui_widget.h"

#include <QLabel>
#include <QProcess>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //通过time函数设置随机种子
    srand(time(0));


}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_agreed_clicked()
{
    ui->label->setText("好的我们把电脑关了休息一下吧！");
    QProcess::execute("shutdown /s /t 5"); // 立即关机
}

void Widget::on_pushButton_reject_clicked()
{

}

void Widget::on_pushButton_reject_pressed()
{
    //修改按钮的位置，不让用户点击到
    //随机数的方式
    int width = this->geometry().width();
    int height = this->geometry().height();

    //为了不让移动的范围超出对应的widget的范围
    int x = rand() % width;
    int y = rand() % height;

    ui->pushButton_reject->move(x, y);
}
