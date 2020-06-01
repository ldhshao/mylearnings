#ifndef TESTMYSQL_H
#define TESTMYSQL_H
#include <QThread>

//bool testWriteRead();
//bool testMoreWriteRead(int iCnt);
bool testWriteDatabase();
bool testReadDatabase();

#if 0
//上下机芯传感器状态刷新线程
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
#endif

#endif // TESTMYSQL_H
