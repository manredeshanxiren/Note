#include "widget.h"
#include "ui_widget.h"

#include <QIcon>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //设置按钮的图标
    ui->pushButton_target->setIcon(QIcon(":/image/cheems.png"));
    ui->pushButton_target->setIconSize(QSize(120, 120));

    ui->pushButton_up->setIcon(QIcon(":/image/up-arrow.png"));
    ui->pushButton_up->setIconSize(QSize(60,60));

    ui->pushButton_down->setIcon(QIcon(":/image/down-arrow.png"));
    ui->pushButton_down->setIconSize(QSize(60,60));

    ui->pushButton_left->setIcon(QIcon(":/image/left-arrow.png"));
    ui->pushButton_left->setIconSize(QSize(60,60));

    ui->pushButton_right->setIcon(QIcon(":/image/right-arrow.png"));
    ui->pushButton_right->setIconSize(QSize(60,60));

    //这种方式很可能写错
//    ui->pushButton_up->setShortcut(QKeySequence("ctrl + w"));
//    ui->pushButton_down->setShortcut(QKeySequence("s"));
//    ui->pushButton_left->setShortcut(QKeySequence("a"));
//    ui->pushButton_right->setShortcut(QKeySequence("d"));
    //所以我们采用枚举的方式实现一下
    //ui->pushButton_up->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_W));
    ui->pushButton_up->setShortcut(QKeySequence(Qt::Key_W));
    ui->pushButton_down->setShortcut(QKeySequence(Qt::Key_S));
    ui->pushButton_left->setShortcut(QKeySequence(Qt::Key_A));
    ui->pushButton_right->setShortcut(QKeySequence(Qt::Key_D));

    ui->pushButton_up->setAutoRepeat(true);
    ui->pushButton_down->setAutoRepeat(true);
    ui->pushButton_left->setAutoRepeat(true);
    ui->pushButton_right->setAutoRepeat(true);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_up_clicked()
{
    //获取到target的位置
    QRect rect = ui->pushButton_target->geometry();
    //基于上次的位置来设置新的位置
    ui->pushButton_target->setGeometry(rect.x(), rect.y() - 5, rect.width(), rect.height());
}

void Widget::on_pushButton_right_clicked()
{
    //获取到target的位置
    QRect rect = ui->pushButton_target->geometry();
    //基于上次的位置来设置新的位置
    ui->pushButton_target->setGeometry(rect.x() + 5, rect.y(), rect.width(), rect.height());
}

void Widget::on_pushButton_left_clicked()
{
    //获取到target的位置
    QRect rect = ui->pushButton_target->geometry();
    //基于上次的位置来设置新的位置
    ui->pushButton_target->setGeometry(rect.x() - 5, rect.y(), rect.width(), rect.height());
}

void Widget::on_pushButton_down_clicked()
{
    //获取到target的位置
    QRect rect = ui->pushButton_target->geometry();
    //基于上次的位置来设置新的位置
    ui->pushButton_target->setGeometry(rect.x(), rect.y() + 5, rect.width(), rect.height());
}
