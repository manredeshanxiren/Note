#include "widget.h"
#include "ui_widget.h"

#include <QLabel>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    QLabel* label1 = new QLabel(ui->tab);
    label1->setText("标签页1");

    QLabel* label2 = new QLabel(ui->tab_2);
    label2->setText("标签页2");
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    int count = ui->tabWidget->count();
    QWidget* w = new QWidget(ui->tabWidget);
    ui->tabWidget->addTab(w, QString("tab ") + QString::number(count + 1));
    QLabel* label = new QLabel(w);
    label->setText("标签页" + QString::number(count + 1));

    //设置当前选中的tab
    ui->tabWidget->setCurrentIndex(count);
}


void Widget::on_pushButton_2_clicked()
{
    int idx = ui->tabWidget->currentIndex();
    ui->tabWidget->removeTab(idx);

    // 更新标签页标题
    for(int i = idx; i < ui->tabWidget->count(); ++i) {
        QString newTitle = QString("Tab %1").arg(i + 1); // 根据需要修改标题格式
        ui->tabWidget->setTabText(i, newTitle);
        QString newlabel = QString("标签页 %1").arg(i + 1);
    }
}

