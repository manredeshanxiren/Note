#include "widget.h"
#include "ui_widget.h"

#include <QNetworkDatagram>

const QString SERVER_IP = "127.0.0.1";
const quint16 SERVER_PORT = 9090;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //创建套接字
    socket = new QUdpSocket(this);

    this->setWindowTitle("客户端");

    //利用信号槽处理服务器返回的数据
    connect(socket, &QUdpSocket::readyRead, this, &Widget::responseHandle);

}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    //获取输入框中的内容
    QString request = ui->lineEdit->text();
    //构造udp请求数据
    QNetworkDatagram requestdatagram = QNetworkDatagram(request.toUtf8(), QHostAddress(SERVER_IP), SERVER_PORT);
    //发送数据
    socket->writeDatagram(requestdatagram);
    //将发送的数据同步到列表框中
    ui->listWidget->addItem("客户端说：" + request);
    //将文本框清空
    ui->lineEdit->setText("");
}

void Widget::responseHandle()
{
    //接收服务器发来的数据
    const QNetworkDatagram& responsedatagram = socket->receiveDatagram();
    QString response = responsedatagram.data();
    //将对应的数据显示到窗口
    ui->listWidget->addItem("服务端说：" + response);
}
