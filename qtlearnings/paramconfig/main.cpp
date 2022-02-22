#include "mainwindow.h"
#include "Util/logger.h"
#include <QApplication>
#include<QFontDatabase>
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
#include<QTextCodec>
#endif
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    //qInstallMsgHandler(customMessageHandler);
#else
    //qInstallMessageHandler(customMessageHandler);
#endif
    int nIndex = QFontDatabase::addApplicationFont("/opt/config/fonts/MSYHBD.TTF");
    if (-1 < nIndex)
    {
        QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));
        if (strList.count() > 0)
        {
            QFont fontThis(strList.at(0));
            a.setFont(fontThis);
        }
        for (int i = 0; i < strList.count(); i++) qDebug()<<i<<strList[i];
    }

    MainWindow w;
    //w.setStyleSheet("MainWindow{ border-image: url(:/images/background2.png);}");
    w.showMaximized();
    return a.exec();
}
