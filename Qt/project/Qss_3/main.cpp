#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString style = "QLabel { border : 5px dashed blue; padding-left : 20px}";

    a.setStyleSheet(style);

    Widget w;
    w.show();
    return a.exec();
}
