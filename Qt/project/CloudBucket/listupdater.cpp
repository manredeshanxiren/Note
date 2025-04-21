#include "listupdater.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QHttpMultiPart>
#include <QMessageBox>

ListUpdater::ListUpdater(QObject *parent)
    : QObject(parent)
{
}

// 获取文件列表
void ListUpdater::fetchFileList(QListWidget *fileListWidget)
{
    QNetworkRequest request(QUrl("http://117.72.37.100:9090/listshow"));
    QNetworkReply *reply = networkManager.get(request);

    // 捕获 fileListWidget 以便在 lambda 内部使用
    connect(reply, &QNetworkReply::finished, this, [this, reply, fileListWidget]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
            QJsonArray fileArray = jsonResponse.array();

            fileListWidget->clear();
            for (const QJsonValue &file : fileArray) {
                fileListWidget->addItem(file.toString());
            }

            emit fileListUpdated();
        } else {
            QMessageBox::warning(nullptr, "Error", "Failed to fetch file list: " + reply->errorString());
        }
        reply->deleteLater();  // 确保 QNetworkReply 对象被删除
    });
}

// 上传文件
void ListUpdater::uploadFile(const QString &filePath)
{
    QUrl url("http://117.72.37.100:9090/upload");  // 修改为你的服务器地址
    QNetworkRequest request(url);

    QFile *file = new QFile(filePath);
    if (!file->open(QIODevice::ReadOnly)) {
        emit fileUploaded(false);
        return;
    }

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\"" + file->fileName() + "\""));
    filePart.setBodyDevice(file);
    file->setParent(multiPart); // 让 multiPart 负责删除 file

    multiPart->append(filePart);

    QNetworkReply *reply = networkManager.post(request, multiPart);
    multiPart->setParent(reply); // 让 reply 负责删除 multiPart
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onUploadReplyFinished(reply);
    });
}

// 删除文件
void ListUpdater::deleteFile(const QString &fileName, QListWidget *fileListWidget)
{
    QUrl url("http://117.72.37.100:9090/delete?filename=" + fileName);  // 修改为你的服务器地址
    QNetworkRequest request(url);
    QNetworkReply *reply = networkManager.deleteResource(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onDeleteReplyFinished(reply);
    });
}

void ListUpdater::onListReplyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
        QJsonArray fileArray = jsonResponse.array();

        // 解析文件列表并更新 QListWidget
        QListWidget *fileListWidget = qobject_cast<QListWidget *>(sender());
        fileListWidget->clear();

        for (const QJsonValue &file : fileArray) {
            fileListWidget->addItem(file.toString());
        }

        emit fileListUpdated();
    } else {
        QMessageBox::warning(nullptr, "Error", "Failed to fetch file list: " + reply->errorString());
    }
    reply->deleteLater();
}

void ListUpdater::onUploadReplyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        emit fileUploaded(true);
    } else {
        emit fileUploaded(false);
    }
    reply->deleteLater();
}

void ListUpdater::onDeleteReplyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        emit fileDeleted(true);
    } else {
        emit fileDeleted(false);
    }
    reply->deleteLater();
}
