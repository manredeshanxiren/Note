#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QSslSocket>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    qDebug()<<"QSslSocket="<<QSslSocket::sslLibraryBuildVersionString();
    qDebug() << "OpenSSL支持情况:" << QSslSocket::supportsSsl();


}

Widget::~Widget()
{
    delete ui;
}

