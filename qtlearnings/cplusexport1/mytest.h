#ifndef MYTEST_H
#define MYTEST_H
#include "message.h"
#include <QTimer>
#include <QThread>
#include <QtDebug>

class MyTest : public QObject
{
    Q_OBJECT
public:
    void setMessage(Message *msg){
        m_msg = msg;
    }
    void test(){
        QTimer *timer = new QTimer();
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));
        timer->start(2000);
    }

public slots:
    void update(){
        if(m_msg){
            static int i = 0;
            m_msg->setAuthor(QString("%1").arg(i++));
        }
    }

private:
    Message *m_msg;
};

class WorkerThreadTest : public QThread
  {
    Q_OBJECT
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
public:
    void setRunning(const bool &a){
        m_running = a;
        emit runningChanged();
    }
    bool running() const{
        return m_running;
    }
    void setMessage(Message *msg){
        m_msg = msg;
    }
    void run() override {
        int i = 0;
        while(m_running && i < 50){
            if(m_msg){
                m_msg->setAuthor(QString("%1").arg(i++));
                qDebug()<<"Thread: "<<m_msg->author();
            }
            msleep(2000);
        }
    }
signals:
    void runningChanged();
private:
    bool m_running;
    Message *m_msg;
  };

#endif // MYTEST_H
