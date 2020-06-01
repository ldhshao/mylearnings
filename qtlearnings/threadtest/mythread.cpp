#include "mythread.h"
#include <QDebug>

CWorkerThread::CWorkerThread(QObject* parent)
    : QThread (parent)
{
}

void CWorkerThread::startThread()
{
    if(!this->isRunning())
        start();
}

void CWorkerThread::stopThread()
{
    m_stop = true;
}

void CWorkerThread::slot_quit()
{
    m_stop = true;
    qDebug()<<"m_stop set to true";
}

void CWorkerThread::run()
{
    m_stop = false;

    while (!m_stop)
    {
        //read date from table, compare data
        qDebug()<<"Worker Thread is running";
        msleep(2000);
    }

    quit();
    qDebug()<<"CWorkThread quited";
}
