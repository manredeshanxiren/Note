#include "label.h"

#include <QDebug>
#include <QMouseEvent>

Label::Label(QWidget* parent) : QLabel(parent)
{

}

void Label::mousePressEvent(QMouseEvent *event)
{
    //event对象包含了对应的鼠标点击位置的坐标
    qDebug() << event->x() << ", " << event->y();
}
