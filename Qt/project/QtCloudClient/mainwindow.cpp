#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QCryptographicHash>
#include <QFile>
#include <QFileDialog>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

// MainWindow 构造函数
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , networkManager(new QNetworkAccessManager(this)) // 创建网络管理器
    , correctPassword("123456") // 预设的正确密码
    , passwordHash(QCryptographicHash::hash(correctPassword.toUtf8(), QCryptographicHash::Md5).toHex()) // 对正确密码进行MD5哈希
{
    ui->setupUi(this); // 设置UI
    ui->mainContainer->setVisible(false); // 隐藏主容器

    // 连接网络管理器的finished信号到槽函数on_fileListDownloadFinished
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::on_fileListDownloadFinished);

    // 加载文件列表
    loadFileList();
}

// MainWindow 析构函数
MainWindow::~MainWindow()
{
    delete ui;
}

// 登录按钮点击事件槽函数
void MainWindow::on_loginButton_clicked()
{
    QString inputPassword = ui->passwordInput->text(); // 获取用户输入的密码
    QString inputHash = QCryptographicHash::hash(inputPassword.toUtf8(), QCryptographicHash::Md5).toHex(); // 对用户输入的密码进行MD5哈希
    if (inputHash == passwordHash) { // 检查哈希值是否匹配
        showMainContent(); // 显示主内容
    } else {
        QMessageBox::warning(this, "Error", "Incorrect password. Please try again."); // 显示错误信息
    }
}

// 显示主内容
void MainWindow::showMainContent()
{
    ui->passwordContainer->setVisible(false); // 隐藏密码输入容器
    ui->mainContainer->setVisible(true); // 显示主容器
}

// 加载文件列表
void MainWindow::loadFileList()
{
    QNetworkRequest request(QUrl("http://117.72.37.100:9090/listshow")); // 创建网络请求
    networkManager->get(request); // 发送GET请求
}

// 上传按钮点击事件槽函数
void MainWindow::on_uploadButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select File to Upload"); // 打开文件选择对话框
    if (fileName.isEmpty()) { // 如果没有选择文件
        return;
    }

    QFile *file = new QFile(fileName); // 创建QFile对象
    if (!file->open(QIODevice::ReadOnly)) { // 打开文件
        QMessageBox::warning(this, "Error", "Cannot open file for reading."); // 显示错误信息
        delete file;
        return;
    }

    // 创建QHttpMultiPart对象
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/octet-stream")); // 设置内容类型
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\"" + file->fileName() + "\"")); // 设置内容处置
    filePart.setBodyDevice(file); // 设置文件设备
    file->setParent(multiPart); // 设置文件的父对象
    multiPart->append(filePart); // 添加文件部分到multiPart

    // 创建网络请求
    QNetworkRequest request(QUrl("http://117.72.37.10:9090/upload"));
    QNetworkReply *reply = networkManager->post(request, multiPart); // 发送POST请求
    multiPart->setParent(reply); // 设置multiPart的父对象
    connect(reply, &QNetworkReply::finished, this, &MainWindow::on_fileUploadFinished); // 连接请求完成信号到槽函数
}

// 文件列表下载完成事件槽函数
void MainWindow::on_fileListDownloadFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) { // 检查是否有错误
        QMessageBox::warning(this, "Error", "Failed to load file list."); // 显示错误信息
        reply->deleteLater();
        return;
    }

    QString fileListHtml = reply->readAll(); // 读取响应数据
    ui->fileList->setHtml(fileListHtml); // 设置文件列表的HTML内容
    reply->deleteLater(); // 删除响应对象
}

// 文件上传完成事件槽函数
void MainWindow::on_fileUploadFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender()); // 获取发出信号的对象
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) { // 检查是否成功上传
        QMessageBox::information(this, "Success", "File uploaded successfully."); // 显示成功信息
        loadFileList(); // 重新加载文件列表
    } else {
        QMessageBox::warning(this, "Error", "Failed to upload file."); // 显示错误信息
    }
    reply->deleteLater(); // 删除响应对象
}
