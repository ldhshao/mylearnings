#ifndef CDEVPOSMGR_H
#define CDEVPOSMGR_H
#include <list>
#include <QString>
using namespace std;

#define LINE_MAX    10
#define MACHINE_MAX 16
#define PORT_MAX    12
#define LINE_BITS_COUNT    8
#define MACHINE_BITS_COUNT 8
#define PORT_BITS_COUNT    8
#define LINE_BITS_MASK     0xFF
#define MACHINE_BITS_MASK  0xFF
#define PORT_BITS_MASK     0xFF
#define LINE_BITS_SHIFT    (MACHINE_BITS_COUNT + PORT_BITS_COUNT)
#define make_dev_point(l,m,p)         ((((l)&LINE_BITS_MASK)<<LINE_BITS_SHIFT) + (((m)&MACHINE_BITS_MASK)<<PORT_BITS_COUNT) + ((p)&PORT_BITS_MASK))
#define get_line_from_dev_point(p)    (((p)>>LINE_BITS_SHIFT) & LINE_BITS_MASK)
#define get_machine_from_dev_point(p) (((p)>>PORT_BITS_COUNT) & MACHINE_BITS_MASK)
#define get_port_from_dev_point(p)    ((p) & PORT_BITS_MASK)

class CDevPosMgr
{
public:
    static CDevPosMgr* instance();

    QStringList getLineNames();
    list<list<bool>> getLinePorts(int l);
    list<bool> getMachinePorts(int l, int m);
    int        getMachineCount(int l);
    void       setPortValue(int l, int m, int port, bool use);
    void       setDevPoint(uint32_t devPt, bool use);
    QString    makeStrDevPoint(uint32_t devPoint);
    uint32_t   makeDevPoint(QString strDevPoint);
    bool       isDevPointValid(uint32_t);

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
