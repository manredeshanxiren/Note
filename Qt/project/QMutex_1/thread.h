#ifndef THREAD_H
#define THREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>

class Thread : public QThread
{
    Q_OBJECT
public:
    Thread();

    void run();


    static int num;

    //创建锁对象
    static QMutex mutex;
};

#endif // THREAD_H
