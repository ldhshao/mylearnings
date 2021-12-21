#include "cdevposmgr.h"
#include <QStringList>
#include <QDebug>

static uint16_t gCs1MachineCnt = 255;
static uint16_t gCs2MachineCnt = 255;
static uint16_t gCs3MachineCnt = 255;
static uint16_t gCs4MachineCnt = 255;
static uint16_t gDio1MachineCnt = 10;
static uint16_t gDio2MachineCnt = 10;
static uint16_t gDio3MachineCnt = 10;

CDevPosMgr* CDevPosMgr::instance()
{
    static CDevPosMgr mgr;
    return &mgr;
}

CDevPosMgr::CDevPosMgr()
{
    int i = 0;
    lineNames[i] = "CS1";
    lineTypes[i] = LINETYPE_CS;
    lineMachineCounts[i] = &gCs1MachineCnt;
    i++;
    lineNames[i] = "CS2";
    lineTypes[i] = LINETYPE_CS;
    lineMachineCounts[i] = &gCs2MachineCnt;
    i++;
    lineNames[i] = "CS3";
    lineTypes[i] = LINETYPE_CS;
    lineMachineCounts[i] = &gCs3MachineCnt;
    i++;
    lineNames[i] = "CS4";
    lineTypes[i] = LINETYPE_CS;
    lineMachineCounts[i] = &gCs4MachineCnt;
    i++;
    lineNames[i] = "DIO1";
    lineTypes[i] = LINETYPE_DIO;
    lineMachineCounts[i] = &gDio1MachineCnt;
    i++;
    lineNames[i] = "DIO2";
    lineTypes[i] = LINETYPE_DIO;
    lineMachineCounts[i] = &gDio2MachineCnt;
    i++;
    lineNames[i] = "DIO3";
    lineTypes[i] = LINETYPE_DIO;
    lineMachineCounts[i] = &gDio3MachineCnt;
    i++;
    lineCount = i;
    portCount[LINETYPE_CS] = 12;
    portCount[LINETYPE_DIO] = 12;
    memset(portInfos, 0, sizeof(portInfos));
}

QStringList CDevPosMgr::getLineNames()
{
    QStringList names;
    for(int i = 0; i < lineCount; i++){
        names.append(lineNames[i]);
    }
    qDebug()<<"line names: "<<names;
    return names;
}

list<list<bool>> CDevPosMgr::getLinePorts(int l, int portType)
{
    list<list<bool>> resList;
    if (l >= LINE_MAX || l >= lineCount || 0 > l)
        return resList;
    if (portType >= PORTTYPE_CNT || portType < PORTTYPE_IN)
        return resList;

    int ptCnt = portCount[lineTypes[l]];
    for (int j = 0; j < MACHINE_MAX && j < *(lineMachineCounts[l]); j++){
        list<bool> portList;
        for (int k = 0; k < PORT_MAX && k < ptCnt; k++){
            portList.push_back(portInfos[l][j][portType][k]);
        }
        resList.push_back(portList);
    }

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
    int ptCnt = portCount[lineTypes[l]];
    for (int k = 0; k < PORT_MAX && k < ptCnt; k++){
        portList.push_back(portInfos[l][m][portType][k]);
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

    int ptCnt = portCount[lineTypes[l]];
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
    if (isDevPointValid(devPt)){
        int l = get_line_from_dev_point(devPt) - 1;
        int m = get_machine_from_dev_point(devPt);
        int p = get_port_from_dev_point(devPt);

        setPortValue(l, m, portType, p, use);
    }
}

QString CDevPosMgr::makeStrDevPoint(uint32_t devPoint)
{
    QString strDevPoint;
    if (!isDevPointValid(devPoint))
        return strDevPoint;

    int l = get_line_from_dev_point(devPoint) - 1;
    int m = get_machine_from_dev_point(devPoint);
    int p = get_port_from_dev_point(devPoint);

    strDevPoint = lineNames[l];
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
        uint32_t tmp = strDevPoint.toUInt();
        if (isDevPointValid(tmp)) devPt = tmp;
    }

    return devPt;
}

bool CDevPosMgr::isDevPointValid(uint32_t devPoint)
{
    int l = get_line_from_dev_point(devPoint);
    int m = get_machine_from_dev_point(devPoint);
    int p = get_port_from_dev_point(devPoint);
    if (0 >= l || l> lineCount){
        return false;
    }
    if (0 > m || m>= getMachineCount(l)){
        return false;
    }
    if (0 > p || p >= portCount[lineTypes[l]]){
        return false;
    }

    return true;
}
