#include "cdevposmgr.h"
#include "Util/PageCfg.h"
#include <QStringList>
#include <QDebug>

static uint16_t gMachineCnt = MACHINE_MAX;
#define LINE_SIZE LINE_MAX
static QString  gLineNames[] = {
    "CS1",
    "CS2",
    "DIO1",
    "CS1_BS",
    "CS2_BS",
    "COM1",
    "COM2",
    "COM3",
    "TCP",
    "CS3",
    "DIO2",
    "CS3_BS",
    "CS4",
    "DIO3",
    "CS4_BS",
    "CS5",
    "DIO4",
    "CS5_BS",
};
static QString  gLineMachineCountNames[] = {
    "CS1沿线数据.下设备数量",
    "CS2沿线数据.下设备数量",
    "DIO1沿线数据.下设备数量",
    "CS1沿线数据.闭锁输入设备台号",
    "CS2沿线数据.闭锁输入设备台号",//"CS2_BS",
    "COM1",
    "COM2",
    "COM3",
    "TCP",
    "CS3沿线数据.下设备数量",
    "DIO2沿线数据.下设备数量",
    "CS3_BS沿线数据.闭锁输入设备台号",
    "CS4沿线数据.下设备数量",
    "DIO3沿线数据.下设备数量",
    "CS4_BS沿线数据.闭锁输入设备台号",
    "CS5沿线数据.下设备数量",
    "DIO4沿线数据.下设备数量",
    "CS5_BS沿线数据.闭锁输入设备台号",
};

CDevPosMgr* CDevPosMgr::instance()
{
    static CDevPosMgr mgr;
    return &mgr;
}

CDevPosMgr::CDevPosMgr()
{
    inited = false;
    for (int i = 0; i < LINE_MAX; i++){
        if (i < LINE_SIZE){
            if (-1 < gLineNames[i].indexOf("DIO"))
                lineTypes[i] = LINETYPE_DIO;
            else if (-1 < gLineNames[i].indexOf("BS"))
                lineTypes[i] = LINETYPE_CSBS;
            else if (-1 < gLineNames[i].indexOf("COM"))
                lineTypes[i] = LINETYPE_COM;
            else if (-1 < gLineNames[i].indexOf("TCP"))
                lineTypes[i] = LINETYPE_TCP;
            else
                lineTypes[i] = LINETYPE_CS;
        }
        lineMachineCounts[i] = &gMachineCnt;
        lineMachineSizes[i] = 0;
    }
    lineCount = LINE_SIZE;
    portCount[LINETYPE_CS] = 12;
    portCount[LINETYPE_DIO] = 12;
    portCount[LINETYPE_COM] = 1000;
    portCount[LINETYPE_TCP] = 1;
    memset(portInfos, 0, sizeof(portInfos));
}

bool CDevPosMgr::initDevPosMgr(PageCfgList *devCfg)
{
    if (!inited){
        for (int i = 0; i < LINE_SIZE; i++){
            UiCfgItem* item = devCfg->findItemByName(gLineMachineCountNames[i]);
            if (nullptr != item){
                lineMachineCounts[i] = item->paramAddress();
                if (LINETYPE_CSBS == lineTypes[i]){
                    lineMachineSizes[i] = item->datacount();
                }
            }
        }
        inited = true;
    }

    return inited;
}
bool CDevPosMgr::isValidLine(int l)
{
    if (l >= LINE_MAX || l >= lineCount || 0 > l)
        return false;
    return true;
}
bool CDevPosMgr::isValidPortType(int portType)
{
    if (portType >= PORTTYPE_CNT || portType < PORTTYPE_IN)
        return false;
    return true;
}

QStringList CDevPosMgr::getLineNames()
{
    QStringList names;
    for(int i = 0; i < lineCount; i++){
        names.append(gLineNames[i]);
    }
    qDebug()<<"line names: "<<names;
    return names;
}

int CDevPosMgr::getLineNo(QString strLineName)
{
    for (int i =0; i < lineCount; i++){
        if (strLineName == gLineNames[i]){
            return i;
        }
    }

    return -1;
}
list<list<bool>> CDevPosMgr::getLinePorts(int l, int portType)
{
    list<list<bool>> resList;
    //if (l >= LINE_MAX || l >= lineCount || 0 > l)
    //    return resList;
    //if (portType >= PORTTYPE_CNT || portType < PORTTYPE_IN)
    //    return resList;

    //int ptCnt = portCount[lineTypes[l]];
    //for (int j = 0; j < MACHINE_MAX && j < *(lineMachineCounts[l]); j++){
    //    list<bool> portList;
    //    for (int k = 0; k < PORT_MAX && k < ptCnt; k++){
    //        portList.push_back(portInfos[l][j][portType][k]);
    //    }
    //    resList.push_back(portList);
    //}

    return resList;
}

list<bool> CDevPosMgr::getMachinePorts(int l, int m, int portType)
{
    list<bool> portList;
    if (l >= LINE_MAX || l >= lineCount || 0 > l)
        return portList;
    if (portType >= PORTTYPE_CNT || portType < PORTTYPE_IN)
        return portList;

    if (!isValidMachine(l, m)){
        return portList;
    }
    //int ptCnt = portCount[lineTypes[l]];
    int ptCnt = getPortCount(l, portType);
    for (int k = 0; k < PORT_MAX && k < ptCnt; k++){
        portList.push_back(portInfos[l][m][portType][k]);
    }

    return portList;
}
list<int> CDevPosMgr::getMachineAvailablePortNos(int l, int m, int portType)
{
    list<int> portList;
    if (l >= LINE_MAX || l >= lineCount || 0 > l)
        return portList;
    if (portType >= PORTTYPE_CNT || portType < PORTTYPE_IN)
        return portList;

    if (!isValidMachine(l, m)){
        return portList;
    }
    int ptCnt = getPortCount(l, portType);
    for (int k = 0; k < PORT_MAX && k < ptCnt; k++){
        if (!portInfos[l][m][portType][k])
            portList.push_back(k);
    }

    return portList;
}

int CDevPosMgr::getMachineCount(int l)
{
    int iCnt = -1;
    if (l >= LINE_MAX || l >= lineCount){
        return iCnt;
    }

    switch (lineTypes[l]) {
    case LINETYPE_CS:
    case LINETYPE_DIO:
        iCnt = *(lineMachineCounts[l]);
        break;
    case LINETYPE_CSBS:
        for (int i=0; i < lineMachineSizes[l]; i++){
            if (0 >= lineMachineCounts[l][i]){
                iCnt = i+1;
                break;
            }
        }
        break;
    case LINETYPE_COM:
        break;
    case LINETYPE_TCP:
        break;
    }
    return iCnt;
}

list<int> CDevPosMgr::getMachines(int l)
{
    list<int> macList;
    switch (lineTypes[l]) {
    case LINETYPE_CS:
    case LINETYPE_DIO:
        for (int i = 0; i < *(lineMachineCounts[l]); i++){
            macList.push_back(i);
        }
        break;
    case LINETYPE_CSBS:
        for (int i=0; i < lineMachineSizes[l]; i++){
            if (0 >= lineMachineCounts[l][i]){//0 is invalid
                break;
            }
            macList.push_back(lineMachineCounts[l][i] - 1);//make from 0
        }
        break;
    }
    return macList;
}

list<int> CDevPosMgr::getAvailableMachines(int l, int portType)
{
    list<int> macList0 = getMachines(l);
    list<int> macList1;
    for (auto it = macList0.begin(); it != macList0.end(); it++){
        list<int> ports = getMachineAvailablePortNos(l, *it, portType);
        if (0 < ports.size())
            macList1.push_back(*it);
    }
    return macList1;
}

uint8_t CDevPosMgr::getPortCount(int l, int portType)
{
    uint8_t pCnt = 0;
    switch (lineTypes[l]) {
    case LINETYPE_CS:
        pCnt = 12;
        break;
    case LINETYPE_CSBS:
        if (PORTTYPE_IN == portType)
            pCnt = 4;
        else
            pCnt = 2;
        break;
    case LINETYPE_DIO:
        pCnt = 12;
        break;
    case LINETYPE_COM:
        if (PORTTYPE_IN == portType)
            pCnt = 1120;
        break;
    default:
        break;
    }

    return pCnt;
}

void CDevPosMgr::setPortValue(int l, int m, int portType, int port, bool use)
{
    if (l >= LINE_MAX || l >= lineCount){
        return ;
    }
    if (portType >= PORTTYPE_CNT || portType < PORTTYPE_IN)
        return ;
    if (!isValidMachine(l, m)){
        return ;
    }

    int ptCnt = getPortCount(l, portType);
    if (port >= ptCnt){
        return ;
    }

    portInfos[l][m][portType][port] = use;
    qDebug()<<"line "<<l<<" machine "<<m<<" porttype "<<portType<<" port "<<port<<" val "<<use;
}

void CDevPosMgr::setDevPoint(uint32_t devPt, int portType, bool use)
{
    if (portType >= PORTTYPE_CNT || portType < PORTTYPE_IN)
        return ;
    if (isDevPointValid(devPt, portType)){
        int l = get_line_from_dev_point(devPt) - 1;
        int m = get_machine_from_dev_point(devPt);
        int p = get_port_from_dev_point(devPt);

        setPortValue(l, m, portType, p, use);
    }
}

QString CDevPosMgr::makeStrDevPoint(uint32_t devPoint)
{
    QString strDevPoint;
    int l = get_line_from_dev_point(devPoint) - 1;
    int m = get_machine_from_dev_point(devPoint);
    int p = get_port_from_dev_point(devPoint);

    if (0 > l || l>= lineCount){
        return strDevPoint;
    }

    strDevPoint = gLineNames[l];
    strDevPoint.append(".").append(QString::number(m+1)).append(".").append(QString::number(p+1));

    return strDevPoint;
}

uint32_t CDevPosMgr::makeDevPoint(QString strDevPoint)
{
    //deal int string
    //deal string CS1.12.3
    uint32_t devPt = 0;
    QStringList compList = strDevPoint.split('.');
    if (3 == compList.count()){
        int l = -1, m = -1, p = -1;
        bool bOK = false;
        for (int i = 0; i < lineCount; i++){
            if (compList[0] == gLineNames[i]){
                l = i+1; break;
            }
        }
        if (-1 < l){
            m = compList[1].toInt(&bOK) - 1;
        }
        if (bOK){
            p = compList[2].toInt(&bOK) - 1;
        }
        if (bOK){
            devPt = make_dev_point(l, m, p);
        }
    }else {
        bool bOK = false;
        uint32_t tmp = strDevPoint.toUInt(&bOK);
        if (bOK) devPt = tmp;
    }

    return devPt;
}

bool CDevPosMgr::isDevPointValid(uint32_t devPoint, int portType)
{
    int l = get_line_from_dev_point(devPoint);
    int m = get_machine_from_dev_point(devPoint);
    int p = get_port_from_dev_point(devPoint);
    if (0 >= l || l> lineCount){
        return false;
    }
    if (!isValidMachine(l-1, m)){
        return false;
    }
    if (0 > p || p >= getPortCount(l, portType)){
        return false;
    }

    return true;
}

bool CDevPosMgr::isValidMachine(int l, int m)
{
    switch (lineTypes[l]) {
    case LINETYPE_CS:
    case LINETYPE_DIO:
        if (0 <= m && m < *(lineMachineCounts[l])){
            return true;
        }
        break;
    case LINETYPE_CSBS:
        {
            list<int> macList = getMachines(l);
            for (auto it = macList.begin(); it != macList.end(); it++){
                if (m == *it) return true;
            }
        }
        break;
    case LINETYPE_COM:
        break;
    case LINETYPE_TCP:
        break;
    }
    return false;
}

bool CDevPosMgr::isDevPointAvailable(uint32_t devPoint, int portType)
{
    if (!isDevPointValid(devPoint, portType))
        return false;

    int l = get_line_from_dev_point(devPoint) - 1;
    int m = get_machine_from_dev_point(devPoint);
    int p = get_port_from_dev_point(devPoint);
    //check machine no

    return !(portInfos[l][m][portType][p]);
}
