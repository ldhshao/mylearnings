#ifndef BINDOBJ_H
#define BINDOBJ_H

#include "ibindobj.h"
//#include "hpoint.h"
//#include "hpointoperation.h"
//#include "globalobj.h"
#include <string>
using namespace std;

// 数值类型可以改为使用模板，以后实现

/*
 * INT型数据绑定类
 */
class BindInt: public IBindObj
{
public:
    BindInt()
    {
        pVal = NULL;
        m_iminptr = NULL;
        m_imaxptr = NULL;
        negative = false;
        negativeAva = false;
        isSingle = false;
    }
    virtual int keyEventFilter(QKeyEvent *ev);
    virtual QString checkSet();
    virtual QString showSet(){
        if(negative == true && *pVal == 0)
            return QString("-");
        else
            return QString::number(*pVal,10);
    }
    virtual void clear() {}

    void SetValPtr(int * ptr){pVal = ptr; if(*pVal >=0) {negative = false;}else {negative = true;}}
    void SetVal(int val){*pVal = val;}
    void SetMinMax(int min,int max)
    {m_imin = min;m_imax = max;}
    void SetMinMaxEx(int * minptr,int * maxptr)
    {m_iminptr = minptr;m_imaxptr = maxptr;}

    void SetNegativeAva(){negativeAva = true;}
    void SetSinglenum(){isSingle = true;}

private:
    int * pVal;
    int m_imin;
    int m_imax;
    int * m_iminptr;
    int * m_imaxptr;
    bool negative; // 正负值标识,true 负数
    bool negativeAva;
    bool isSingle; // 是否为一位数，默认为false
};
//20180710合并
class BindMultInt: public IBindObj
{
public:
    BindMultInt(int *pVal, int min, int max,int mode):
        m_cur(0),
        m_max(mode)
    {
        if(BindIntList.isEmpty())
        {
            for(int i =0 ; i< mode; i++)
            {
                BindInt* bin= new BindInt;
                bin->SetValPtr(&pVal[i]);
                bin->SetMinMax(min,max);
                BindIntList.append(bin);
            }
        }
    }

    virtual int keyEventFilter(QKeyEvent *ev);
    virtual QString checkSet();
    virtual QString showSet();
    virtual void clear() {}

    int GetPointNum();
    void ResetPointNum();
private:
    void shift(int n);
    QList<BindInt*> BindIntList;
    int m_cur;
    int m_max;
};


///*
// * BYTE型数据绑定类
// */
//class  BindByte : public IBindObj
//{
//public:
//    BindByte()
//    {
//        pVal = NULL;
//        m_iminptr = NULL;
//        m_imaxptr = NULL;
//        isSingle = false;
//    }
//    virtual int keyEventFilter(QKeyEvent *ev);
//    virtual QString checkSet();
//    virtual QString showSet(){return QString::number(*pVal,10);}
//    virtual void clear() {}
//
//    void SetValPtr(byte * ptr){pVal = ptr;}
//    void SetVal(byte val){*pVal = val;}
//    void SetMinMax(byte min,byte max)
//    {
//        m_imin = min;
//        m_imax = max;
//    }
//    void SetMinMaxEx(byte * minptr,byte * maxptr)
//    {
//        m_iminptr = minptr;
//        m_imaxptr = maxptr;
//    }
//
//    void SetSinglenum(){isSingle = true;}
//
//private:
//    byte * pVal;
//    byte m_imin;
//    byte m_imax;
//    byte * m_iminptr;
//    byte * m_imaxptr;
//    bool isSingle; // 是否为一位数，默认为false
//};
//
/*
 * UINT16 型数据绑定类
 */
class  BindUint16 : public IBindObj
{
public:
    BindUint16()
    {
        pVal = nullptr;
        m_iminptr = nullptr;
        m_imaxptr = nullptr;
        isSingle = false;
    }
    virtual int keyEventFilter(QKeyEvent *ev);
    virtual QString checkSet();
    virtual QString showSet(){return QString::number(*pVal,10);}
    virtual void clear() {}
    virtual bool setValue(const QString& strVal);

    void SetValPtr(uint16_t * ptr){pVal = ptr;}
    void SetVal(uint16_t val){*pVal = val;}
    void SetMinMax(uint16_t min,uint16_t max)
    {
        m_imin = min;
        m_imax = max;
    }
    void SetMinMaxEx(uint16_t * minptr,uint16_t * maxptr)
    {
        m_iminptr = minptr;
        m_imaxptr = maxptr;
    }

    void SetSinglenum(){isSingle = true;}

protected:
    bool isValid(uint16_t val);

private:
    uint16_t * pVal;
    uint16_t m_imin;
    uint16_t m_imax;
    uint16_t * m_iminptr;
    uint16_t * m_imaxptr;
    bool isSingle; // 是否为一位数，默认为false
};

///*
// * Selection 数据绑定
// */
//class BindSelection: public IBindObj
//{
//public:
//    BindSelection(){
//        pVal = nullptr;
//        m_strList = nullptr;
//        m_strNum = 0;
//    }
//    virtual int keyEventFilter(QKeyEvent *ev);
//    virtual QString checkSet();
//    virtual QString showSet(){return checkSet();}
//    virtual void clear() {}
//
//    void SetValPtr(uint16_t * ptr) {pVal = ptr;}
//    void SetStringList(string * pstrlist,int number)
//    {
//        m_strList = pstrlist;
//        m_strNum = number;
//    }
//
//private:
//    uint16_t * pVal;
//    string * m_strList;
//    int m_strNum;
//};
////20180710合并
//class BindMultSelection: public IBindObj
//{
//public:
//    BindMultSelection(byte *pVal, string *pStr, int iCount,int mode):
//        m_cur(0),
//        m_max(mode)
//    {
//        if(BindSelectionList.isEmpty())
//        {
//            for(int i =0 ; i< mode; i++)
//            {
//                BindSelection* bin= new BindSelection;
//                bin->SetValPtr(&pVal[i]);  // byte*
//                bin->SetStringList(pStr,iCount);
//                BindSelectionList.append(bin);
//            }
//        }
//    }
//
//    virtual int keyEventFilter(QKeyEvent *ev);
//    virtual QString checkSet();
//    virtual QString showSet(){return checkSet();}
//    virtual void clear() {}
//
//    int GetPointNum();
//    void ResetPointNum();
//private:
//    void shift(int n);
//    QList<BindSelection*> BindSelectionList;
//    int m_cur;
//    int m_max;
//};
//
///*
// * 点类型数据绑定
// */
//typedef HPoint Point;
////20180710合并
//static const string SlaveName[] = {
//    "0", "CS1", "CS2", "IO1", "IO2", "IN", "COM1", "COM2", "CS1_BS", "CS2_BS","DIO"
//};
//enum {
//    Slave_Default, CS1, CS2, IO1, IO2, IN, COM1, COM2, CS1_BS, CS2_BS, DIO,Slave_Count
//};
//const int LSlave_Count = 15; // low slave's count
//const int DIOSlaveCount = 10;
//
////const int IOINPORTNUM = 12;
////const int IOOUTPORTNUM = 10;
////const int INPORTNUM = 24;
//
//
//class PointBinder : public IBindObj
//{
//public:
//    PointBinder() {
//        _ptr = NULL;      // pointer to a point list
//        memset(&_cur,0,sizeof(_cur));     // pointer to current point in list
//        single = 0;           // 1: single 0: multi
//        counter = 0;       // 0:slave 1: '.' 2: lowslave 3: '.' 4: port
//        m_index = 0;           // multi-point setting's index
//        m_max = 0;             // max num of point
//        useCom = 0;     // 0: unuse 1: use
//    }
//    ~PointBinder() {
//
//    }
//
//    virtual int keyEventFilter(QKeyEvent *ev);
//    virtual QString checkSet();
//    virtual QString showSet();
//    virtual void clear();
//
//
//    void setSinglePoint(Point *p, const char * name, int way, int type, int switchtype, int realtime);
//    void setMultiPoint(Point *p, int num, const char * name, int way, int type, int switchtype, int realtime);
//    int getPointNum()const {return m_index;}
//    int getPointTotal()const {return m_max;}
//    void setPointNum(int n) {m_index = n;}
//    void shiftPoint(int n);
//    void resetIndex(){m_index = 0; _cur = _ptr[0];}
//    /*
//    static int DelPointFromParline(Point *p)
//    {
//        if(p->enable == 0) return 0;
//        Point *ls;
//        int index;
//        index = get_point_from_parline(p, &ls);
//        ls[index].enable = 0;
//        ls[index].enable = 0;
//        ls[index].slave = 0;
//        ls[index].lowSlave = 0;
//        ls[index].port = 0;
//        ls[index].switchType = 0;
//        ls[index].type = 0;
//        ls[index].pointWay = 0;
//        strcpy(ls[index].strName, "");
//
//        return 0;
//    }*/
//
//protected:
//    virtual void keyfilterNum(int key);
//    virtual void keyfilterBackspace();
//    virtual void keyfilterDot();
//
//    virtual bool IsSetOK();
//    virtual bool IsPointComplete();
//    virtual bool store_to_parline();
//    virtual bool store_to_parameter(); // 0:success -1: fail
//
//    virtual bool set_point_to_parline(Point *pList, int index);
//    virtual bool del_point_from_parline(Point *pList, int index);
//    virtual int get_point_from_parline(Point *srcPoint, Point ** destList);
//
//    virtual inline void setslave(int n);
//    virtual inline void setlowslave(int n);
//    virtual inline void setport(int n);
//    virtual inline void move_right();
//    virtual inline void move_left();
//    virtual inline void clear_single_point(Point *p);
//
//protected:
//    friend class CBinder;
//    friend class pageSensor;
//
//        Point * _ptr;      // pointer to a point list
//        Point  _cur;     // pointer to current point in list
//        int single;           // 1: single 0: multi
//        int counter;       // 0:slave 1: '.' 2: lowslave 3: '.' 4: port
//        int m_index;           // multi-point setting's index
//        int m_max;             // max num of point
//        int useCom;     // 0: unuse 1: use
//};
//
//class BSPointBinder:public PointBinder
//{
//public:
//    BSPointBinder() {
//        _ptr = NULL;      // pointer to a point list
//        memset(&_cur,0,sizeof(_cur));     // pointer to current point in list
//        single = 0;           // 1: single 0: multi
//        counter = 0;       // 0:slave 1: '.' 2: lowslave 3: '.' 4: port
//        m_index = 0;           // multi-point setting's index
//        m_max = 0;             // max num of point
//    }
//    ~BSPointBinder() {
//    }
//    virtual int keyEventFilter(QKeyEvent *ev);
//    virtual QString checkSet();
//    virtual QString showSet();
//    virtual void clear();
//protected:
//    void keyfilterNum(int key);
//    void keyfilterBackspace();
//    void keyfilterDot();
//
//    bool IsSetOK();
//    bool IsPointComplete();
//    bool store_to_parline();
//    bool store_to_parameter(); // 0:success -1: fail
//
//    bool set_point_to_parline(Point *pList, int index);
//    bool del_point_from_parline(Point *pList, int index);
//    int get_point_from_parline(Point *srcPoint, Point ** destList);
//
//    inline void setslave(int n);
//    inline void setport(int n);
//    inline void move_right();
//    inline void move_left();
//    inline void clear_single_point(Point *p);
//
//
//};
//
//
////20180710合并
//class SuperPointBinder_2:public PointBinder
//{
//public:
//    SuperPointBinder_2() {
//    }
//    ~SuperPointBinder_2() {
//    }
//    virtual void setslave(int n);
//    //    virtual inline void setslave(int n);
//};

#endif // BINDOBJ_H
