#include "server.h"
#include <QByteArray>
#include <QDebug>

Server::Server(QObject *parent) : QObject(parent)
{
    m_piserver = new QTcpServer;
    m_pisocket = new QTcpSocket;
    StartListen(2016);
}

void Server::StartListen(int nPort)
{
    if(m_piserver->listen(QHostAddress::Any,nPort))
        qDebug() << "listen ok";
    else
        qDebug() << "listen err";
    connect(m_piserver,SIGNAL(newConnection()),this,SLOT(newClientConnect()));
}

void Server::newClientConnect()
{
    m_pisocket = m_piserver->nextPendingConnection();
    connect(m_pisocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(m_pisocket,SIGNAL(disconnected()),this,SLOT(disConnect()));
    qDebug() << "new client connect "<<m_pisocket->peerAddress().toString();
}

void Server::readMessage()
{
    qDebug() << "read client message";
    QTcpSocket *receive = (QTcpSocket*)sender();
    if(receive->bytesAvailable() > 0)
    {
        QByteArray buf;
        buf = receive->readAll();
        qDebug() << buf;
    }
}

void Server::disConnect()
{
    QTcpSocket *obj = (QTcpSocket*)sender();//掉线对象
    obj->close();
    qDebug() << "client disconnect "<<obj->peerAddress().toString();//打印出掉线对象的ip
}
