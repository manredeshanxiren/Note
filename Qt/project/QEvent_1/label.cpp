#include "Label.h"

#include <QDebug>

Label::Label(QWidget* parent) : QLabel(parent)
{

}

void Label::enterEvent(QEvent *event)
{
    qDebug() << "enterEvent";

}

void Label::leaveEvent(QEvent *event)
{
    qDebug() << "leaveEvent";
}
