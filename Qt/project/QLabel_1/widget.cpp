#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //在构造函数中，给这几个label设定不同的属性
    //设置对齐
    ui->label->setText("这是一段文本");
    ui->label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    //自动换行
    ui->label_2->setText("设置一段很长的文本设置一段很长的文本设置一段很长的文本设置一段很长的文本设置一段很长的文本设置一段很长的文本设置一段很长的文本");
    ui->label_2->setWordWrap(true);

    //设置缩进
    ui->label_3->setText("这是一段文本这是一段文本这是一段文本这是一段文本这是一段文本这是一段文本这是一段文本这是一段文本这是一段文本这是一段文本这是一段文本这是一段文本这是一段文本");
    ui->label_3->setIndent(50); //单位是像素
    ui->label_3->setWordWrap(true);

    //设置编剧
    ui->label_4->setText("这是一段文本");
    ui->label_4->setMargin(40);
}

Widget::~Widget()
{
    delete ui;
}

