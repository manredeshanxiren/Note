#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QToolBar>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QToolBar* toolBar1 = new QToolBar();
    QToolBar* toolBar2 = new QToolBar();

    this->addToolBar(toolBar1);
    this->addToolBar(toolBar2);

    QAction* action1 = new QAction("动作1");
    QAction* action2 = new QAction("动作2");
    QAction* action3 = new QAction("动作3");
    QAction* action4 = new QAction("动作4");

    toolBar1->addAction(action1);
    toolBar1->addAction(action2);
    toolBar2->addAction(action3);
    toolBar2->addAction(action4);

}

MainWindow::~MainWindow()
{
    delete ui;
}

