#ifndef CDEVPOSMGR_H
#define CDEVPOSMGR_H
#include <list>
#include <QString>
using namespace std;

#define LINE_MAX    10
#define MACHINE_MAX 16
#define PORT_MAX    12


class CDevPosMgr
{
public:
    static CDevPosMgr* instance();

    QStringList getLineNames();
    list<list<bool>> getLinePorts(int l);
    list<bool> getMachinePorts(int l, int m);
    int        getMachineCount(int l);
    void       setPortValue(int l, int m, int port, bool use);

    enum {
      LINETYPE_CS = 0,
      LINETYPE_DIO,
      LINETYPE_CNT
    };
protected:
    CDevPosMgr();

    QString lineNames[LINE_MAX];
    uint8_t lineTypes[LINE_MAX];
    uint16_t* lineMachineCounts[LINE_MAX];
    bool      portInfos[LINE_MAX][MACHINE_MAX][PORT_MAX];
    uint16_t  lineCount;
    uint8_t   portCount[LINETYPE_CNT];
};

#endif // CDEVPOSMGR_H
