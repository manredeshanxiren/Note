#ifndef MYLABEL_H
#define MYLABEL_H


#include<QLabel>

class MyLabel : public QLabel
{
public:
    //构造函数使用带QWidget*版本的
    //这样才能确保自己的对象能够加到对象树中
    MyLabel(QWidget* parent);
    ~MyLabel();
};

#endif // MYLABEL_H
