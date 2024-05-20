#include "widget.h"
#include "ui_widget.h"

#include <QMessageBox>
#include <QNetworkDatagram>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //创建套接字对象, 这里我们将对象挂到对象树上
    //就不用去手动释放了
    socket = new QUdpSocket(this);

    this->setWindowTitle("服务器");

    //使用信号槽机制来实现请求处理
    connect(socket, &QUdpSocket::readyRead, this, &Widget::requestHandle);

    //绑定端口号---先链接信号槽再绑定端口号。
    //绑定好端口号意味着，已经可以接受客户端的请求了
    //那么在这之前我们应该做好处理请求的准备
    bool ret = socket->bind(QHostAddress::Any, 9090);
    if(!ret) //意味着套接字绑定失败，因为一个端口号只能绑定一个进程，所以是有可能绑定失败的
    {
         QMessageBox::critical(this, "服务器出错", socket->errorString());
         return ;
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::requestHandle()
{
    //1.读取请求并解析
    const QNetworkDatagram& requestdatagram = socket->receiveDatagram();
    QString request = requestdatagram.data();
    //2.根据请求计算响应
    const QString& response = process(request);

    //3.把响应写回给客户端
    QNetworkDatagram responsedatagram(response.toUtf8(), requestdatagram.senderAddress(), requestdatagram.senderPort());
    socket->writeDatagram(responsedatagram);

    //将信息展示再listbox
    QString log = "[" + requestdatagram.senderAddress().toString() + ":" + QString::number(requestdatagram.senderPort()) + "]" + "req:" + request
            + "res:" + response;
    ui->listWidget->addItem(log);
}

QString Widget::process(const QString& request)
{
    return request;
}

