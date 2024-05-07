#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    Qt::WindowFlags flags = w.windowFlags();
    w.setWindowFlags(flags & ~Qt::WindowCloseButtonHint);
    w.show();
    return a.exec();
}
