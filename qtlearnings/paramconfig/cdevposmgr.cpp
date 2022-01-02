#include "cdevposmgr.h"
#include "Util/PageCfg.h"
#include <QStringList>
#include <QDebug>

static uint16_t gMachineCnt = MACHINE_MAX;
#define LINE_SIZE 11
static QString  gLineNames[] = {
    "CS1",
    "CS2",
    "CS3",
    "CS4",
    "CS1_BS",
    "CS2_BS",
    "CS3_BS",
    "CS4_BS",
    "DIO1",
    "DIO2",
    "DIO3"
};
static QString  gLineMachineCountNames[] = {
    "CS1下位机数量",
    "CS2下位机数量",
    "CS3下位机数量",
    "CS4下位机数量",
    "CS1_BS闭锁数量",
    "CS2_BS闭锁数量",
    "CS3_BS闭锁数量",
    "CS4_BS闭锁数量",
    "DIO1下位机数量",
    "DIO2下位机数量",
    "DIO3下位机数量"
};
static QString  gLineMachineTypeNames[] = {
    "",
    "",
    "",
    "",
    "CS1_BS闭锁设备",
    "CS2_BS闭锁设备",
    "CS3_BS闭锁设备",
    "CS4_BS闭锁设备",
    "",
    "",
    "",
};

CDevPosMgr* CDevPosMgr::instance()
{
    static CDevPosMgr mgr;
    return &mgr;
}

CDevPosMgr::CDevPosMgr()
{
    inited = false;
    bsDevVal = 0;
    for (int i = 0; i < LINE_MAX; i++){
        if (i < LINE_SIZE){
            if (-1 < gLineNames[i].indexOf("DIO"))
                lineTypes[i] = LINETYPE_DIO;
            else if (-1 < gLineNames[i].indexOf("BS"))
                lineTypes[i] = LINETYPE_CSBS;
            else
                lineTypes[i] = LINETYPE_CS;
        }
        lineMachineCounts[i] = &gMachineCnt;
        lineMachineTypes[i] = nullptr;
    }
    lineCount = LINE_SIZE;
    portCount[LINETYPE_CS] = 12;
    portCount[LINETYPE_DIO] = 12;
    memset(portInfos, 0, sizeof(portInfos));
}

bool CDevPosMgr::initDevPosMgr(PageCfgList *devCfg)
{
    if (!inited){
        for (int i = 0; i < LINE_SIZE; i++){
            UiCfgItem* item = devCfg->findItemByName(gLineMachineCountNames[i]);
            if (nullptr != item)
                lineMachineCounts[i] = item->paramAddress();
            if (!gLineMachineTypeNames[i].isEmpty()){
                UiCfgItem *typeItem = devCfg->findItemByName(gLineMachineTypeNames[i]);
                if (nullptr != typeItem){
                    lineMachineTypes[i] = typeItem->paramAddress();
                    QStringList params = (dynamic_cast<ComboboxCfgItem*>(typeItem))->params();
                    if (2 == params.count()){
                        if (-1 < params[0].indexOf("不"))
                            bsDevVal = 1;
                    }
                }
            }
        }
        inited = true;
    }

    return inited;
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

    if (m >= MACHINE_MAX || m >= *(lineMachineCounts[l]) || 0 > m){
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

    if (m >= MACHINE_MAX || m >= *(lineMachineCounts[l]) || 0 > m){
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

    iCnt = *(lineMachineCounts[l]);
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
        if (nullptr != lineMachineTypes[l]){
            for (int i = 0; i < *lineMachineCounts[l]; i++){
                if (*(lineMachineTypes[l] + i) == bsDevVal)
                    macList.push_back(i);
            }
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
    if (m >= MACHINE_MAX || m >= *(lineMachineCounts[l])){
        return ;
    }
    if (portType >= PORTTYPE_CNT || portType < PORTTYPE_IN)
        return ;

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

    if (0 > l || l> lineCount){
        return strDevPoint;
    }
    if (0 > m || m>= getMachineCount(l)){
        return strDevPoint;
    }

    strDevPoint = gLineNames[l];
    strDevPoint.append(QString::asprintf(".%d.%d", m+1, p+1));

    return strDevPoint;
}

uint32_t CDevPosMgr::makeDevPoint(QString strDevPoint)
{
    //deal int string
    //deal string CS1.12.3
    uint32_t devPt = 0;
    QStringList compList = strDevPoint.split('.');
    if (3 == compList.count()){
        //
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
    if (0 > m || m>= getMachineCount(l)){//may wrong sometime
        return false;
    }
    if (0 > p || p >= getPortCount(l, portType)){
        return false;
    }

    return true;
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
