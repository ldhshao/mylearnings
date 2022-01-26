#ifndef CDEVPOSMGR_H
#define CDEVPOSMGR_H
#include <list>
#include <QString>
using namespace std;

#define LINE_MAX    18
#define MACHINE_MAX 255
#define PORT_MAX    2048
#define LINE_BITS_COUNT    8
#define MACHINE_BITS_COUNT 8
#define PORT_BITS_COUNT    16
#define LINE_BITS_MASK     0xFF
#define MACHINE_BITS_MASK  0xFF
#define PORT_BITS_MASK     0xFFFF
#define LINE_BITS_SHIFT    (MACHINE_BITS_COUNT + PORT_BITS_COUNT)
#define make_dev_point(l,m,p)         ((((l)&LINE_BITS_MASK)<<LINE_BITS_SHIFT) + (((m)&MACHINE_BITS_MASK)<<PORT_BITS_COUNT) + ((p)&PORT_BITS_MASK))
#define get_line_from_dev_point(p)    (((p)>>LINE_BITS_SHIFT) & LINE_BITS_MASK)
#define get_machine_from_dev_point(p) (((p)>>PORT_BITS_COUNT) & MACHINE_BITS_MASK)
#define get_port_from_dev_point(p)    ((p) & PORT_BITS_MASK)

class PageCfgList;
class CDevPosMgr
{
public:
    static CDevPosMgr* instance();

    QStringList getLineNames();
    int         getLineNo(QString strLineName);
    list<list<bool>> getLinePorts(int l, int portType);
    list<bool> getMachinePorts(int l, int m, int portType);//port use val
    list<int> getMachineAvailablePortNos(int l, int m, int portType);//port index
    int        getMachineCount(int l);
    list<int>  getMachines(int l);
    list<int>  getAvailableMachines(int l, int portType);
    void       setPortValue(int l, int m, int portType, int port, bool use);
    void       setDevPoint(uint32_t devPt, int portType, bool use);
    QString    makeStrDevPoint(uint32_t devPoint);
    uint32_t   makeDevPoint(QString strDevPoint);
    bool       isDevPointValid(uint32_t devPoint, int portType);
    bool       isDevPointAvailable(uint32_t devPoint, int portType);
    bool       initDevPosMgr(PageCfgList* devCfg);//after load config, call this function

    enum {
      LINETYPE_CS = 0,
      LINETYPE_CSBS,
      LINETYPE_DIO,
      LINETYPE_COM,
      LINETYPE_TCP,
      LINETYPE_CNT
    };
    enum {
      PORTTYPE_IN = 0,
      PORTTYPE_OUT,
      PORTTYPE_CNT
    };
protected:
    CDevPosMgr();
    bool    isValidMachine(int l, int m);
    uint8_t getPortCount(int l, int portType);

    QString lineNames[LINE_MAX];
    uint8_t lineTypes[LINE_MAX];
    uint16_t* lineMachineCounts[LINE_MAX];
    bool      portInfos[LINE_MAX][MACHINE_MAX][PORTTYPE_CNT][PORT_MAX];
    uint16_t  lineCount;
    uint16_t  portCount[LINETYPE_CNT];
    uint16_t  lineMachineSizes[LINE_MAX];//for CSBS
    bool      inited;
};

#endif // CDEVPOSMGR_H
