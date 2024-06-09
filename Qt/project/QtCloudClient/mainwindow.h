#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loginButton_clicked();
    void on_uploadButton_clicked();
    void on_fileListDownloadFinished(QNetworkReply* reply);
    void on_fileUploadFinished();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *networkManager;
    QString correctPassword;
    QString passwordHash;

    void loadFileList();
    void showMainContent();
};

#endif // MAINWINDOW_H
