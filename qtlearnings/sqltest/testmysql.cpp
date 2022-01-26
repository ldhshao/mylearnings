#include "testmysql.h"
#include <random>
#include <ctime>
#include <cstring>
#include <fstream>
#include <QSql>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>
using namespace std;

bool connectDatabase(){
    return true;
}

bool testWriteRead(){
    //name size data
    int iSize[] = {16, 32, 64, 128, 256};
    unsigned char bBuf[256];
    unsigned char bReadBuf[256];
    char strFileName[256];
    QSqlDatabase dbconn = QSqlDatabase::addDatabase("QMYSQL");
    dbconn.setHostName("192.168.0.10");
    dbconn.setDatabaseName("mytest");
    dbconn.setUserName("root");
    dbconn.setPassword("");

    qDebug("database open status: %d\n", dbconn.open());

    QSqlError error = dbconn.lastError();
    qDebug() << error.text();

    minstd_rand0 gtmp(static_cast<unsigned>(time(0)));
    for (int i = 0; i < sizeof(iSize)/sizeof(int); i++){
        unsigned int iRand = gtmp();
        //construct date and save;
        ofstream ofs1;
        sprintf(strFileName, "%u_%d_ori.dat", iRand, iSize[i]);
        ofs1.open(strFileName);
        memset(bBuf, 0, sizeof(bBuf));
        for (int j = 0; j < iSize[i]; j++){
            bBuf[j] = static_cast<unsigned char>(gtmp());
            ofs1<<bBuf[j]<<" ";
        }
        ofs1.close();
        //write date into table
        QSqlQuery query;
        query.prepare("INSERT INTO t4 (name, size, data) VALUES (?, ?, ?)");
        query.addBindValue(strFileName);
        query.addBindValue(iSize[i]);
        query.addBindValue(QByteArray::fromRawData(reinterpret_cast<const char*>(bBuf), iSize[i]));
        if(!query.exec())
            qDebug()<<"Error: "<<query.lastError();
        //read date from table, compare data
        query.prepare("select size, data from t4 where name=?");
        query.addBindValue(strFileName);
        if(!query.exec())
            qDebug()<<"Error: "<<query.lastError();
        while(query.next()){
            QByteArray bArrTmp = query.value(1).toByteArray();
            memcpy(bReadBuf, bArrTmp.data(), bArrTmp.length());
            sprintf(strFileName, "%u_%d_read.dat", iRand, iSize[i]);
            ofs1.open(strFileName);
            if (bArrTmp.length() != iSize[i])
                qDebug()<<"Error: data len "<<bArrTmp.length()<<" iSize "<<iSize[i];
            for (int k = 0; k < bArrTmp.length(); k++){
                ofs1<<bReadBuf[k]<<" ";
            }
            ofs1.close();
        }

        for (int j = 0; j < iSize[i]; j++){
            if(bBuf[j] != bReadBuf[j]){
                qDebug()<<"failed: "<<strFileName<<" index "<<j;
                goto failPath;
            }
        }
    }
    dbconn.close();
    return true;

failPath:
    dbconn.close();
    return false;
}

bool testMoreWriteRead(int iCnt){
    for (int i = 0; i < iCnt; i++){
        if (!testWriteRead())
            break;
        qDebug()<<"finish test "<<i<<"\n";
    }
}
bool testWriteDatabase()
{
    QSqlDatabase dbconn = QSqlDatabase::addDatabase("QMYSQL");
    dbconn.setHostName("192.168.254.128");
    dbconn.setDatabaseName("mydatabase");
    dbconn.setUserName("root");
    dbconn.setPassword("rootroot");
    if(!dbconn.open())
    {
        qDebug()<<"failed to open mydatabase";
        return false;
    }

    QSqlQuery query("", dbconn);
    int iCnt = 0;
    char strsn[40] = {0};
    unsigned char btestitem = 0, bcmdcode = 0, bres = 0;
    unsigned char basensorinfo[255] = {0};
    unsigned char bapkts[1024] = {0};
    unsigned char barestime[8] = {0xe4, 0x07, 0x06, 0x01, 0x0f, 0x19, 0x03};
    while (iCnt < 10)
    {
        sprintf(strsn, "sn_%04d", iCnt++);
        btestitem = bcmdcode = static_cast<unsigned char>(iCnt);
        memset(basensorinfo, btestitem, sizeof(basensorinfo));
        memset(bapkts, btestitem, sizeof(bapkts));
        barestime[6] = 0x19 + (iCnt <<4);
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
    return true;
}
bool testReadDatabase()
{
    QSqlDatabase dbconn = QSqlDatabase::addDatabase("QMYSQL");
    dbconn.setHostName("192.168.254.128");
    dbconn.setDatabaseName("mydatabase");
    dbconn.setUserName("root");
    dbconn.setPassword("rootroot");
    if(!dbconn.open())
    {
        qDebug()<<"failed to open mydatabase";
        return false;
    }

    QSqlQuery query("", dbconn);
        query.prepare("select modulesn, testitem, starttime, responsetime, cmdcode, sensorinfo, result, pkts from moduletests");
    int iCnt = 0;
    char strsn[40] = {0};
    unsigned char btestitem = 0, bcmdcode = 0, bres = 0;
    unsigned char basensorinfo[255] = {0};
    unsigned char bapkts[1024] = {0};
    unsigned char barestime[8] = {0xe4, 0x07, 0x06, 0x01, 0x0f, 0x19, 0x03};
    if(!query.exec("select modulesn, testitem, starttime, responsetime, cmdcode, sensorinfo, result, pkts from moduletests"))
    {
        qDebug()<<"Error: "<<query.lastError();
        return false;
    }
    while (query.next())
    {
        qDebug()<<"item "<<iCnt++;
        QByteArray batmp;
        if (query.value(0).isNull()) qDebug()<<"0 is NULL";
        batmp = query.value(0).toByteArray();
        memcpy(strsn, batmp.data(), batmp.length());
        if (query.value(1).isNull()) qDebug()<<"1 is NULL";
        btestitem = query.value(1).toInt();
        if (query.value(4).isNull()) qDebug()<<"4 is NULL";
        bcmdcode = query.value(4).toInt();
        if (query.value(6).isNull()) qDebug()<<"6 is NULL";
        bres = query.value(6).toInt();
        if (query.value(2).isNull()) qDebug()<<"2 is NULL";
        QDateTime dt = query.value(2).toDateTime();
        if (query.value(3).isNull()) qDebug()<<"3 is NULL";
        batmp = query.value(3).toByteArray();
        memcpy(barestime, batmp.data(), batmp.length());
        if (query.value(5).isNull()) qDebug()<<"5 is NULL";
        batmp = query.value(5).toByteArray();
        memcpy(basensorinfo, batmp.data(), batmp.length());
        if (query.value(7).isNull()) qDebug()<<"7 is NULL";
        batmp = query.value(7).toByteArray();
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
    return true;
}
#if 0
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

    QSqlDatabase dbconn = QSqlDatabase::addDatabase("QMYSQL");
    dbconn.setHostName("192.168.254.128");
    dbconn.setDatabaseName("mydatabase");
    dbconn.setUserName("root");
    dbconn.setPassword("rootroot");
    if(!dbconn.open())
        qDebug()<<"failed to open mydatabase";

    QSqlQuery query("", dbconn);
    int iCnt = 0;
    char strsn[40] = {0};
    unsigned char btestitem = 0, bcmdcode = 0, bres = 0;
    unsigned char basensorinfo[256] = {0};
    unsigned char bapkts[1024] = {0};
    unsigned char barestime[8] = {0xe4, 0x07, 0x06, 0x01, 0x0f, 0x19, 0x03};
    while (!m_stop)
    {
        while (iCnt < 10)
        {
        sprintf(strsn, "sn_%04d", iCnt++);
        btestitem = bcmdcode = static_cast<unsigned char>(iCnt);
        memset(basensorinfo, btestitem, sizeof(basensorinfo));
        memset(bapkts, btestitem, sizeof(bapkts));
        barestime[6] = 0x19 + (iCnt <<4);
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
        //read date from table, compare data
        msleep(200);
    }
}
#endif
