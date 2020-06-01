#include "testtable.h"
#include <QtSql>

int createTestTable()
{
    if (!QSqlDatabase::drivers().contains("QSQLITE"))
        return ERROR_NO_SQLITE;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("tests.db");
    if (!db.open())
        return ERROR_OPEN_DB;

    QStringList tables = db.tables();
    if (tables.contains("tests", Qt::CaseInsensitive))
        return true;

    QSqlQuery q;
    if (!q.exec(QLatin1String("create table tests(id integer primary key, begintime integer, endtime integer, result integer, errorcode integer, corestatus blob)")))
    {
        QSqlError err = q.lastError();
        return ERROR_CREATE_TABLE;
    }

    return 0;
}
bool initTestTable()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("tests.db");

    if (!db.open())
        return false;

    QSqlQuery q;
    if (!q.prepare(QLatin1String("insert into tests(begintime, endtime, result, errorcode, corestatus) values(?, ?, ?, ?, ?)")))
    {
        QSqlError err = q.lastError();
        return false;
    }
    q.addBindValue(1);
    q.addBindValue(10);
    q.addBindValue(0);
    q.addBindValue(0);
    q.addBindValue(QByteArray("123456789012345"));
    q.exec();
    q.addBindValue(11);
    q.addBindValue(20);
    q.addBindValue(1);
    q.addBindValue(9);
    q.addBindValue(QByteArray("abcdefghijk"));
    q.exec();

    return true;
}
int GetData(CoreTest* pTest, unsigned uiTestCount)
{
    int iCnt = 0;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("tests.db");

    if (!db.open())
        return iCnt;

    QSqlQuery q("select begintime, endtime, result, errorcode, corestatus from tests");
    while( q.next()){
        pTest[iCnt].uiBeginTime = q.value(0).toUInt();
        pTest[iCnt].uiEndTime = q.value(1).toUInt();
        pTest[iCnt].bResult = q.value(2).toUInt();
        pTest[iCnt].bErrCode = q.value(3).toUInt();
        //memccpy();
        iCnt++;
        if (iCnt >= uiTestCount)
            break;
    }
    return iCnt;
}
