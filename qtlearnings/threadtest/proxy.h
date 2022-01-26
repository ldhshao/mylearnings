#ifndef PROXY_H
#define PROXY_H
#include <QObject>
#include <QThread>
#include <functional>

struct SCmdInfo;
typedef std::function<void (struct SCmdInfo* pCmd, int iRes)> FuncCmdFinished;
//typedef std::function<void (void* pCmd, int iRes)> FuncCmdFinished;
struct SCmdInfo
{
    SCmdInfo ()
        : bCmd(0), bRes(0), pbSendData(nullptr), bSndDataLen(0), pbRcvData(nullptr), bRcvDataSize(0), bRcvDataLen(0),callback(nullptr){
    }
    SCmdInfo (unsigned char cmd, unsigned char* sendData, unsigned char sendDataLen, unsigned char* rcvData, unsigned char rcvDataSize)
        : bCmd(cmd), bRes(0), pbSendData(sendData), bSndDataLen(sendDataLen), pbRcvData(rcvData), bRcvDataSize(rcvDataSize), bRcvDataLen(0), callback(nullptr){
    }
    unsigned char bCmd;
    unsigned char bRes;
    unsigned char *pbSendData;
    unsigned char bSndDataLen;
    unsigned char *pbRcvData;
    unsigned char bRcvDataSize;
    unsigned char bRcvDataLen;
    FuncCmdFinished callback;
};

class WorkerEx : public QObject
{
    Q_OBJECT

public slots:
    void slot_runCmd(struct SCmdInfo *pCmd);

signals:
      void cmdDone(struct SCmdInfo *pCmd, int iRes);
};

class Proxy : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    explicit Proxy(QObject *parent = nullptr);
    ~Proxy();

    bool acceptCmd(struct SCmdInfo* pCmd);

signals:
    void runCmd(struct SCmdInfo* pCmd);

public slots:
    void slot_cmdDone(struct SCmdInfo *pCmd, int iRes);
};

class Test
{
public:
    Test();

    SCmdInfo cmd1;
    SCmdInfo cmd2;

    void cmdFinished(struct SCmdInfo* pCmd, int iRes);
    //void cmdFinished(void* pCmd, int iRes);
};

#endif // PROXY_H
