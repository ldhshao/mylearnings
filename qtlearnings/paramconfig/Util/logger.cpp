#include "logger.h"
#include <QDate>
#include <QFile>
#include <QTextStream>

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
void customMessageHandler(QtMsgType type, const char *msg)
{
    QString txt;
    switch (type)
    {
    case QtDebugMsg:
        txt = QString(" %1").arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
        abort();
    }
    QTime l_time = QTime::currentTime();
    QDate l_Date = QDate::currentDate();
    QString l_strDate = l_Date.toString("MM.dd");
    QString l_strTime = l_time.toString("hh:mm:ss");
    l_strDate.append(" ");
    l_strDate.append(l_strTime);
    QFile outFile(LOG_FILE);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << l_strDate<< " " << txt << endl;
}
#else
void customMessageHandler(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    QString txt;
    switch (type)
    {
    case QtDebugMsg:
        txt = msg;
        break;
    case QtWarningMsg:
        txt = QString("Warning: ") + msg;
        break;
    case QtCriticalMsg:
        txt = QString("Critical: ") + msg;
        break;
    case QtFatalMsg:
        txt = QString("Fatal: ") + msg;
        abort();
    }
    QTime l_time = QTime::currentTime();
    QDate l_Date = QDate::currentDate();
    QString l_strDate = l_Date.toString("MM.dd");
    QString l_strTime = l_time.toString("hh:mm:ss");
    l_strDate.append(" ");
    l_strDate.append(l_strTime);
    QFile outFile(LOG_FILE);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << l_strDate<< " " << txt << endl;
}
#endif

