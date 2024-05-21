#include "widget.h"

#include <QApplication>

#include <QFile>

QString loadQss()
{
    QFile type(":/style.qss");
    type.open(QFile::ReadOnly);
    QString qss = type.readAll();
    type.close();
    return qss;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //设置全局的样式
    a.setStyleSheet(loadQss());

    Widget w;
    w.show();
    return a.exec();
}
