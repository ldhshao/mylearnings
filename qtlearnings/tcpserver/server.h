#ifndef SERVER_H
#define SERVER_H

#include <QObject>

class QTcpServer;

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

signals:

public slots:
    void sendFortune();

private:
    QTcpServer *tcpServer;
    QStringList fortunes;
};

#endif // SERVER_H
