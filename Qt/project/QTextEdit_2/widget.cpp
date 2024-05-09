#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_textEdit_textChanged()
{
    qDebug() << "textChanged:" << ui->textEdit->toPlainText();
}

void Widget::on_textEdit_selectionChanged()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    qDebug() << "select text:" << cursor.selectedText();
}

void Widget::on_textEdit_cursorPositionChanged()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    qDebug() << "cursor position: " << cursor.position();
}

void Widget::on_textEdit_undoAvailable(bool b)
{
    qDebug() << "undoAvailable :" << b;
}


void Widget::on_textEdit_redoAvailable(bool b)
{
    qDebug() << "redoAvailable :" << b;
}
