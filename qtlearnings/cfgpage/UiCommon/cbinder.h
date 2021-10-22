#ifndef CBINDER_H
#define CBINDER_H
#include<cstdint>
#include <QStringList>
class CKeyDnEdit;
class CKeyDnComboBox;

enum pointmode{
    defaultmode               = 0L,
    point_quantity            =  1L << 0,
    point_way                   =  1L << 1,
    point_type                  =  1L << 2,
    point_switchtype       =  1L << 3,
    point_com                  =  1L << 4,
    point_bs                      = 1L << 5,
    point_realtime                      = 1L << 6,
    point_super                = 1L << 7,
};

static const pointmode Single = defaultmode;
static const pointmode Multi = point_quantity;
static const pointmode Input = defaultmode;
static const pointmode Output = point_way;
static const pointmode Switch = defaultmode;
static const pointmode Analogy = point_type;
static const pointmode S_Open = defaultmode;
static const pointmode S_Close = point_switchtype;
static const pointmode Com = point_com;
static const pointmode BS = point_bs;
static const pointmode REALTIME = point_realtime;
static const pointmode SuperPoint = point_super;

class CBinder
{
public:
    CBinder();

    /*
     * Value type
     */
//    static void
//    BindEdit(CKeyDnEdit *pObj, int *pVal,int min, int max,int mode =0);
//
//    static void
//    BindEdit(CKeyDnEdit *pObj, int *pVal, int *min, int *max);
//
//    static void
//    BindEdit(CKeyDnEdit *pObj, byte *pVal, int min, int max);
//
//    static void
//    BindEdit(CKeyDnEdit *pObj, byte *pVal, byte *min, byte *max);
//
    static void
    BindEdit(CKeyDnEdit *pObj, uint16_t *pVal, uint16_t min, uint16_t max);
//
//    static void
//    BindEdit(CKeyDnEdit *pObj, quint16 *pVal, quint16 *min, quint16 *max);
//
//
    /*
     * Selection type
     */
    static void
    BindComboBox(CKeyDnComboBox *pObj, uint16_t *pVal, const QStringList &params, const QString& strDefault);

//    static void
//    BindEdit(CKeyDnEdit *pObj, byte*pVal, string *pStr, int iCount,int mode = 0);


//    /*
//     * Point type
//     */
//    static void
//    BindEdit(CKeyDnEdit *pObj, Point *point, const char *name, int mode,...); // 多点最后一个参数添加个数

    //static void
    //BindEdit(CKeyDnEdit *pObj, Point *point,  bool bs, const char *name, int mode,...);

    /*
     * Clear Settings
     */
    static void
    Clear(CKeyDnEdit *pObj);

    static void
    ClearPObj(CKeyDnEdit *pObj);
};

#endif // CBINDER_H
