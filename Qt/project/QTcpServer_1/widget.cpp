#include "widget.h"
#include "ui_widget.h"

#include <QMessageBox>
#include <QTcpSocket>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //设置窗口标题
    this->setWindowTitle("服务端");

    //创建tcpserver的对象实例
    tcpserver = new QTcpServer(this);

    //通过信号槽设置处理客户端请求的函数
    connect(tcpserver, &QTcpServer::newConnection, this, &Widget::requestHandle);

    //绑定+监听:设定对应的监听地址的范围以及端口号
    bool ret = tcpserver->listen(QHostAddress::Any, 9090);
    if(!ret) //如果失败打印失败的结果
    {
        QMessageBox::critical(this, "服务器绑定失败" , tcpserver->errorString());
        exit(1);
    }
}

Widget::~Widget()
{
    delete ui;
}



void Widget::requestHandle()
{
    //获取客户端的信息
    QTcpSocket* clientSocket = tcpserver->nextPendingConnection();
    QString log = "[" + clientSocket->peerAddress().toString() + ":" + clientSocket->peerPort() + "] 客户端上线";
    //显示到窗口
    ui->listWidget->addItem(log);

    //信号槽：处理响应
    connect(clientSocket, &QTcpSocket::readyRead, this, [=]()
    {
        //a)取出数据,因为readall返回的是一个QByteArray,通过赋值可以转化为QString
        QString request = clientSocket->readAll();
        //b)根据请求生成响应
        const QString& response = process(request);
        //c)将响应发送到客户端
        clientSocket->write(response.toUtf8());
        //d)将数据显示到窗口
        QString log = "[" + clientSocket->peerAddress().toString() + ":" + clientSocket->peerPort() + "]" + "req :" + request + "res :" + response;
        ui->listWidget->addItem(log);
    });

    //信号槽处理：当客户端退出链接的时候
    connect(clientSocket, &QTcpSocket::disconnected, this, [=]()
    {
        QString log = "[" + clientSocket->peerAddress().toString() + ":" + clientSocket->peerPort()+ "] 客户端下线";
        //显示到窗口
        ui->listWidget->addItem(log);
        //在下一次循环，才delete比直接delete安全
        clientSocket->deleteLater();
    });
}

QString Widget::process(QString request)
{
    return request;
}

