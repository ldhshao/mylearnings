#include "bindobj.h"
//#include <QMessageBox>

// int,short,byte can use this；longer than int should use a new one.
#define valueFilter(type, ev)\
    int keyVal = ev->key();\
    type tmp = *pVal;\
    switch(keyVal)\
    {\
    case Qt::Key_Left:\
        if (tmp > m_imin){\
            tmp = tmp -1;\
            if (tmp >= m_imin){*pVal = tmp; setState(BS_MODIFIED); }\
        }\
        break;\
    case Qt::Key_Right:\
        if (tmp < m_imax){\
            tmp = tmp + 1;\
            if (tmp <= m_imax){*pVal = tmp; setState(BS_MODIFIED); }\
        }\
        break;\
    case Qt::Key_Up:\
    case Qt::Key_Down:\
    case Qt::Key_Return:\
        break;\
    case Qt::Key_Backspace:\
        if(!isSingle){\
            type n = tmp /10;\
            if (n >= m_imin){*pVal = n; setState(BS_MODIFIED);}\
            else {*pVal = m_imin; setState(BS_MODIFIED);}\
        }\
        break;\
    case Qt::Key_Escape:\
        break;\
    case Qt::Key_0:case Qt::Key_1:case Qt::Key_2:\
    case Qt::Key_3:case Qt::Key_4:case Qt::Key_5:\
    case Qt::Key_6:case Qt::Key_7:case Qt::Key_8:\
    case Qt::Key_9:\
        if(isSingle)\
        {\
            if(((keyVal - 0x30) >= m_imin)&&((keyVal - 0x30) <= m_imax))    {*pVal = keyVal - 0x30;setState(BS_MODIFIED);}\
        }\
        else{\
            tmp = tmp*10 + keyVal - 0x30;\
            {\
                if(tmp > m_imax) *pVal = m_imax;\
                else *pVal = tmp;\
                setState(BS_MODIFIED);\
            }\
        }\
    break;\
    default:\
        break;\
    }
#define valueFilterWithPtr(type, ev)\
    int keyVal = ev->key();\
    type tmp = *pVal;\
    switch(keyVal)\
    {\
    case Qt::Key_Left:\
        if (tmp > *m_iminptr){\
            tmp = tmp -1;\
            if(tmp >= *m_iminptr){*pVal = tmp; setState(BS_MODIFIED); }\
        }\
        break;\
    case Qt::Key_Right:\
        if (tmp < *m_imaxptr){\
            tmp = tmp + 1;\
            if(tmp <= *m_imaxptr){*pVal = tmp; setState(BS_MODIFIED); }\
        }\
        break;\
    case Qt::Key_Up:\
    case Qt::Key_Down:\
    case Qt::Key_Return:\
        break;\
    case Qt::Key_Backspace:\
        if(!isSingle){\
            type n = tmp /10;\
            if(n >= *m_iminptr){ *pVal = n; setState(BS_MODIFIED);}\
            else {*pVal = *m_iminptr; setState(BS_MODIFIED);}\
        }\
        break;\
    case Qt::Key_Escape:\
        break;\
    case Qt::Key_0:case Qt::Key_1:case Qt::Key_2:\
    case Qt::Key_3:case Qt::Key_4:case Qt::Key_5:\
    case Qt::Key_6:case Qt::Key_7:case Qt::Key_8:\
    case Qt::Key_9:\
        if(isSingle)\
        {\
            if(((keyVal - 0x30) >= *m_iminptr)&&((keyVal - 0x30) <= *m_imaxptr))    {*pVal = keyVal - 0x30;setState(BS_MODIFIED);}\
        }\
        else{\
            tmp = tmp*10 + keyVal - 0x30;\
            if(tmp > *m_imaxptr) *pVal = *m_imaxptr;\
            else *pVal = tmp;\
            setState(BS_MODIFIED);\
        }\
    break;\
    default:\
        break;\
    }
#if 0
#define valueFilter(type, ev, ret)\
    int keyVal = ev->key();\
    int tmp = *pVal;\
    switch(keyVal)\
    {\
    case Qt::Key_Left:\
        if(tmp != 0)\
            *pVal = tmp-1;\
        break;\
    case Qt::Key_Right:\
        *pVal = tmp + 1;\
        break;\
    case Qt::Key_Up:\
    case Qt::Key_Down:\
    case Qt::Key_Return:\
        ret = kd_check_emit;\
        break;\
    case Qt::Key_Backspace:\
        if(!isSingle) *pVal = *pVal/10;\
        ret = kd_show;\
        break;\
    case Qt::Key_Escape:\
        ret = kd_emit;\
        break;\
    case Qt::Key_0:case Qt::Key_1:case Qt::Key_2:\
    case Qt::Key_3:case Qt::Key_4:case Qt::Key_5:\
    case Qt::Key_6:case Qt::Key_7:case Qt::Key_8:\
    case Qt::Key_9:\
        if(isSingle)\
        {\
            if(m_iminptr != NULL &&(m_imaxptr!=NULL)){\
                if(((keyVal - 0x30) >= *m_iminptr)&&((keyVal - 0x30) <= *m_imaxptr))    {*pVal = keyVal - 0x30;}\
            }\
            else {\
                if(((keyVal - 0x30) >= m_imin)&&((keyVal - 0x30) <= m_imax))    {*pVal = keyVal - 0x30;}\
            }\
        }\
        else{\
            tmp = tmp*10 + keyVal - 0x30;\
            *pVal = (type)tmp;\
            if(m_iminptr != NULL &&(m_imaxptr!=NULL))\
            {\
                if(tmp > *m_imaxptr)\
                *pVal = *m_imaxptr;\
            }\
            else\
            {\
                if(tmp > m_imax)\
                *pVal = m_imax;\
            }\
        }\
        ret = kd_show;\
    break;\
    default:\
        ret = kd_emit;\
        break;\
    }
#endif

#define valueCheck()\
    if(m_iminptr != NULL &&(m_imaxptr!=NULL))\
    {\
    if(*pVal > *m_imaxptr)\
    *pVal = *m_imaxptr;\
    else if(*pVal < *m_iminptr)\
    *pVal = *m_iminptr;\
    }\
    else\
    {\
    if(*pVal > m_imax)\
    *pVal = m_imax;\
    else if(*pVal < m_imin)\
    *pVal = m_imin;\
    }\
    return QString::number(*pVal,10);\

#define onlyvalueCheck()\
    if(m_iminptr != NULL &&(m_imaxptr!=NULL))\
    {\
    if(*pVal > *m_imaxptr)\
    *pVal = *m_imaxptr;\
    else if(*pVal < *m_iminptr)\
    *pVal = *m_iminptr;\
    }\
    else\
    {\
    if(*pVal > m_imax)\
    *pVal = m_imax;\
    else if(*pVal < m_imin)\
    *pVal = m_imin;\
    }
#define fullvalueFilter(type, ev, ret)\
    int keyVal = ev->key();\
    type tmp = *pVal;\
    switch(keyVal)\
    {\
    case Qt::Key_Left:\
            *pVal = tmp-1;\
            onlyvalueCheck()\
        break;\
    case Qt::Key_Right:\
        *pVal = tmp + 1;\
       onlyvalueCheck()\
        break;\
    case Qt::Key_Up:\
    case Qt::Key_Down:\
    case Qt::Key_Return:\
        ret = kd_check_emit;\
        break;\
    case Qt::Key_Backspace:\
        if(!isSingle) *pVal = *pVal/10;\
        ret = kd_show;\
        break;\
    case Qt::Key_Escape:\
        ret = kd_emit;\
        break;\
    case Qt::Key_0:case Qt::Key_1:case Qt::Key_2:\
    case Qt::Key_3:case Qt::Key_4:case Qt::Key_5:\
    case Qt::Key_6:case Qt::Key_7:case Qt::Key_8:\
    case Qt::Key_9:\
        if(isSingle)\
        {\
            if(m_iminptr != NULL &&(m_imaxptr!=NULL)){\
                if(((keyVal - 0x30) >= *m_iminptr)&&((keyVal - 0x30) <= *m_imaxptr))    {*pVal = keyVal - 0x30;}\
            }\
            else {\
                if(((keyVal - 0x30) >= m_imin)&&((keyVal - 0x30) <= m_imax))    {*pVal = keyVal - 0x30;}\
            }\
        }\
        else{\
            *pVal = tmp*10 + keyVal - 0x30;\
            if(m_iminptr != NULL &&(m_imaxptr!=NULL))\
            {\
                if(*pVal > *m_imaxptr)\
                *pVal = *m_imaxptr;\
            }\
            else\
            {\
                if(*pVal > m_imax)\
                *pVal = m_imax;\
            }\
        }\
        ret = kd_show;\
    break;\
    default:\
        ret = kd_emit;\
        break;\
    }


/*
 * BIND INT
 */
int BindInt::keyEventFilter(QKeyEvent *ev)
{
    int ret = kd_check;
    if(Qt::Key_A == ev->key())
    {
        if(negativeAva == false)
            return kd_null;

        if(*pVal == 0)
        {
            if(m_iminptr != NULL && *m_iminptr < 0)
            {
                negative = true;
            }
            else if(m_imin < 0)
            {
                negative = true;
            }
        }
        return kd_show;
    }

    if(negative == true && ev->key() != Qt::Key_A) // 对可以为负的做单独处理
    {
        int tmp = *pVal;
        int keyVal = ev->key();
        switch(keyVal)
        {
        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Return:
            if(*pVal > 0)
                negative = false;
            ret = kd_check_emit;
            break;
        case Qt::Key_Backspace:
            *pVal = *pVal/10;
            if(*pVal == 0)
                negative = false;
            ret = kd_show;
            break;
        case Qt::Key_Escape:
            ret = kd_emit;
            break;

        case Qt::Key_Left:
            *pVal = tmp-1;
            onlyvalueCheck()
              ret = kd_check;
            break;
        case Qt::Key_Right:
            *pVal = tmp+1;
             onlyvalueCheck()
            ret = kd_check;
            break;

        case Qt::Key_0:case Qt::Key_1:case Qt::Key_2:
        case Qt::Key_3:case Qt::Key_4:case Qt::Key_5:
        case Qt::Key_6:case Qt::Key_7:case Qt::Key_8:
        case Qt::Key_9:
            *pVal = tmp*10 - (keyVal - 0x30);
            // 对上限做限制
            if(m_iminptr != NULL &&(m_imaxptr!=NULL))
            {
                if(*pVal > *m_imaxptr)
                    *pVal = *m_imaxptr;
            }
            else
            {
                if(*pVal > m_imax)
                    *pVal = m_imax;
                else if(*pVal < m_imin)
                    *pVal = m_imin;
            }

            ret = kd_show;
            break;
        default:
            ret = kd_emit;
            break;
        }
    }
    else{
        fullvalueFilter(int, ev, ret)
    }
    return ret;
}

QString BindInt::checkSet()
{
    valueCheck()
}
///*
// * BIND BYTE
// */
//int BindByte::keyEventFilter(QKeyEvent *ev)
//{
//    int ret = 0;
//    valueFilter(byte, ev, ret)
//    return ret;
//}
//
//QString BindByte::checkSet()
//{
//    valueCheck()
//}

/*
 * BIND UINT16
 */
int BindUint16::keyEventFilter(QKeyEvent *ev)
{
    int ret = 0;
    valueFilter(uint16_t, ev);
    return ret;
}

bool BindUint16::setValue(const QString &strVal)
{
    bool ret = true;
    if (strVal != showSet()){
        uint16_t newVal = strVal.toUShort();
        if (newVal != *pVal){
            if (isValid(newVal)){
                *pVal = newVal;
                setState(BS_MODIFIED);
            }else {
                ret = false;
            }
        }
    }

    return ret;
}
bool BindUint16::isValid(uint16_t val)
{
    if (m_imin <= val && val <= m_imax){
        return true;
    }

    return false;
}

/*
 * BIND UINT16Ptr
 */
int BindUint16Ptr::keyEventFilter(QKeyEvent *ev)
{
    int ret = 0;
    valueFilterWithPtr(uint16_t, ev);
    return ret;
}

bool BindUint16Ptr::setValue(const QString &strVal)
{
    bool ret = true;
    if (strVal != showSet()){
        uint16_t newVal = strVal.toUShort();
        if (newVal != *pVal){
            if (isValid(newVal)){
                *pVal = newVal;
                setState(BS_MODIFIED);
            }else {
                ret = false;
            }
        }
    }

    return ret;
}
bool BindUint16Ptr::isValid(uint16_t val)
{
    if(nullptr != m_iminptr && nullptr != m_imaxptr){
        if (*m_iminptr <= val && val <= *m_imaxptr){
            return true;
        }
    }

    return false;
}
bool BindUint16Ptr::onRangeChanged()
{
    if (!isValid(*pVal)){
        if (*m_iminptr > *pVal){
            *pVal = *m_iminptr;
            setState(BS_MODIFIED);
        }else if (*m_imaxptr < *pVal){
            *pVal = *m_imaxptr;
            setState(BS_MODIFIED);
        }
        return isModified();
    }
    return false;
}

///*
// * Selection
// */
//int BindSelection::keyEventFilter(QKeyEvent *ev)
//{
//    int ret = kd_check;
//    int keyVal = ev->key();
//    uint16_t tmp;
//
//    if(pVal != NULL) {tmp = *pVal;}
//
//    switch (keyVal) {
//    case Qt::Key_Left:
//        if(tmp > 0) tmp --;
//        else if(tmp == 0) tmp = m_strNum - 1;
//        //else LOG_ERROR(Global::log, "select left error!")
//
//        ret = kd_check_emit;
//        break;
//    case Qt::Key_Right:
//        if(tmp < m_strNum-1) tmp ++;
//        else if(tmp == m_strNum - 1) tmp = 0;
//        //else LOG_ERROR(Global::log, "select right error!")
//
//        ret = kd_check_emit;
//        break;
//    case Qt::Key_Up:
//    case Qt::Key_Down:
//    case Qt::Key_Return:
//        ret = kd_check_emit;
//        break;
//    case Qt::Key_Escape:
//        ret = kd_emit;
//        break;
//    default:
//        break;
//    }
//
//    if(pVal != nullptr) {*pVal = tmp;}
//
//    return ret;
//}
//QString BindSelection::checkSet()
//{
//    int n = 0;
//    if(pVal != nullptr) n = *pVal;
//
//    return QString::fromUtf8(this->m_strList[n].c_str());
//}


///*
// * POINT
// */
//void PointBinder::setSinglePoint(Point *p, const char *name, int way, int type, int switchtype, int realtime)
//{
//    single = 1;
//    _ptr = p;
//    strcpy(_ptr->strName, name);
//    _ptr->pointWay = way;
//    _ptr->type = type;
//    _ptr->switchType = switchtype;
//    _ptr->realtime = realtime;
//    _cur = _ptr[0];
//}
//
//void PointBinder::setMultiPoint(Point *p, int num, const char *name, int way, int type, int switchtype, int realtime)
//{
//    single = 0;
//    _ptr = p;
//    m_max = num;
//    for(int i = 0; i < num; i++)
//    {
//        sprintf(_ptr[i].strName, "%s%d", name, i+1);
//        _ptr[i].pointWay = way;
//        _ptr[i].type = type;
//        _ptr[i].switchType = switchtype;
//        _ptr[i].realtime = realtime;
//    }
//    _cur = _ptr[0];
//}
//
//int PointBinder::keyEventFilter(QKeyEvent *ev)
//{
//    int ret = kd_check;
//    int keyVal = ev->key();
//
//    switch (keyVal) {
//    /// Number
////20180710合并
//    case Qt::Key_A:
//        keyVal = 0x30+10;
//        if(counter == 0)//qh 只有第按键才可以
//        {
//            keyfilterNum(keyVal);
//        }
//        break;
//    case Qt::Key_0:case Qt::Key_1:case Qt::Key_2:
//    case Qt::Key_3:case Qt::Key_4:case Qt::Key_5:
//    case Qt::Key_6:case Qt::Key_7:case Qt::Key_8:
//    case Qt::Key_9:
////20180710合并
//        if(counter%2==1)//按完点之后再按数字,自动counter++
//        {
//            move_right();
//        }
//        keyfilterNum(keyVal);
//        break;
//        /// BACKSPACE
//    case Qt::Key_Backspace:
//        keyfilterBackspace();
//        break;
//        /// DOT
//    case Qt::Key_Period:
//        keyfilterDot();
//        break;
//    case Qt::Key_T:
//        if(single) return kd_null;
////20180710合并
//        if(IsSetOK())
//        {
//            shiftPoint(0);
//            ret = kd_show_emit; //qh
//        }
//        else
//            return kd_null;
//        break;
//    case Qt::Key_K:
//        if(single) return kd_null;
////20180710合并
//        if(IsSetOK())
//        {
//            shiftPoint(1);
//            ret = kd_show_emit; //qh
//        }
//        else
//            return kd_null;
//        break;
//    case Qt::Key_Up:
//    case Qt::Key_Down:
//        if(IsSetOK())
//        {
//            //m_index = 0; _cur = _ptr[0];
//            /*changed by wujinping: 修正切换控件显示时点显示不完整的问题 */
////20180710合并
//            ret = kd_emit;  //qh
//        }
//        else ret = kd_null;
//        break;
//    case Qt::Key_Return:
//        if(IsSetOK())
//        {
//            ret = kd_check_emit;
////            m_index = 0;//2018-04-03 注释
////            _cur = _ptr[0];//2018-04-03 注释
//        }
//        else ret = kd_null;
//        break;
//    case Qt::Key_Escape:
//        ret = kd_emit; m_index = 0;
//        break;
//    default:
//        break;
//    }
//
//    return ret;
//}
//
//QString PointBinder::checkSet()
//{
//    QString output;
//    Point *point = &_cur;
//
//    if(point->slave == 0)
//        counter = 0;
//
//    switch (counter) {
//    case 0:
//        output = QString(SlaveName[point->slave].c_str());
//        break;
//    case 1:
//        output = QString(SlaveName[point->slave].c_str()) +
//                QChar('.');
//        break;
//    case 2:
//        output = QString(SlaveName[point->slave].c_str()) +
//                QChar('.') + QString::number(point->lowSlave);
//        break;
//    case 3:
//        output = QString(SlaveName[point->slave].c_str()) +
//                QChar('.') + QString::number(point->lowSlave) +
//                QChar('.');
//        break;
//    case 4:
////20180710合并
//        if(point->slave == CS1 || point->slave == CS2 || point->slave == COM1 || point->slave == COM2 \
//||point->slave ==DIO\
//                || point->slave == CS1_BS || point->slave == CS2_BS)//qh
//        {
//            output = QString(SlaveName[point->slave].c_str()) +
//                    QChar('.') + QString::number(point->lowSlave) +
//                    QChar('.') + QString::number(point->port);
//        }
//        else
//        {
//            output = QString(SlaveName[point->slave].c_str()) +
//                    QChar('.') + QString::number(point->port);
//        }
//        break;
//    default:
//        break;
//    }
//
//    return output;
//}
//
//QString PointBinder::showSet()
//{
//    QString output;
//    Point *point = & _cur;
//
//    if(point->slave == 0) {
//        output = "0";
//        counter = 0;
//    } else {
////20180710合并
//        if(point->slave == CS1 || point->slave == CS2 || point->slave == COM1 || point->slave == COM2 \
//||point->slave ==DIO \
//                || point->slave == CS1_BS || point->slave == CS2_BS)
//        {
//        output = QString(SlaveName[point->slave].c_str()) + QChar('.') +
//                QString::number(point->lowSlave) + QChar('.') +
//                QString::number(point->port);
//        }
//        else
//        {
//            output = QString(SlaveName[point->slave].c_str()) + QChar('.') +
//                    QString::number(point->port);
//        }
//        counter = 4;
//    }
//    return output;
//}
//
//void PointBinder::keyfilterNum(int key)
//{
//    int _key = key - 0x30;
//    Point *point = &_cur;
//    if(counter == 0){
//        setslave(_key);
//    }
////20180710合并
//    //    else if(counter%2 == 1){  //在點的位置,如果按數字,那麼count++
//    //        move_right();
//    //        keyfilterNum(key);
//    //    }
//    else if(counter == 2){
//        int tmp = point->lowSlave * 10 + _key;
//        setlowslave(tmp);
//    }
//    else if(counter == 4){
//        // 设置好的点被修改时删掉在临时parline的信息
//        // 这个时候删除可以保证在页面未保存时，查重
//        // 能得到需要的结果
//        if(point->enable == 1)
//        {
//            int index;
//            Point *pList;
//            /* 为COM功能增加判断, 如果不能找到COM从机地址对应的pList, 则直接退出 */
//            index = get_point_from_parline(point, &pList);
//            del_point_from_parline(pList, index);
//            point->enable = 0;
//        }
//
//        int tmp = point->port * 10 + _key;
//        setport(tmp);
//    }
//}
//
//void PointBinder::keyfilterDot()
//{
//    Point *point = &_cur;
//    if((counter == 0 && point->slave != 0) ||
//       (counter == 2 && point->lowSlave != 0))
//    {
//        move_right();
//    }
//}
//
//void PointBinder::keyfilterBackspace()
//{
//    Point *point = &_cur;
//    if(counter == 0) {
//        point->enable = 0;
//        point->slave = 0;
//        point->lowSlave = 0;
//        point->port = 0;
//    }
//    else if(counter == 1 || counter == 3) {
//        move_left();
//    }
//    else if(counter == 2) {
//        if(point->lowSlave == 0) move_left();
//        else point->lowSlave = point->lowSlave/10;
//    }
//    else if(counter == 4) {
//        // 设置好的点被修改时删掉在临时parline的信息
//        if(point->enable == 1) {
//            int index;
//            Point *pList;
//            /* 为COM功能增加判断, 如果不能找到COM从机地址对应的pList, 则直接退出 */
//            index = get_point_from_parline(point, &pList);
//            del_point_from_parline(pList, index);
//            point->enable = 0;
//        }
//        if(point->port == 0) move_left();
//        else point->port = point->port/10;
//    }
//}
//
//void PointBinder::shiftPoint(int n)
//{
//    if(n == 0) {
//        m_index --;
//        if(m_index < 0) m_index = m_max - 1;
//    }
//    else if(n == 1) {
//        m_index ++;
//        if(m_index == m_max) m_index = 0;
//    }
//
//    _cur = _ptr[m_index];
//}
//
//bool PointBinder::IsSetOK()
//{
//    Point* point = &_cur;
//
//    if(point->slave == 0) // 点未设置
//        goto finish;
//
//    //2016-07-22 添加 板卡无效不能设置
//    switch (point->slave) {
//    case 1://CS1
//    {
//        if(Global::g_tmpblock.cardConfig.avaCS1 == 0)
//        {
//            QMessageBox::information(NULL, "提示", "设置错误，CS1无效");
//            return false;
//        }
//        else //2016-09-06 添加 检查下位机数量
//        {
//            if(Global::g_tmpblock.cardConfig.slaveCountsCS1 == 0)
//            {
//                QMessageBox::information(NULL, "提示", "设置错误，CS1没有设置下位机");
//                return false;
//            }
//            else
//            {
//                if(point->lowSlave > Global::g_tmpblock.cardConfig.slaveCountsCS1)
//                {
//                    QMessageBox::information(NULL, "提示", "设置错误，CS1下位机设置超出板卡设置的下位机数量");
//                    return false;
//                }
//            }
//        }
//    }
//        break;
//    case 2://CS2
//    {
//        if(Global::g_tmpblock.cardConfig.avaCS2 == 0)
//        {
//            QMessageBox::information(NULL, "提示", "设置错误，CS2无效");
//            return false;
//        }
//        else //2016-09-06 添加 查查下位机数量
//        {
//            if(Global::g_tmpblock.cardConfig.slaveCountsCS2 == 0)
//            {
//                QMessageBox::information(NULL, "提示", "设置错误，CS2没有设置下位机");
//                return false;
//            }
//            else
//            {
//                if(point->lowSlave > Global::g_tmpblock.cardConfig.slaveCountsCS2)
//                {
//                    QMessageBox::information(NULL, "提示", "设置错误，CS2下位机设置超出板卡设置的下位机数量");
//                    return false;
//                }
//            }
//        }
//    }
//        break;
//    case 3://IO1
//        if(Global::g_tmpblock.cardConfig.avaIO1 == 0)
//        {
//            QMessageBox::information(NULL, "提示", "设置错误，IO1无效");
//            return false;
//        }
//        break;
//    case 4://IO2
//        if(Global::g_tmpblock.cardConfig.avaIO2 == 0)
//        {
//            QMessageBox::information(NULL, "提示", "设置错误，IO2无效");
//            return false;
//        }
//        break;
//    case 5://IN
//        if(Global::g_tmpblock.cardConfig.avaIN == 0)
//        {
//            QMessageBox::information(NULL, "提示", "设置错误，IN无效");
//            return false;
//        }
//        break;
//    case 6://COM1
//        if(Global::g_tmpblock.serialHost[0].enable == 0)
//        {
//            QMessageBox::information(NULL, "提示", "设置错误，COM1无效");
//            return false;
//        }
//        else
//        {
//            int flag = 0;
//            for(int num = 0; num < QUANTITY_HSLAVE; num++)
//            {
//                if(point->lowSlave == Global::g_tmpblock.serialHost[0].slaveInHost[num].addr && Global::g_tmpblock.serialHost[0].slaveInHost[num].enable)
//                {
//                    flag++;
//
//                    int startAddr = 0;
//                    int readCounts = 0;
//                    int portFlag = 0;
//                    for(int i = 0; i < QUANTITY_SERIAL_SEGMENTS; i++)
//                    {
//                        startAddr = Global::g_tmpblock.serialHost[0].slaveInHost[num].readReg[i].addrsReadReg;
//                        readCounts = Global::g_tmpblock.serialHost[0].slaveInHost[num].readReg[i].lenReadReg;
//                        if(point->port >= startAddr && point->port < (startAddr + readCounts))
//                        {
//                            portFlag++;
//                            break;
//                        }
//                    }
//
//                    if(portFlag == 0)
//                    {
//                        QMessageBox::information(NULL, "提示", "设置错误，寄存器地址不在串口中设置的查询范围内");
//                        return false;
//                    }
//                    else
//                        break;
//                }
//            }
//            if(flag == 0)
//            {
//                QMessageBox::information(NULL, "提示", "设置错误，COM1没有设置该从机地址");
//                return false;
//            }
//        }
//        break;
//    case 7://COM2
//        if(Global::g_tmpblock.serialHost[1].enable == 0)
//        {
//            QMessageBox::information(NULL, "提示", "设置错误，COM2无效");
//            return false;
//        }
//        else
//        {
//            int flag = 0;
//            for(int num = 0; num < QUANTITY_HSLAVE; num++)
//            {
//                if(point->lowSlave == Global::g_tmpblock.serialHost[1].slaveInHost[num].addr && Global::g_tmpblock.serialHost[1].slaveInHost[num].enable)
//                {
//                    flag++;
//                    int startAddr = 0;
//                    int readCounts = 0;
//                    int portFlag = 0;
//                    for(int i = 0; i < QUANTITY_SERIAL_SEGMENTS; i++)
//                    {
//                        startAddr = Global::g_tmpblock.serialHost[1].slaveInHost[num].readReg[i].addrsReadReg;
//                        readCounts = Global::g_tmpblock.serialHost[1].slaveInHost[num].readReg[i].lenReadReg;
//                        if(point->port >= startAddr && point->port < (startAddr + readCounts))
//                        {
//                            portFlag++;
//                            break;
//                        }
//                    }
//
//                    if(portFlag == 0)
//                    {
//                        QMessageBox::information(NULL, "提示", "设置错误，寄存器地址不在串口中设置的查询范围内");
//                        return false;
//                    }
//                    else
//                        break;
//
//                }
//            }
//            if(flag == 0)
//            {
//                QMessageBox::information(NULL, "提示", "设置错误，COM2没有设置该从机地址");
//                return false;
//            }
//        }
//        break;
/////////////////////////////////////////////////
////20180710合并
//    case 8://CS1_BS
//        if(Global::g_tmpblock.cardConfig.avaCS1 == 0)
//        {
//            QMessageBox::information(NULL, "提示", "设置错误，CS1无效");
//            return false;
//        }
//        if(point->port > 2)
//        {
//            QMessageBox::information(NULL, "提示", "设置错误，CS1沿线闭锁板上只有2个输入点");
//            return false;
//        }
//        break;
//    case 9://CS2_BS
//        if(Global::g_tmpblock.cardConfig.avaCS2 == 0)
//        {
//            QMessageBox::information(NULL, "提示", "设置错误，CS2无效");
//            return false;
//        }
//        if(point->port > 2)
//        {
//            QMessageBox::information(NULL, "提示", "设置错误，CS2沿线闭锁板上只有2个输入点");
//            return false;
//        }
//        break;
//    case 10://DIO  qh
//        if(Global::g_tmpblock.cardConfig.avaDIO == 0)
//        {
//            QMessageBox::information(NULL, "提示", "设置错误，DIO无效");
//            return false;
//        }
//        else //qh添加 检查下位机数量
//        {
//            if(Global::g_tmpblock.cardConfig.countsDIO == 0)
//            {
//                QMessageBox::information(NULL, "提示", "设置错误，DIO没有设置下位机");
//                return false;
//            }
//            else
//            {
//                if(point->lowSlave > Global::g_tmpblock.cardConfig.countsDIO)
//                {
//                    QMessageBox::information(NULL, "提示", "设置错误，DIO下位机设置超出板卡设置的下位机数量");
//                    return false;
//                }
//            }
//        }
//        break;
//    default:
//        QMessageBox::information(NULL, "提示", "设置错误，中位机号码错误");
//        break;
/////////////////////////////////////////////////
//    }
//
//    if(!IsPointComplete()) {
//        QMessageBox::information(NULL,QString::fromUtf8("提示"),QString::fromUtf8("点设置未完成"));
//        return false;
//    }
//    /*  如果该点已经设置完成, 再次进入时也要经过过合法性检测 */
//    if(point->enable == 1)
//        return true;
//    if(!store_to_parline())
//        return false;
//
//finish:
//    store_to_parameter();
//
//    return true;
//}
//
//bool PointBinder::IsPointComplete()
//{
//    Point *point = &_cur;
//    if(point->slave == 0) {
//        point->enable = 0;
//        point->lowSlave = 0;
//        point->port = 0;
//        return true;
//    }
//    else {
//        //2016-08-30 注释
////        if(point->port <= 0) return false;
////        else return true;
//
//        //2016-08-30 添加
//        int ok = 1;
//        switch (point->slave) {
//        case 1://CS1
//        case 2://CS2
//////////////////////////////////////////
////20180710合并
//        case 6://COM1
//        case 7://COM2
//        case 8://CS1_BS
//        case 9://CS2_BS
//        case 10://DIO
//        {
//            if(point->lowSlave <= 0 || point->port <= 0)
//                ok = 0;
//        }
//            break;
//////////////////////////////////////////
//        case 3://IO1
//        case 4://IO2
//        case 5://IN
//        {
//            if(point->port <= 0)
//                ok = 0;
//        }
//            break;
//        default:
//            break;
//        }
//
//        if(ok == 1)
//            return true;
//        else
//            return false;
//    }
//
//    return true;
//}
//
//int PointBinder::get_point_from_parline(Point *srcPoint, Point **destList)
//{
//    ParLine * parline = &Global::g_tmpLine;
//    int index = 0;
//    Point *point = srcPoint;
//    Point *pList = NULL;
//
//    switch(point->slave)
//    {
//    case CS1:
////        index = point->lowSlave * 4 + point->port - 1;//2016-09-02 注释
//        index = (point->lowSlave - 1) * 4 + point->port - 1;//2016-09-02 添加
//        if(point->pointWay == INPUTPOINT)
//            pList = parline->inputListCS1;
//        else
//            pList = parline->outputListCS1;
//        break;
//    case CS2:
////        index = point->lowSlave * 4 + point->port - 1;//2016-09-02 注释
//        index = (point->lowSlave - 1) * 4 + point->port - 1;//2016-09-02 添加
//        if(point->pointWay == INPUTPOINT)
//            pList = parline->inputListCS2;
//        else
//            pList = parline->outputListCS2;
//        break;
////20180710合并
//    case DIO://qh
//        index = (point->lowSlave - 1) * 12 + point->port - 1;
//        if(point->pointWay == INPUTPOINT)
//            pList = parline->inputListDIO;
//        else
//            pList = parline->outputListDIO;
//        break;
//    case IO1:
//        index = point->port - 1;
//        if(point->pointWay == INPUTPOINT)
//            pList = parline->inputListIO1;
//        else
//            pList = parline->outputListIO1;
//        break;
//    case IO2:
//        index = point->port - 1;
//        if(point->pointWay == INPUTPOINT)
//            pList = parline->inputListIO2;
//        else
//            pList = parline->outputListIO2;
//        break;
//    case IN:
//        index = point->port - 1;
//        pList = parline->inputListIN;
//        break;
//    case COM1:
//    {
//        index = point->port - 1;
//        if(point->pointWay == INPUTPOINT)
//        {
//            for(int num = 0; num < QUANTITY_HSLAVE; num++)
//            {
//                if(point->lowSlave == Global::g_tmpblock.serialHost[0].slaveInHost[num].addr && Global::g_tmpblock.serialHost[0].slaveInHost[num].enable)
//                {
//                    pList = parline->serialHostList[0].slaveList[num].inputList;
//                    break;
//                }
//            }
//        }
//    }
//        break;
//    case COM2:
//    {
//        index = point->port - 1;
//        if(point->pointWay == INPUTPOINT)
//        {
//            for(int num = 0; num < QUANTITY_HSLAVE; num++)
//            {
//                if(point->lowSlave == Global::g_tmpblock.serialHost[1].slaveInHost[num].addr && Global::g_tmpblock.serialHost[1].slaveInHost[num].enable )
//                {
//                    pList = parline->serialHostList[1].slaveList[num].inputList;
//                    break;
//                }
//            }
//        }
//    }
//        break;
/////////////////////////////////////////////
////20180710合并
//    case CS1_BS:
//        //        index = point->port - 1;//2016-09-02 添加
//        index = (point->lowSlave - 1) * 2 + (point->port - 1);
//        if(point->pointWay == INPUTPOINT)
//            pList = parline->inputListCS1BS;
//        break;
//    case CS2_BS:
//        //        index = point->port - 1;//2016-09-02 添加
//        index = (point->lowSlave - 1) * 2 + (point->port - 1);
//        if(point->pointWay == INPUTPOINT)
//            pList = parline->inputListCS2BS;
//        break;
//    default:
//        break;
//    }
//
//    *destList = pList;
//    return index;
//}
//
//bool PointBinder::set_point_to_parline(Point *pList, int index)
//{
//    if(index < 0 || pList == NULL) return false;
//    /* 如果parline中该点已经使用, 首先检测该点是否是自身 */
//    if(pList[index].enable == 1)
//    {
//        if((!strcmp(_cur.strName, pList[index].strName)) && (_cur.lowSlave == pList[index].lowSlave) && (_cur.slave == pList[index].slave) && (_cur.port == pList[index].port))
//        {
//            qDebug("%s is used by itself(slave=%d, lowslave=%d, port=%d)", _cur.strName, _cur.slave,_cur.lowSlave, _cur.port);
//            QMessageBox::information(NULL, QString::fromUtf8("提示"),  QString("测试消息: 该点被自身占用!"));
//            return true;
//        }
//        else
//        {
//            QString str = QString("该点已在") + pList[index].strName + QString("使用，请重新设置！");
//            QMessageBox::information(NULL, QString::fromUtf8("提示"), str);
//        }
//        return false;
//    }
//    else    // save to parline
//    {
//        _cur.enable = 1;
//        pList[index] = _cur;
//        return true;
//    }
//    return true;
//}
//
//bool PointBinder::del_point_from_parline(Point *pList, int index)
//{
//    if(!pList){
//        return false;
//    }
//    clear_single_point(pList + index);
//    return true;
//}
//
//bool PointBinder::store_to_parline()
//{
//    Point * point = &_cur;
//    Point * pList;
//    int index;
//    index = get_point_from_parline(point, &pList);
//
//    return set_point_to_parline(pList, index);
//}
//
//bool PointBinder::store_to_parameter()
//{
//    _ptr[m_index] = _cur;
//    return true;
//}
//
//void PointBinder::clear()
//{
//    Point * point = &_cur;
//    if(single)  //单个点
//    {
//        if(point->enable)
//        {
//            Point *pList;
//            int index;
//            index = get_point_from_parline(point, &pList);
//            del_point_from_parline(pList, index);
//            // parameter 中的点保留其他绑定时给定属性
//            point->enable = 0; point->slave = 0;
//            point->lowSlave = 0; point->port = 0;
//            *_ptr = *point;
//        }
//    }
//    else    //多个点
//    {
//        Point *pList;
//        Point *p = _ptr;
//        int index;
//        for(int i = 0; i < m_max; i++)
//        {
//            if(p[i].enable == 1)
//            {
//                index = get_point_from_parline(p+i, &pList);
//                del_point_from_parline(pList, index);
//                // parameter 中的点保留其他绑定时给定属性
//                p[i].enable = 0; p[i].slave = 0;
//                p[i].lowSlave = 0; p[i].port = 0;
//            }
//        } //end for
//
//        // clear _cur
//        point->enable = 0; point->slave = 0;
//        point->lowSlave = 0; point->port = 0;
//
//    } //end else
//
//    counter = 0;
//    m_index = 0;
//}
//
//
//// inline functions
//void PointBinder::setslave(int n)
//{
//    Point *point = &_cur;
//    if(n < 0) {n = 0; point->slave = 0; return;}
////20180710合并
//    else if(n >= Slave_Count||n == CS1_BS||n==CS2_BS)//qh
//        return;//qh
//
//    if(point->pointWay == INPUTPOINT)
//    {
////        if(!useCom &&
////            (n == COM1 || n == COM2))//2016-11-04 注释 useCom不知道用途
////            return;
//    }
//    else if(point->pointWay == OUTPUTPOINT)
//    {
//        if(n == IN || n == COM1 || n == COM2)
//            return;
//    }
//    point->slave = n;
//}
//
//void PointBinder::setlowslave(int n)
//{
//    Point *point = &_cur;
//    if(n < 0) { n = 0; point->lowSlave = 0; return; }
//
//    if(point->slave == CS1 || point->slave == CS2)
//    {
//        if(n <= LSlave_Count)
//            point->lowSlave = n;
//    }
////20180710合并
//    else if(point->slave == DIO)
//    {
//        if(n <= DIOSlaveCount)
//        {
//            point->lowSlave = n;
//        }
//    }
//    else if(point->slave == COM1 || point->slave == COM2)
//    {
//        if(n < 256)
//            point->lowSlave = n;
//    }
////20180710合并
//    else if(point->slave == CS1_BS || point->slave == CS2_BS)
//    {
//        if(n <= CS_LINE_PHONE_NUM)
//            point->lowSlave = n;
//    }
//}
//
//void PointBinder::setport(int n)
//{
//    Point *point = &_cur;
//    if(n < 0) { n = 0; point->port = 0; return; }
//
//    if(point->slave == CS1 || point->slave == CS2)
//    {
//        if(n <= 4) {point->port = n;}
//    }
////20180710合并
//    else if(point->slave == DIO)
//    {
//        if(n <=12)
//        {
//            point->port =n;
//        }
//    }
//    else if(point->slave == IO1 || point->slave == IO2)
//    {
//        if(point->pointWay == INPUTPOINT)
//        {
//            if(n <= IOINPORTNUM)  {point->port = n;}
//        }
//        else
//        {
//            if(n <= IOOUTPORTNUM) {point->port = n;}
//        }
//    }
//    else if(point->slave == IN)
//    {
//        if(n <= INPORTNUM) {point->port = n;}
//    }
//    else if(point->slave == COM1 || point->slave == COM2)
//    {
//        if(n <= QUANTITY_SERIAL_SEGMENTS * QUANTITY_SERIAL_REGISTERS)
//            point->port = n;
//    }
////20180710合并
//    else if(point->slave == CS1_BS || point->slave == CS2_BS)
//    {
//        //        if(n <= CS_LINE_PHONE_NUM) {point->port = n;}
//        if(n <= 2) {point->port = n;}
//    }
//}
//
//void PointBinder::move_left()
//{
//    Point *p = &_cur;
////20180710合并
//    if(p->slave == CS1 || p->slave == CS2 \
//            || p->slave == COM1 || p->slave == COM2 \
//            || p->slave == CS1_BS || p->slave == CS2_BS \
//            || p->slave == DIO)
//    {
//        counter != 0? counter--: counter = 0;
//    }
//    else // 只有0, 1, 4
//    {
//        if(counter == 4)
//            counter = 1;
//        else
//            counter = 0;
//    }
//}
//
//void PointBinder::move_right()
//{
//    Point *p = &_cur;
////20180710合并
//    if(p->slave == CS1 || p->slave == CS2 \
//            || p->slave == COM1 || p->slave == COM2 \
//            || p->slave == CS1_BS || p->slave == CS2_BS \
//            || p->slave == DIO)//qh
//    {
//        counter != 4? counter ++: counter = 4;
//    }
//    else    // 只有0, 1, 4
//    {
//        if(counter == 1)
//            counter = 4;
//        else
//            counter = 1;
//    }
//}
//
//void PointBinder::clear_single_point(Point *p)
//{
//    p->enable = 0;
//    p->slave = 0;
//    p->lowSlave = 0;
//    p->port = 0;
//    p->switchType = 0;
//    p->type = 0;
//    p->pointWay = 0;
//    strcpy(p->strName, "");
//}
//
//int BSPointBinder::keyEventFilter(QKeyEvent *ev)
//{
//    int ret = kd_check;
//    int keyVal = ev->key();
//
//    switch (keyVal) {
//    /// Number
//    case Qt::Key_0:case Qt::Key_1:case Qt::Key_2:
//    case Qt::Key_3:case Qt::Key_4:case Qt::Key_5:
//    case Qt::Key_6:case Qt::Key_7:case Qt::Key_8:
//    case Qt::Key_9:
//        keyfilterNum(keyVal);
//        break;
//        /// BACKSPACE
//    case Qt::Key_Backspace:
//        keyfilterBackspace();
//        break;
//        /// DOT
//    case Qt::Key_Period:
//        keyfilterDot();
//        break;
//    case Qt::Key_T:
//        if(single) return kd_null;
//        if(!IsSetOK()) {
//            return kd_null;
//        }
//        shiftPoint(0);
//        ret = kd_show_emit;
//        break;
//    case Qt::Key_K:
//        if(single) return kd_null;
//        if(!IsSetOK()) {
//            return kd_null;
//        }
//        shiftPoint(1);
//        ret = kd_show_emit;
//        break;
//    case Qt::Key_Up:
//    case Qt::Key_Down:
//        if(IsSetOK())
//        {
//            //m_index = 0; _cur = _ptr[0];
//            /*changed by wujinping: 修正切换控件显示时点显示不完整的问题 */
//            ret = kd_show_emit;
//        }
//        else ret = kd_null;
//        break;
//    case Qt::Key_Return:
//        if(IsSetOK())
//        { ret = kd_check_emit; m_index = 0;}
//        else ret = kd_null;
//        break;
//    case Qt::Key_Escape:
//        ret = kd_emit; m_index = 0;
//        break;
//    default:
//        break;
//    }
//
//    return ret;
//}
//
//QString BSPointBinder::checkSet()
//{
//    QString output;
//    Point *point = &_cur;
//
//    if(point->slave == 0)
//        counter = 0;
//
//    switch (counter) {
//    case 0:
//        output = QString(SlaveName[point->slave].c_str());
//        break;
//    case 1:
//        output = QString(SlaveName[point->slave].c_str()) +
//                QChar('.');
//        break;
//    case 2:
//        output = QString(SlaveName[point->slave].c_str()) +
//                QChar('.') + QString::number(point->port);
//        break;
//    default:
//        break;
//    }
//
//    return output;
//}
//
//QString BSPointBinder::showSet()
//{
//    QString output;
//    Point *point = & _cur;
//
//    if(point->slave == 0) {
//        output = "0";
//        counter = 0;
//    } else {
//
//            output = QString(SlaveName[point->slave].c_str()) + QChar('.') +
//                    QString::number(point->port);
//        counter = 2;
//    }
//    return output;
//}
//
//void BSPointBinder::keyfilterNum(int key)
//{
//    int _key = key - 0x30;
//    Point *point = &_cur;
//    if(counter == 0){
//        setslave(_key);
//    }
//    else if(counter%2 == 1){
//        move_right();
//        keyfilterNum(key);
//    }
//    else if(counter == 2){
//        // 设置好的点被修改时删掉在临时parline的信息
//        // 这个时候删除可以保证在页面未保存时，查重
//        // 能得到需要的结果
//        if(point->enable == 1)
//        {
//            int index;
//            Point *pList;
//            index = get_point_from_parline(point, &pList);
//            del_point_from_parline(pList, index);
//            point->enable = 0;
//        }
//
//        int tmp = point->port * 10 + _key;
//        setport(tmp);
//    }
//}
//
//void BSPointBinder::keyfilterDot()
//{
//    Point *point = &_cur;
//    if(counter == 0 && point->slave != 0)
//    {
//        move_right();
//    }
//}
//
//void BSPointBinder::keyfilterBackspace()
//{
//    Point *point = &_cur;
//    if(counter == 0) {
//        point->enable = 0;
//        point->slave = 0;
//        point->lowSlave = 0;
//        point->port = 0;
//    }
//    else if(counter == 1) {
//        move_left();
//    }
//
//    else if(counter == 2) {
//        // 设置好的点被修改时删掉在临时parline的信息
//        if(point->enable == 1) {
//            int index;
//            Point *pList;
//            index = get_point_from_parline(point, &pList);
//            del_point_from_parline(pList, index);
//            point->enable = 0;
//        }
//        if(point->port == 0) move_left();
//        else point->port = point->port/10;
//    }
//}
//
//bool BSPointBinder::IsSetOK()
//{
//    Point* point = &_cur;
//    if(point->enable == 1)
//        return true;
//
//    if(point->slave == 0) // 点未设置
//        goto finish;
//
//    //2016-07-22 添加 板卡无效不能设置
//    switch (point->slave) {
//        case 1://CS1
//        {
//            if(Global::g_tmpblock.cardConfig.avaCS1 == 0)
//            {
//                QMessageBox::information(NULL, "提示", "设置错误，CS1无效");
//                return false;
//            }
//        }
//        break;
//        case 2://CS2
//        {
//            if(Global::g_tmpblock.cardConfig.avaCS2 == 0)
//            {
//                QMessageBox::information(NULL, "提示", "设置错误，CS2无效");
//                return false;
//            }
//        }
//        break;
//    default:
//        QMessageBox::information(NULL, "提示", "设置错误，中位机号码错误");
//        return false;
//    }
//
//
//    if(!IsPointComplete()) {
//        QMessageBox::information(NULL,QString::fromUtf8("提示"),QString::fromUtf8("点设置未完成"));
//        return false;
//    }
//
//    if(!store_to_parline())
//        return false;
//
//finish:
//    store_to_parameter();
//
//    return true;
//}
//
//bool BSPointBinder::IsPointComplete()
//{
//    Point *point = &_cur;
//    if(point->slave == 0) {
//        point->enable = 0;
//        point->lowSlave = 0;
//        point->port = 0;
//        return true;
//    }
//    else {
//        //2016-08-30 注释
////        if(point->port <= 0) return false;
////        else return true;
//
//        //2016-08-30 添加
//        int ok = 1;
//        switch (point->slave) {
//            case 1://CS1
//            case 2://CS2
//               if( point->port <= 0)
//                    ok = 0;
//               break;
//        }
//        if(ok == 1)
//            return true;
//        else
//            return false;
//    }
//
//    return true;
//}
//
//int BSPointBinder::get_point_from_parline(Point *srcPoint, Point **destList)
//{
//    ParLine * parline = &Global::g_tmpLine;
//    int index = 0;
//    Point *point = srcPoint;
//    Point *pList;
//
//    switch(point->slave)
//    {
//    case CS1:
//        index = point->port - 1;//2016-09-02 添加
//        if(point->pointWay == INPUTPOINT)
//            pList = parline->inputListCS1BS;
//        break;
//    case CS2:
//        index = point->port - 1;//2016-09-02 添加
//        if(point->pointWay == INPUTPOINT)
//            pList = parline->inputListCS2BS;
//        break;
//    default:
//        break;
//    }
//
//    *destList = pList;
//    return index;
//}
//
//bool BSPointBinder::set_point_to_parline(Point *pList, int index)
//{
//    if(index < 0) return false;
//
//    if(pList[index].enable == 1)
//    {
//        QString str = QString("该点已在") + pList[index].strName + QString("使用，请重新设置！");
//        QMessageBox::information(NULL, QString::fromUtf8("提示"), str);
//        return false;
//    }
//    else    // save to parline
//    {
//        _cur.enable = 1;
//        pList[index] = _cur;
//        return true;
//    }
//    return true;
//}
//
//bool BSPointBinder::del_point_from_parline(Point *pList, int index)
//{
//    clear_single_point(pList + index);
//    return true;
//}
//
//bool BSPointBinder::store_to_parline()
//{
//    Point * point = &_cur;
//    Point * pList;
//    int index;
//    index = get_point_from_parline(point, &pList);
//
//    return set_point_to_parline(pList, index);
//}
//
//bool BSPointBinder::store_to_parameter()
//{
//    _ptr[m_index] = _cur;
//    return true;
//}
//
//void BSPointBinder::clear()
//{
//    Point * point = &_cur;
//    if(single)  //单个点
//    {
//        if(point->enable)
//        {
//            Point *pList;
//            int index;
//            index = get_point_from_parline(point, &pList);
//            del_point_from_parline(pList, index);
//            // parameter 中的点保留其他绑定时给定属性
//            point->enable = 0; point->slave = 0;
//            point->lowSlave = 0; point->port = 0;
//            *_ptr = *point;
//        }
//    }
//    else    //多个点
//    {
//        Point *pList;
//        Point *p = _ptr;
//        int index;
//        for(int i = 0; i < m_max; i++)
//        {
//            if(p[i].enable == 1)
//            {
//                index = get_point_from_parline(p+i, &pList);
//                del_point_from_parline(pList, index);
//                // parameter 中的点保留其他绑定时给定属性
//                p[i].enable = 0; p[i].slave = 0;
//                p[i].lowSlave = 0; p[i].port = 0;
//            }
//        } //end for
//
//        // clear _cur
//        point->enable = 0; point->slave = 0;
//        point->lowSlave = 0; point->port = 0;
//
//    } //end else
//
//    counter = 0;
//    m_index = 0;
//}
//
//
//// inline functions
//void BSPointBinder::setslave(int n)
//{
//    Point *point = &_cur;
//    if(n < 0) {n = 0; point->slave = 0; return;}
//    else if(n > 2) return;
//
//    point->slave = n;
//}
//
//void BSPointBinder::setport(int n)
//{
//    Point *point = &_cur;
//    if(n < 0) { n = 0; point->port = 0; return; }
//
//    if(point->slave == CS1 || point->slave == CS2)
//    {
//        if(n <= CS_LINE_PHONE_NUM) {point->port = n;}
//    }
//}
//
//void BSPointBinder::move_left()
//{
//    Point *p = &_cur;
//    if(p->slave == CS1 || p->slave == CS2)
//    {
//        counter != 0? counter--: counter = 0;
//    }
//}
//
//void BSPointBinder::move_right()
//{
//    Point *p = &_cur;
//    if(p->slave == CS1 || p->slave == CS2 )
//    {
//        counter != 2? counter ++: counter = 2;
//    }
//}
//
//void BSPointBinder::clear_single_point(Point *p)
//{
//    p->enable = 0;
//    p->slave = 0;
//    p->lowSlave = 0;
//    p->port = 0;
//    p->switchType = 0;
//    p->type = 0;
//    p->pointWay = 0;
//    strcpy(p->strName, "");
//}
//
//
//
////20180710合并，新的SuperPointBinder_2类
//void SuperPointBinder_2::setslave(int n)
//{
//    Point *point = &_cur;
//    if(n < 0) {n = 0; point->slave = 0; return;}
//    else if(n >= Slave_Count)//qh
//        return;//qh
//
//    if(point->pointWay == INPUTPOINT)
//    {
//        //        if(!useCom &&
//        //            (n == COM1 || n == COM2))//2016-11-04 注释 useCom不知道用途
//        //            return;
//    }
//    else if(point->pointWay == OUTPUTPOINT)
//    {
//        if(n == IN || n == COM1 || n == COM2)
//            return;
//    }
//    point->slave = n;
//}
//
//
//
//
//
//
//
//
//
//
//
//
////20180710合并
//int BindMultSelection::keyEventFilter(QKeyEvent *ev)
//{
//    int ret = kd_check;
//    int keyVal = ev->key();
//    switch(keyVal)
//    {
//    case Qt::Key_K:
//        shift(1);
//        ret = kd_check_emit;
//        break;
//    case Qt::Key_T:
//        shift(0);
//        ret = kd_check_emit;
//        break;
//    default:
//        ret = BindSelectionList.at(m_cur)->keyEventFilter(ev);
//        break;
//    }
//    return ret;
//}
//
//QString BindMultSelection::checkSet()
//{
//    return BindSelectionList.at(m_cur)->checkSet();
//}
//
//void BindMultSelection::ResetPointNum()
//{
//    m_cur = 0;
//}
//
//void BindMultSelection::shift(int n)
//{
//    if(n == 0) {
//        m_cur --;
//        if(m_cur < 0) m_cur = m_max - 1;
//    }
//    else if(n == 1) {
//        m_cur ++;
//        if(m_cur == m_max) m_cur = 0;
//    }
//}
//
//int  BindMultSelection::GetPointNum()
//{
//    return m_cur;
//}
//
//
//int BindMultInt::keyEventFilter(QKeyEvent *ev)
//{
//    int ret = kd_check;
//    int keyVal = ev->key();
//    switch(keyVal)
//    {
//    case Qt::Key_K:
//        shift(1);
//        ret = kd_check_emit;
//        break;
//    case Qt::Key_T:
//        shift(0);
//        ret = kd_check_emit;
//        break;
//    default:
//        ret = BindIntList.at(m_cur)->keyEventFilter(ev);
//        break;
//    }
//    return ret;
//}
//
//QString BindMultInt::checkSet()
//{
//    return BindIntList.at(m_cur)->checkSet();
//}
//
//QString BindMultInt::showSet()
//{
//    return BindIntList.at(m_cur)->showSet();
//}
//
//int BindMultInt::GetPointNum()
//{
//    return m_cur;
//}
//
//void BindMultInt::ResetPointNum()
//{
//    m_cur = 0;
//}
//
//void BindMultInt::shift(int n)
//{
//    if(n == 0) {
//        m_cur --;
//        if(m_cur < 0) m_cur = m_max - 1;
//    }
//    else if(n == 1) {
//        m_cur ++;
//        if(m_cur == m_max) m_cur = 0;
//    }
//}
//
