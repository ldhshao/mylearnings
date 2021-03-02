#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

    void StartListen(int nPort);
private:
    QTcpServer *m_piserver;
    QTcpSocket *m_pisocket;

signals:

public slots:
    void newClientConnect();
    void readMessage();
    void disConnect();
};

#endif // SERVER_H
