#include "cbinder.h"
#include "ckeydnedit.h"
#include "../cdevpointedit.h"
#include "ckeydncombobox.h"
#include "bindobj.h"

CBinder::CBinder()
{
}

///*
// * Value type
// */
////20180711合并
//void
//CBinder::BindEdit(CKeyDnEdit *pObj, int *pVal, int min, int max,int mode)
//{
//    if(mode == 0)
//    {
//        if(pObj->pBind == NULL)
//        {
//            BindInt *pbind = new BindInt();
//            pObj->pBind = pbind;
//        }
//        pObj->setAccessibleName(QString::fromUtf8("请用数字键设置参数"));
//
//        BindInt *pTmpBind = dynamic_cast<BindInt *> (pObj->pBind);
//        pTmpBind->SetValPtr(pVal);
//        pTmpBind->SetMinMax(min,max);
//        pObj->showText();
//    }
//    else
//    {
//        if(pObj->pBind == NULL)
//        {
//            BindMultInt *pbind = new BindMultInt(pVal, min, max,mode);
//            pObj->pBind = pbind;
//        }
//        BindMultInt *pTmpBind = dynamic_cast<BindMultInt *> (pObj->pBind);
//        pTmpBind->ResetPointNum();
//        pObj->setAccessibleName(QString::fromUtf8("请用数字键设置参数"));
//        pObj->showText();
//    }
//}
//
//void
//CBinder::BindEdit(CKeyDnEdit *pObj, int *pVal, int *min, int *max)
//{
//    if(pObj->pBind == NULL)
//    {
//        BindInt *pbind = new BindInt();
//        pObj->pBind = pbind;
//    }
//    pObj->setAccessibleName(QString::fromUtf8("请用数字键设置参数"));
//
//    BindInt *pTmpBind = dynamic_cast<BindInt *> (pObj->pBind);
//    pTmpBind->SetValPtr(pVal);
//    pTmpBind->SetMinMaxEx(min,max);
//    pObj->showText();
//}
//
//void
//CBinder::BindEdit(CKeyDnEdit *pObj, byte *pVal, int min, int max)
//{
//    if(pObj->pBind == NULL)
//    {
//        BindByte *pbind = new BindByte();
//        pObj->pBind = pbind;
//    }
//    pObj->setAccessibleName(QString::fromUtf8("请用数字键设置参数"));
//
//    BindByte *pTmpBind = dynamic_cast<BindByte *> (pObj->pBind);
//    pTmpBind->SetValPtr(pVal);
//    pTmpBind->SetMinMax(min,max);
//    pObj->showText();
//}
//
//void
//CBinder::BindEdit(CKeyDnEdit *pObj, byte *pVal, byte *min, byte *max)
//{
//    if(pObj->pBind == NULL)
//    {
//        BindByte *pbind = new BindByte();
//        pObj->pBind = pbind;
//    }
//    pObj->setAccessibleName(QString::fromUtf8("请用数字键设置参数"));
//
//    BindByte *pTmpBind = dynamic_cast<BindByte *> (pObj->pBind);
//    pTmpBind->SetValPtr(pVal);
//    pTmpBind->SetMinMaxEx(min,max);
//    pObj->showText();
//}
//
// quint16
void
CBinder::BindEdit(CKeyDnEdit *pObj, uint16_t *pVal, uint16_t min, uint16_t max, uint16_t defaultVal)
{
    if(pObj->pBind == NULL)
    {
        BindUint16 *pbind = new BindUint16();
        pObj->pBind = pbind;
    }
    pObj->setAccessibleName(QString::fromUtf8("请用数字键设置参数"));

    BindUint16 *pTmpBind = dynamic_cast<BindUint16 *> (pObj->pBind);
    pTmpBind->SetValPtr(pVal);
    pTmpBind->SetMinMax(min,max);
    if (!pTmpBind->isValid(*pVal)){
        if(pTmpBind->isValid(defaultVal)){
            pTmpBind->SetVal(defaultVal);
        }else{
            pTmpBind->SetVal(min);
        }
    }
    pObj->showText();
}

void
CBinder::BindEdit(CKeyDnEdit *pObj, uint16_t *pVal, uint16_t *min, uint16_t *max, uint16_t defaultVal)
{
    if(pObj->pBind == NULL)
    {
        BindUint16Ptr *pbind = new BindUint16Ptr();
        pObj->pBind = pbind;
    }
    pObj->setAccessibleName(QString::fromUtf8("请用数字键设置参数"));

    BindUint16Ptr *pTmpBind = dynamic_cast<BindUint16Ptr *> (pObj->pBind);
    pTmpBind->SetValPtr(pVal);
    pTmpBind->SetMinMax(min,max);
    if (!pTmpBind->isValid(*pVal)){
        if(pTmpBind->isValid(defaultVal)){
            pTmpBind->SetVal(defaultVal);
        }else{
            pTmpBind->SetVal(*min);
        }
    }
    pObj->showText();
}
void
CBinder::BindEdit(CKeyDnEdit *pObj, uint16_t *pVal, uint16_t min, uint16_t *max, uint16_t defaultVal)
{
    uint16_t *pMin = Uint16PtrMngr::instance()->registerValue(min);
    BindEdit(pObj,pVal,pMin,max, defaultVal);
}
void
CBinder::BindEdit(CKeyDnEdit *pObj, uint16_t *pVal, uint16_t *min, uint16_t max, uint16_t defaultVal)
{
    uint16_t *pMax = Uint16PtrMngr::instance()->registerValue(max);
    BindEdit(pObj,pVal,min,pMax, defaultVal);
}

void CBinder::BindEdit(CDevPointEdit *pEdit, uint16_t *pVal, uint32_t defaultVal)
{
    pEdit->setValuePtr(pVal);
}
/*
 * Selection type
 */
void CBinder::BindComboBox(CKeyDnComboBox *pObj, uint16_t *pVal, const QStringList &params, const QString &strDefault)
{
    pObj->insertItems(0, params);
    pObj->setValuePtr(pVal);
    if (0 <= *pVal && *pVal < pObj->count()){
        pObj->setCurrentIndex(*pVal);
    }else{
        int iDef = pObj->findText(strDefault);
        if (iDef < 0)
            iDef = 0;
        pObj->setCurrentIndex(iDef);
    }
}
////20180710合并
//void
//CBinder::BindEdit(CKeyDnEdit *pObj, byte *pVal, string *pStr, int iCount,int mode)
//{
//    //     qDebug()<<pVal<<&pVal[0]<<&pVal[1]<<&pVal[2];
//    if(mode == 0)
//    {
//        if(pObj->pBind == NULL)
//        {
//            BindSelection *pbind = new BindSelection();
//            pObj->pBind = pbind;
//        }
//        BindSelection * pTmpBind = dynamic_cast<BindSelection *>(pObj->pBind);
//        pObj->setAccessibleName(QString::fromUtf8("请用左右箭头选择"));
//        pTmpBind->SetValPtr(pVal);  // byte*
//        pTmpBind->SetStringList(pStr,iCount);
//        pObj->showText();
//    }
//    else
//    {
//        if(pObj->pBind != NULL)
//        {
//            delete pObj->pBind;
//        }
//        BindMultSelection* pbind = new BindMultSelection(pVal, pStr, iCount,mode);
//        pObj->pBind = pbind;
//
//        BindMultSelection * pTmpBind = dynamic_cast<BindMultSelection *>(pObj->pBind);
//        pTmpBind->ResetPointNum();
//        pObj->setAccessibleName(QString::fromUtf8("请用左右箭头选择,\n用T键选择上一个输出点\n用K键选择下一个输出点"));
//        pObj->showText();
//    }
//}
//
//
///*
// * Point type
// */
//void
//CBinder::BindEdit(CKeyDnEdit *pObj, Point *point, const char *name, int mode,...)
//{
//    int way,type,switchtype, realtime;
//    if(pObj->pBind == NULL)
//    {
//        if(mode & point_bs){
//            BSPointBinder *pbind = new BSPointBinder();
//            pObj->pBind = pbind;
//        }
//        else if(mode & point_super){
//            SuperPointBinder_2 *pbind = new SuperPointBinder_2();
//            pObj->pBind = pbind;
//        }
//        else{
//            PointBinder *pbind = new PointBinder();
//            pObj->pBind = pbind;
//        }
//    }
//    PointBinder * pTmpBind = dynamic_cast<PointBinder *>(pObj->pBind);
//
//    mode & point_way ? way = 1: way = 0;
//    mode & point_type ? type = 1: type = 0;
//    mode & point_switchtype ? switchtype = 1: switchtype = 0;
//    mode & point_realtime ? realtime = 1: realtime = 0;
//    if(mode & Multi)
//    {
//        va_list argptr;
//        va_start(argptr, mode); //初始化变元指针
//        int pointnum = va_arg(argptr, int);
//        va_end(argptr);
//        pTmpBind->setMultiPoint(point,pointnum,name,way,type,switchtype, realtime);
//    }
//    else
//    {
//        pTmpBind->setSinglePoint(point,name,way,type,switchtype, realtime);
//    }
//    pTmpBind->resetIndex();
//    QString strTip;
//    if(way == INPUTPOINT)
//    {
//        {
//            //2016-07-19 添加
//            if(mode & Multi){
//                if(mode & point_bs)
//                    strTip = QString::fromUtf8("键1=CS1，2=CS2\n用*设置点, 数字键设置闭锁板号(例如CS1.1)\n用T键选择上一个传感器输入点\n用K键选择下一个传感器输入点");
////20180710合并
//if(mode & point_super)
//                    strTip = QString::fromUtf8("键1=CS1，2=CS2，3=IO1，4=IO2，5=IN，6=COM1，\n7=COM2，8=CS1_BS，9=CS2_BS , A= DIO\n用*设置中位机、下位机、口号\n用T键选择上一个传感器输入点\n用K键选择下一个传感器输入点");
//                else
//                    strTip = QString::fromUtf8("键1=CS1，2=CS2，3=IO1，4=IO2，\n5=IN，6=COM1，7=COM2 , A=DIO \n用*设置中位机、下位机、口号\n用T键选择上一个输入点\n用K键选择下一个输入点");
//            }
//            else{
//                if(mode & point_bs)
//                    strTip = QString::fromUtf8("键1=CS1，2=CS2\n用*设置点, 数字键设置闭锁板号(例如CS1.1)");
//                else
//                    strTip = QString::fromUtf8("键1=CS1，2=CS2，3=IO1，4=IO2，\n5=IN，6=COM1，7=COM2 , A=DIO\n用*设置中位机、下位机、口号");
//            }
//        }
//    }
//    else if(way == OUTPUTPOINT)
//    {
//
//        //2016-07-22 添加
////20180710合并
//        if(mode & Multi)
//            strTip = QString::fromUtf8("键1=CS1，2=CS2，3=IO1，\n4=IO2 , A=DIO\n用*设置中位机、下位机、口号\n用T键选择上一个输出点\n用K键选择下一个输出点");
//        else
//            strTip = QString::fromUtf8("键1=CS1，2=CS2，3=IO1，\n4=IO2 , A=DIO\n用*设置中位机、下位机、口号");
//    }
//    pObj->setAccessibleName(strTip);
//
//    pObj->showText();
//}
/*
 * Point type
 */
/*
void
CBinder::BindEdit(CKeyDnEdit *pObj, Point *point, bool bs, const char *name, int mode,...)
{
    int way,type,switchtype;
    if(pObj->pBind == NULL)
    {
        BSPointBinder *pbind = new BSPointBinder();
        pObj->pBind = pbind;
    }
    BSPointBinder * pTmpBind = dynamic_cast<BSPointBinder *>(pObj->pBind);
    mode & point_way ? way = 1: way = 0;
    mode & point_type ? type = 1: type = 0;
    mode & point_switchtype ? switchtype = 1: switchtype = 0;

    if(mode & Multi)
    {
        va_list argptr;
        va_start(argptr, mode); //初始化变元指针
        int pointnum = va_arg(argptr, int);
        va_end(argptr);
        pTmpBind->setMultiPoint(point,pointnum,name,way,type,switchtype);
    }
    else
    {
        pTmpBind->setSinglePoint(point,name,way,type,switchtype);
    }

    QString strTip;
    if(way == INPUTPOINT)
    {
        {
            if(mode & Multi)
                strTip = QString::fromUtf8("键1=CS1，2=CS2\n用*设置下位机口号(例如CS1.1)\n用T键选择上一个输入点\n用K键选择下一个输入点");
            else
                strTip = QString::fromUtf8("键1=CS1，2=CS2\n用*设置下位机口号(例如CS1.1)");
        }
    }
    else if(way == OUTPUTPOINT)
    {
        if(mode & Multi)
            strTip = QString::fromUtf8("键1=CS1，2=CS2\n用*设置下位机口号(例如CS1.1)\n用T键选择上一个输出点\n用K键选择下一个输出点");
        else
            strTip = QString::fromUtf8("键1=CS1，2=CS2\n用*设置下位机口号(例如CS1.1)");
    }
    pObj->setAccessibleName(strTip);

    pObj->showText();
}
*/
void
CBinder::Clear(CKeyDnEdit *pObj)
{
    if(pObj->pBind != NULL)
        pObj->pBind->clear();
}

void
CBinder::ClearPObj(CKeyDnEdit *pObj)
{
    if(pObj->pBind != NULL)
        pObj->pBind = NULL;
}

Uint16PtrMngr* Uint16PtrMngr::instance()
{
    static Uint16PtrMngr _ins;
    return &_ins;
}
uint16_t *Uint16PtrMngr::registerValue(uint16_t val)
{
    uint16_t *pFind = nullptr;
    for(auto it = ptrList.begin(); it != ptrList.end(); it++){
        if (*(*it) == val){
            pFind = *it;
            break;
        }
    }

    if (nullptr == pFind){
        pFind = new uint16_t(val);
        ptrList.push_back(pFind);
    }

    return pFind;
}
Uint16PtrMngr::~Uint16PtrMngr()
{
    for(auto it = ptrList.begin(); it != ptrList.end(); it++){
        delete (*it);
    }
    ptrList.clear();
}
