#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QCryptographicHash>
#include <QFile>
#include <QFileDialog>
#include <QHttpMultiPart>
#include <QHttpPart>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , networkManager(new QNetworkAccessManager(this))
    , correctPassword("123456")
    , passwordHash(QCryptographicHash::hash(correctPassword.toUtf8(), QCryptographicHash::Md5).toHex())
{
    ui->setupUi(this);
    ui->mainContainer->setVisible(false);

    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::on_fileListDownloadFinished);
    loadFileList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginButton_clicked()
{
    QString inputPassword = ui->passwordInput->text();
    QString inputHash = QCryptographicHash::hash(inputPassword.toUtf8(), QCryptographicHash::Md5).toHex();
    if (inputHash == passwordHash) {
        showMainContent();
    } else {
        QMessageBox::warning(this, "Error", "Incorrect password. Please try again.");
    }
}

void MainWindow::showMainContent()
{
    ui->passwordContainer->setVisible(false);
    ui->mainContainer->setVisible(true);
}

void MainWindow::loadFileList()
{
    QNetworkRequest request(QUrl("http://117.72.37.100:9090/listshow"));
    networkManager->get(request);
}

void MainWindow::on_uploadButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select File to Upload");
    if (fileName.isEmpty()) {
        return;
    }

    QFile *file = new QFile(fileName);
    if (!file->open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "Cannot open file for reading.");
        delete file;
        return;
    }

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/octet-stream"));
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\"" + file->fileName() + "\""));
    filePart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(filePart);

    QNetworkRequest request(QUrl("http://117.72.37.10:9090/upload"));
    QNetworkReply *reply = networkManager->post(request, multiPart);
    multiPart->setParent(reply);
    connect(reply, &QNetworkReply::finished, this, &MainWindow::on_fileUploadFinished);
}

void MainWindow::on_fileListDownloadFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::warning(this, "Error", "Failed to load file list.");
        reply->deleteLater();
        return;
    }

    QString fileListHtml = reply->readAll();
    ui->fileList->setHtml(fileListHtml);
    reply->deleteLater();
}

void MainWindow::on_fileUploadFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QMessageBox::information(this, "Success", "File uploaded successfully.");
        loadFileList();
    } else {
        QMessageBox::warning(this, "Error", "Failed to upload file.");
    }
    reply->deleteLater();
}
