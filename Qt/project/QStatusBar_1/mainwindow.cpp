#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStatusBar>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //有就获取，无则创建
    QStatusBar* statusbar = this->statusBar();

    //向窗口中设置状态栏
    this->setStatusBar(statusbar);

    //向状态栏中添加消息
    //statusbar->showMessage("这是一个状态消息", 2000);

    //给状态栏中添加子控件
    QLabel* label = new QLabel("我是一个标签");
    statusbar->addWidget(label);

    QProgressBar* progressbar = new QProgressBar();

    progressbar->setRange(0,100);
    progressbar->setValue(33);
    statusbar->addWidget(progressbar);

    QPushButton* button = new QPushButton("刷新");
    statusbar->addPermanentWidget(button);
}

MainWindow::~MainWindow()
{
    delete ui;
}

