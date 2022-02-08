#include "mainwindow.h"
#include "Util/logger.h"
#include <QApplication>
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
#include<QTextCodec>
#include<QFontDatabase>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    int nIndex = QFontDatabase::addApplicationFont("/opt/config/fonts/MSYHBD.TTF");
    if (-1 == nIndex)
    {
        QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));
        if (strList.count() > 0)
        {
            QFont fontThis(strList.at(0));
            a.setFont(fontThis);
        }
    }
    //qInstallMsgHandler(customMessageHandler);
#else
    qInstallMessageHandler(customMessageHandler);
#endif
    MainWindow w;
    //w.setStyleSheet("MainWindow{ border-image: url(:/images/background2.png);}");
    w.showMaximized();
    return a.exec();
}
