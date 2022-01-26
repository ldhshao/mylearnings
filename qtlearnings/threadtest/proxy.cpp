#include "proxy.h"
#include <QDebug>

void WorkerEx::slot_runCmd(struct SCmdInfo* pCmd)
{
    qDebug()<<"comand "<<pCmd->bCmd;
    emit cmdDone(pCmd, 1);
}
Proxy::Proxy(QObject *parent) : QObject(parent)
{
    WorkerEx *worker = new WorkerEx;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &Proxy::runCmd, worker, &WorkerEx::slot_runCmd);
    connect(worker, &WorkerEx::cmdDone, this, &Proxy::slot_cmdDone);
    workerThread.start();
}

Proxy::~Proxy()
{
    workerThread.quit();
    workerThread.wait();
}

bool Proxy::acceptCmd(struct SCmdInfo* pCmd)
{
    emit runCmd(pCmd);
    return true;
}

void Proxy::slot_cmdDone(struct SCmdInfo *pCmd, int iRes)
{
    if (nullptr != pCmd && nullptr != pCmd->callback)
    {
        pCmd->callback(pCmd, iRes);
    }
}

Proxy p;
Test::Test()
{
    FuncCmdFinished func = std::bind(&Test::cmdFinished, this, std::placeholders::_1, std::placeholders::_2);
    cmd1.bCmd = 1;
    cmd1.callback = func;
    cmd2.bCmd = 2;
    cmd2.callback = nullptr;
    p.acceptCmd(&cmd1);
    p.acceptCmd(&cmd2);
}

void Test::cmdFinished(struct SCmdInfo *pCmd, int iRes)
//void Test::cmdFinished(void *pCmd, int iRes)
{
    if(&cmd1 == pCmd)
        qDebug()<<"cmd1 Finished, result "<<iRes;
    if(&cmd2 == pCmd)
        qDebug()<<"cmd2 Finished, result "<<iRes;
}
