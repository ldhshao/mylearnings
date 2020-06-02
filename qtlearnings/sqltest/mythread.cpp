#include "mythread.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

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

void CWorkerThread::run()
{
    m_stop = false;

    QSqlDatabase dbconn = QSqlDatabase::addDatabase("QMYSQL", "write");
    dbconn.setHostName("192.168.254.128");
    dbconn.setDatabaseName("mydatabase");
    dbconn.setUserName("root");
    dbconn.setPassword("rootroot");
    if(!dbconn.open())
    {
        qDebug()<<"WorkerThread failed to open mydatabase, Error: "<<dbconn.lastError().text();
    }

    QSqlQuery query("", dbconn);
    int iCnt = 0;
    char strsn[40] = {0};
    unsigned char btestitem = 0, bcmdcode = 0, bres = 0;
    unsigned char basensorinfo[255] = {0};
    unsigned char bapkts[1024] = {0};
    unsigned char barestime[8] = {0xe4, 0x07, 0x06, 0x01, 0x0f, 0x19, 0x03, 0x0};
    while (!m_stop)
    {
        //read date from table, compare data
        qDebug()<<"Worker Thread is running";
        while (iCnt < 10)
        {
            sprintf(strsn, "sn_%04d", iCnt++);
            btestitem = bcmdcode = static_cast<unsigned char>(iCnt);
            memset(basensorinfo, btestitem, sizeof(basensorinfo));
            memset(bapkts, btestitem, sizeof(bapkts));
            barestime[5] = 0x19 + (iCnt <<4);
            query.prepare("INSERT INTO moduletests (modulesn, testitem, starttime, responsetime, cmdcode, sensorinfo, result, pkts) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
            query.addBindValue(strsn);
            query.addBindValue(btestitem);
            query.addBindValue(QDateTime::currentDateTime());
            query.addBindValue(QByteArray::fromRawData(reinterpret_cast<const char*>(barestime), sizeof(barestime)));
            query.addBindValue(bcmdcode);
            query.addBindValue(QByteArray::fromRawData(reinterpret_cast<const char*>(basensorinfo), sizeof(basensorinfo)));
            query.addBindValue(bres);
            query.addBindValue(QByteArray::fromRawData(reinterpret_cast<const char*>(bapkts), sizeof(bapkts)));
            if(!query.exec())
                qDebug()<<"Error: "<<query.lastError();
        }
        msleep(2000);
    }
    dbconn.close();
    quit();
}

void Worker::doWork(QSqlQuery* query)
{
    if (query->exec())
        emit resultReady(query);
    else
        qDebug()<<"Read Error: "<<query->lastError();
}

Controller::Controller()
{
    qDebug()<<"Controller construction";
    Worker *worker = new Worker;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &Controller::operate, worker, &Worker::doWork);
    connect(worker, &Worker::resultReady, this, &Controller::handleResults);
    workerThread.start();

    QSqlDatabase dbconn = QSqlDatabase::addDatabase("QMYSQL", "read");
    dbconn.setHostName("192.168.254.128");
    dbconn.setDatabaseName("mydatabase");
    dbconn.setUserName("root");
    dbconn.setPassword("rootroot");
    if(!dbconn.open())
    {
        qDebug()<<"Controller failed to open mydatabase, Error: "<<dbconn.lastError().text();
    }

    m_pquery = new QSqlQuery("select modulesn, testitem, starttime, responsetime, cmdcode, sensorinfo, result, pkts from moduletests", dbconn);
}
Controller::~Controller()
{
    workerThread.quit();
    workerThread.wait();
    delete m_pquery;
}
void Controller::testreaddb()
{
    qDebug()<<"emit operate qsqlquery";
    emit operate(m_pquery);
}
void Controller::handleResults(QSqlQuery* query)
{
    qDebug()<<"Controller handleResults";
    int iCnt = 0;
    char strsn[40] = {0};
    unsigned char btestitem = 0, bcmdcode = 0, bres = 0;
    unsigned char basensorinfo[255] = {0};
    unsigned char bapkts[1024] = {0};
    unsigned char barestime[8] = {0xe4, 0x07, 0x06, 0x01, 0x0f, 0x19, 0x03};
    while (query->next())
    {
        qDebug()<<"item "<<iCnt++;
        QByteArray batmp;
        if (query->value(0).isNull()) qDebug()<<"0 is NULL";
        batmp = query->value(0).toByteArray();
        memset(strsn, 0, sizeof(strsn));
        memcpy(strsn, batmp.data(), batmp.length());
        if (query->value(1).isNull()) qDebug()<<"1 is NULL";
        btestitem = query->value(1).toInt();
        if (query->value(4).isNull()) qDebug()<<"4 is NULL";
        bcmdcode = query->value(4).toInt();
        if (query->value(6).isNull()) qDebug()<<"6 is NULL";
        bres = query->value(6).toInt();
        if (query->value(2).isNull()) qDebug()<<"2 is NULL";
        QDateTime dt = query->value(2).toDateTime();
        if (query->value(3).isNull()) qDebug()<<"3 is NULL";
        batmp = query->value(3).toByteArray();
        memset(barestime, 0, sizeof(barestime));
        memcpy(barestime, batmp.data(), batmp.length());
        if (query->value(5).isNull()) qDebug()<<"5 is NULL";
        batmp = query->value(5).toByteArray();
        memset(basensorinfo, 0, sizeof(basensorinfo));
        memcpy(basensorinfo, batmp.data(), batmp.length());
        if (query->value(7).isNull()) qDebug()<<"7 is NULL";
        batmp = query->value(7).toByteArray();
        memset(bapkts, 0, sizeof(bapkts));
        memcpy(bapkts, batmp.data(), batmp.length());

        qDebug()<<"modulesn "<<strsn<<" testitem "<<btestitem<<" cmdcode "<<bcmdcode<<" bresult "<<bres;
        qDebug()<<"starttime "<<dt;
        qDebug()<<"responsetime "<<barestime[0]<<" "<<barestime[1]<<" "<<barestime[2]<<" "<<barestime[3]<<" "<<barestime[4]<<" "<<barestime[5]<<" "<<barestime[6]<<" "<<barestime[7];
        qDebug()<<"sensorinfo";
        for(int i = 0; i < sizeof(basensorinfo); i+=8)
            qDebug()<<basensorinfo[i] <<" "<<basensorinfo[i+1] <<" "<<basensorinfo[i+2] <<" "<<basensorinfo[i+3] <<" "<<basensorinfo[i+4] <<" "<<basensorinfo[i+5] <<" "<<basensorinfo[i+6] <<" "<<basensorinfo[i+7];
        qDebug()<<"packet";
        for(int i = 0; i < sizeof(bapkts); i+=8)
            qDebug()<<bapkts[i] <<" "<<bapkts[i+1] <<" "<<bapkts[i+2] <<" "<<bapkts[i+3] <<" "<<bapkts[i+4] <<" "<<bapkts[i+5] <<" "<<bapkts[i+6] <<" "<<bapkts[i+7];
    }
}
