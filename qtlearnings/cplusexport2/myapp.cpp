#include "myapp.h"
#include <QtDebug>

MyThread::MyThread(QObject* parent)
    : QThread(parent)
{

}

MyApp::MyApp(int &argc, char **argv)
    : QGuiApplication(argc, argv)
{
    QObject::connect(this, &QGuiApplication::lastWindowClosed, this, &MyApp::onLastWindowClosed);
}

bool MyApp::appendThread(MyThread* pThread)
{
    if(NULL != pThread){
        m_listhread.push_back(pThread);
        return true;
    }

    return false;
}
void MyApp::onLastWindowClosed()
{
    qDebug()<<"MyApp onLastWindowClose begin";
    for(int i = 0; i < m_listhread.size(); i++){
        if(m_listhread[i]->running())
            m_listhread[i]->setRunning(false);
    }
    if(!m_listhread.empty()){
        while(true){
            if(!m_listhread[0]->isFinished())
                break;
            delete m_listhread[0];
            m_listhread.pop_front();
            //msleep(50);
        }
    }
    emit closing();
    qDebug()<<"MyApp onLastWindowClose end";
}
