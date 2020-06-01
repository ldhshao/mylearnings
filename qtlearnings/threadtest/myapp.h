#ifndef MYAPP_H
#define MYAPP_H
#include "mythread.h"
#include <QGuiApplication>
#include <QList>
#include <QDebug>

class MyApp : public QGuiApplication
{
  Q_OBJECT
public:
    MyApp(int &argc, char **argv): QGuiApplication(argc, argv){

    }
    ~MyApp(){
        for(int i=0; i < m_listhread.size(); i++){
            m_listhread[i]->stopThread();
            m_listhread[i]->wait();
            delete m_listhread[i];
        }
        qDebug()<<"MyApp desconstructon";
    }
    bool appendThread(CWorkerThread* pThread){
        if(pThread) m_listhread.append(pThread);
        return true;
    }
private:
    QList<CWorkerThread*> m_listhread;
};
#endif // MYAPP_H
