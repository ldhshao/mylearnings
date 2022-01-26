#ifndef WORKER_H
#define WORKER_H
#include <QThread>
#include <QDebug>

class Worker : public QObject
{
    Q_OBJECT

public slots:
      void doWork(const QString &parameter) {
          QString result;
          /* ... here is the expensive or blocking operation ... */
          qDebug()<<"param "<<parameter;
          result = "param has dealed";
          emit resultReady(result);
      }

signals:
      void resultReady(const QString &result);
};

class Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    Controller() {
        Worker *worker = new Worker;
        worker->moveToThread(&workerThread);
        connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &Controller::operate, worker, &Worker::doWork);
        connect(worker, &Worker::resultReady, this, &Controller::handleResults);
        workerThread.start();
    }
    ~Controller() {
        workerThread.quit();
        workerThread.wait();
    }
    void dealstring(const QString& strparam){
        emit operate(strparam);
    }
public slots:
    void handleResults(const QString &res){
        qDebug()<<res;
    }
signals:
    void operate(const QString &);
};


#endif // WORKER_H
