#ifndef THREAD_H
#define THREAD_H

#include <QObject>

#include <QThread>

class Thread : public QThread
{
    Q_OBJECT
public:
    Thread();

    //重写父类的run函数
    void run();


signals:
    void notify();
};

#endif // THREAD_H
