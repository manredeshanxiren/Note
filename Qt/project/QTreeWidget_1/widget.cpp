#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, "猫");
    ui->treeWidget->addTopLevelItem(item);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    QString text = ui->lineEdit->text();
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0 , text);
    ui->treeWidget->addTopLevelItem(item);
}

void Widget::on_pushButton_2_clicked()
{
    QString text = ui->lineEdit->text();
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, text);
    QTreeWidgetItem* items = ui->treeWidget->currentItem();
    items->addChild(item);
}

void Widget::on_pushButton_3_clicked()
{
    QTreeWidgetItem* items = ui->treeWidget->currentItem();

    if(items == nullptr) return ;
    QTreeWidgetItem* par = items->parent();
    if(par == nullptr)
    {
        int idx = ui->treeWidget->indexOfTopLevelItem(items);
        ui->treeWidget->takeTopLevelItem(idx);
    }
    else
    {
        par->removeChild(items);
    }
}
