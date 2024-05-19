#ifndef LABEL_H
#define LABEL_H

#include <QObject>
#include <QLabel>

class Label : QLabel
{
    Q_OBJECT
public:
    Label(QWidget* parent);

    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
};

#endif // LABEL_H
