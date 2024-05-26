#include "widget.h"
#include "ui_widget.h"

#include <QNetworkReply>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle("客户端");
    //构建QNetworkAccessManager对象
    manager = new QNetworkAccessManager(this);

}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    //1.获取输入框中的url
    QString url = ui->lineEdit->text();
    //2.构造http请求
    QNetworkRequest request(url);
    //3.向服务器发起请求
    QNetworkReply* response = manager->get(request);
    //4.通过信号槽来处理响应
    connect(response, &QNetworkReply::finished, this, [=]()
    {
        //如果请求成功
        if(response->error() == QNetworkReply::NoError)
        {
            //获取到响应信息，并显示到窗口
            QString html = response->readAll();
            ui->plainTextEdit->setPlainText(html);
        }
        else
        {
            //显示错误信息到窗口
            ui->plainTextEdit->setPlainText(response->errorString());
        }
        //将response延迟释放
        response->deleteLater();
    });

}
