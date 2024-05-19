#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPlainTextEdit>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //获取对应的菜单栏
    QMenuBar* menubar = this->menuBar();

    //创建一个菜单，并且将菜单设置到菜单栏
    QMenu* menu = new QMenu("文件");
    menubar->addMenu(menu);

    //创建一个选项，设置到对应的菜单中
    QAction* action1 = new QAction("打开");
    QAction* action2 = new QAction("保存");
    menu->addAction(action1);
    menu->addAction(action2);

    //指定一个输入框
    edit = new QPlainTextEdit();
    //放大文字
    QFont font;
    font.setPixelSize(20);
    edit->setFont(font);
    this->setCentralWidget(edit);

    //将对应的选项信号和槽对应起来
    connect(action1, &QAction::triggered, this, &MainWindow::action1Handle);
    connect(action2, &QAction::triggered, this, &MainWindow::action2Handle);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::action1Handle()
{
    //1.弹出打开文件的对话框，让用户选择打开哪一个
    QString path = QFileDialog::getOpenFileName();

    //2.把文件名显示到状态栏里
    QStatusBar* statusbar = this->statusBar();
    statusbar->showMessage(path);

    //3.根据用户选择的文件去构造对应的QFile对象
    QFile file(path);
    //用只读的方式打开
    bool ret = file.open(QFile::ReadOnly);
    if(ret == false)
    {
        statusbar->showMessage(path + "打开失败");
        return ;
    }

    //4.读取文件
    QString text = file.readAll();

    //5.关闭文件
    file.close();

    //将读取到的文件设置到编辑框
    edit->setPlainText(text);

}

void MainWindow::action2Handle()
{
    //1.先他拿出保存文件的对话框
    QString path = QFileDialog::getSaveFileName(this);

    //2.在状态栏中显示这个文件
    QStatusBar* statusbar = this->statusBar();
    statusbar->showMessage(path);

    //3.根据用户选择的路径，构造一个对应的QFile对象
    QFile file(path);

    bool ret = file.open(QFile::WriteOnly);
    if(ret == false)
    {
        statusbar->showMessage(path + "保存失败");
    }

    //4.写文件
    const QString & text = edit->toPlainText();
    file.write(text.toUtf8());

    //5.关闭文件
    file.close();
}

