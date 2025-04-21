#ifndef LISTUPDATER_H
#define LISTUPDATER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QUrl>
#include <QListWidget>

class ListUpdater : public QObject
{
    Q_OBJECT
public:
    explicit ListUpdater(QObject *parent = nullptr);

    // 获取文件列表
    void fetchFileList(QListWidget *fileListWidget);

    // 上传文件
    void uploadFile(const QString &filePath);

    // 删除文件
    void deleteFile(const QString &fileName, QListWidget *fileListWidget);

signals:
    void fileListUpdated();
    void fileUploaded(bool success);
    void fileDeleted(bool success);

private slots:
    void onListReplyFinished(QNetworkReply *reply);
    void onUploadReplyFinished(QNetworkReply *reply);
    void onDeleteReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager networkManager;
};

#endif // LISTUPDATER_H
