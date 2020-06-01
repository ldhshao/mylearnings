#include "mainwindow.h"
#include "testtable.h"
#include "testmysql.h"
#include "mythread.h"
#include <QApplication>
#include <QDebug>

void testByteArray()
{
    unsigned char bArr[5] = {1, 2, 0, 3, 4};
    unsigned char bArr2[5] = {0};
    QByteArray byArr = QByteArray::fromRawData(reinterpret_cast<const char*>(bArr), sizeof(bArr));
    char *pc = byArr.data();
   // while(pc){
   //     qDebug()<<static_cast<int>(*pc++)<<" ";
    //}
    memcpy(bArr2, byArr.data(), static_cast<size_t>(byArr.count()));
    for (int i = 0; i < sizeof(bArr2); i++){
        qDebug()<<bArr2[i]<<" ";
    }

    QByteArray byArr2;
    byArr2[0] = 1;
    byArr2[1] = 3;
    byArr2[2] = 0;
    byArr2[3] = 5;
    byArr2[4] = 7;
    qDebug()<<"count "<<byArr2.count()<<" ";
    memcpy(bArr2, byArr2.data(), static_cast<size_t>(byArr2.count()));
    for (int i = 0; i < sizeof(bArr2); i++){
        qDebug()<<bArr2[i]<<" ";
    }
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if 0
    testByteArray();

    CoreTest test[50];
    int iTestCnt = 0;
    //createTestTable();
    //initTestTable();
    iTestCnt = GetData(test, 50);
    if (0 < iTestCnt){
        for(int i = 0; i < iTestCnt; i++){
            qDebug()<<"index "<<i<<" "<<test[i].uiBeginTime<<" "<<test[i].uiBeginTime<<" "<<test[i].bResult<<" "<<test[i].bErrCode<<"\n";
        }
    }
#endif
    //testWriteDatabase();
    //testReadDatabase();
    CWorkerThread worker;
    worker.startThread();

    //testMoreWriteRead(100);
    //CWorkerThread worker;
    //worker.startThread();

    MainWindow w;
    w.show();

    return a.exec();
}
