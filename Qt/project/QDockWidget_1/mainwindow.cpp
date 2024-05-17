#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDockWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //添加一个子窗口对象
    QDockWidget* dockwidget = new QDockWidget();
    //使用dockwidget来添加到主窗口中
    //第一个参数是浮动窗口的位置
    this->addDockWidget(Qt::LeftDockWidgetArea ,dockwidget);

    //给浮动窗口设置标题
    dockwidget->setWindowTitle("我是一个浮动窗口");

    QWidget* container = new QWidget();
    dockwidget->setWidget(container);

    QVBoxLayout* layout = new QVBoxLayout();
    container->setLayout(layout);

    QLabel* label1 = new QLabel("这是一个标签");
    QPushButton* button1 = new QPushButton("按钮");
    layout->addWidget(label1);
    layout->addWidget(button1);

}

MainWindow::~MainWindow()
{
    delete ui;
}

