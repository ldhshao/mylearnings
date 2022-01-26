#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>

class CWorkerThread : public QThread
{
    Q_OBJECT
public:
    CWorkerThread(QObject* parent=nullptr);

    void    startThread();
    void    stopThread();       //结束线程

protected:
    void run() override;  //线程任务

private:
    bool    m_stop;     //停止
};
#endif // MYTHREAD_H
