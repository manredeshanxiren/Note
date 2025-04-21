#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , listUpdater(new ListUpdater(this))
{
    ui->setupUi(this);

    // 初始化文件列表
    listUpdater->fetchFileList(ui->fileListWidget);

    // 连接信号和槽
    connect(listUpdater, &ListUpdater::fileListUpdated, this, [this]() {
        ui->statusbar->showMessage("File list updated.");
    });

    connect(listUpdater, &ListUpdater::fileUploaded, this, [this](bool success) {
        if (success) {
            ui->statusbar->showMessage("File uploaded successfully.");
            listUpdater->fetchFileList(ui->fileListWidget);
        } else {
            QMessageBox::warning(this, "Upload Error", "Failed to upload file.");
        }
    });

    connect(listUpdater, &ListUpdater::fileDeleted, this, [this](bool success) {
        if (success) {
            ui->statusbar->showMessage("File deleted successfully.");
            listUpdater->fetchFileList(ui->fileListWidget);
        } else {
            QMessageBox::warning(this, "Delete Error", "Failed to delete file.");
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_uploadButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select File to Upload");
    if (!filePath.isEmpty()) {
        listUpdater->uploadFile(filePath);
    }
}

void MainWindow::on_refreshButton_clicked()
{
    listUpdater->fetchFileList(ui->fileListWidget);
}

void MainWindow::on_deleteButton_clicked()
{
    QListWidgetItem *item = ui->fileListWidget->currentItem();
    if (item) {
        QString fileName = item->text();
        listUpdater->deleteFile(fileName, ui->fileListWidget);
    }
}
