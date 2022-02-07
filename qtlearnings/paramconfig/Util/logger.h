#ifndef LOGGER_H
#define LOGGER_H
#include <QDebug>

#define LOG_FILE "paramconfig.log"
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
void customMessageHandler(QtMsgType type, const char *msg);
#else
void customMessageHandler(QtMsgType type, const QMessageLogContext &, const QString &msg);
#endif

#endif
