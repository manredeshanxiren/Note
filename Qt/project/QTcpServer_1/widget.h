#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QString process(QString request);

public slots:
    void requestHandle();

private:
    Ui::Widget *ui;
    QTcpServer* tcpserver;
};
#endif // WIDGET_H
