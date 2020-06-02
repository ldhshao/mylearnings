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

class QSqlQuery;
class Worker : public QObject
{
    Q_OBJECT

public slots:
      void doWork(QSqlQuery* query);

signals:
      void resultReady(QSqlQuery *query);
};

class Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    Controller();
    ~Controller();
    void testreaddb();
public slots:
    void handleResults(QSqlQuery* query);
signals:
    void operate(QSqlQuery* query);
protected:
    QSqlQuery *m_pquery;
};
#endif // MYTHREAD_H
