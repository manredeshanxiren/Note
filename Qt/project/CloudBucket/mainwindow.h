#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "listupdater.h"

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
    void on_uploadButton_clicked();
    void on_refreshButton_clicked();
    void on_deleteButton_clicked();

private:
    Ui::MainWindow *ui;
    ListUpdater *listUpdater;
};

#endif // MAINWINDOW_H
