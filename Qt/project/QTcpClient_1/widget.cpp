#include "widget.h"
#include "ui_widget.h"

#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //1.设置窗口标题
    this->setWindowTitle("客户端");

    //2.创建socket对象
    socket = new QTcpSocket(this);

    //3.和服务器建立链接
    socket->connectToHost("127.0.0.1", 9090);

    //4.用信号槽处理对服务器响应的接受
    connect(socket, &QTcpSocket::readyRead, this, [=]()
    {
        //1)读取响应内容
        QString text = socket->readAll();
        //2)将内容显示到窗口
        ui->listWidget->addItem("服务端发送：" + text);
    });

    //5.等待建立链接的结果，查看链接是否建立成功
    bool ret = socket->waitForConnected();
    if(!ret)
    {
        QMessageBox::critical(this, "客户端链接错误", socket->errorString());
        exit(1);
    }

}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    //1.获取输入框中的内容
    QString text = ui->lineEdit->text();
    //2.将内容发送给服务器
    socket->write(text.toUtf8());
    //3.把发的消息显示到窗口
    ui->listWidget->addItem("客户端发送：" + text);
    //4.将输入框清空
    ui->lineEdit->setText("");
}
