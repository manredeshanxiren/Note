#include "dialog.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

Dialog::Dialog(QWidget* parent) : QDialog(parent)
{
    //创建出一些控件，加入到dialog当中，(以dialog作为父窗口)
    QVBoxLayout* layout = new QVBoxLayout();
    this->setLayout(layout);

    QLabel* label = new QLabel("我是一个标签");
    QPushButton* button = new QPushButton("关闭");

    layout->addWidget(label);
    layout->addWidget(button);

    //关联信号槽，让对话框关闭
    connect(button, &QPushButton::clicked, this, &Dialog::handle);
}

void Dialog::handle()
{
    this->close();
}
