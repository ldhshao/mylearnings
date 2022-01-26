#include "cdevpointedit.h"
#include "cdevposmgr.h"
#include "clineselector.h"
#include <QKeyEvent>
#include <QToolTip>
#include <QTimer>
#include <QDebug>

CDevPointEdit::CDevPointEdit(QWidget *parent):QLineEdit(parent)
{
    pVal = nullptr;
    state = DPES_IDLE;
    step = 0;
    m_itemIdx = 0;
    usrKey = 0;
    timerInterval = 400;
    emitTimer = new QTimer(this);
    connect(emitTimer, SIGNAL(timeout()), this, SLOT(slot_emitTimer()));
}

CDevPointEdit::CDevPointEdit(const QString &text, QWidget *parent):QLineEdit(text, parent)
{
    pVal = nullptr;
    state = DPES_IDLE;
    step = 0;
}

void CDevPointEdit::setValuePtr(uint16_t* val)
{
    pVal = val;
    showText();
}

void CDevPointEdit::setValue(uint32_t val)
{
    uint32_t oldVal = (*(pVal + 1) << 16) + *pVal;
    qDebug()<<"old, new "<<oldVal<<val;
    if (val != oldVal){
        CDevPosMgr::instance()->setDevPoint(oldVal, portType, false);
        CDevPosMgr::instance()->setDevPoint(val, portType, true);
        *pVal = val;
        *(pVal + 1) = (val >> 16);
        qDebug()<<"write addr "<<pVal<<" val "<<val;
        emit sig_valueChanged(pVal, val);
        if (val < 0xFFFF)
            emit sig_valueChanged(pVal+1, *(pVal + 1));
    }
    showText();
}

bool CDevPointEdit::setEditText(const QString &strText)
{
    bool bRet = true;
    setValue(CDevPosMgr::instance()->makeDevPoint(strText));

    return bRet;
}

uint32_t CDevPointEdit::getValue()
{
    return (*pVal + (*(pVal + 1)<<16));
}

void CDevPointEdit::showText()
{
    if (nullptr != pVal){
        uint32_t val = getValue();
        setText(CDevPosMgr::instance()->makeStrDevPoint(val));
    }
}

void CDevPointEdit::updateText()
{
    if (nullptr != pVal){
        uint32_t val = getValue();
        CDevPosMgr::instance()->setDevPoint(val, portType, true);
        setText(CDevPosMgr::instance()->makeStrDevPoint(val));
    }
}

void CDevPointEdit::slot_emitTimer()
{
    emitTimer->stop();
    qDebug()<<__FUNCTION__<<usrKey;
    usrKey--;
    if (usrKey < 0) usrKey = 0;
    if (usrKey >= 18)
        usrKey = 18 - 1;
    QString lvl1Names[18] = {
        "CS1", "CS2", "DIO1", "CS1_BS", "CS2_BS",
        "COM1", "COM2", "COM3", "TCP", "CS3", "DIO2", "CS3_BS",
        "CS4", "DIO3", "CS4_BS", "CS5", "DIO4", "CS5_BS"
    };
    setText(lvl1Names[usrKey]);
    usrKey = 0;
}

void CDevPointEdit::keyPressEvent(QKeyEvent *e)
{
    int k = e->key();
    switch (k) {
    case Qt::Key_Down:
    case Qt::Key_Up:
    case Qt::Key_Escape:
        e->setAccepted(false);
        break;
    case Qt::Key_Return:
        if (DPES_IDLE == state){
            mouseReleaseEvent(nullptr);
        }else if (DPES_EDITING == state){
            uint32_t devPt = CDevPosMgr::instance()->makeDevPoint(text());//enrich
            if (0 == devPt){
                setValue(devPt);
            }else if (CDevPosMgr::instance()->isDevPointValid(devPt, portType)){
                if (CDevPosMgr::instance()->isDevPointAvailable(devPt, portType)){
                    setValue(devPt);
                    state = DPES_CONFIRM;
                }else {
                    QToolTip::showText(mapToGlobal(QPoint(0, height()/2)), text() + "已被占用");
                }
            }else {
                QToolTip::showText(mapToGlobal(QPoint(0, height()/2)), "设备地址设置错");
            }
        }else if (DPES_CONFIRM == state){
            state = DPES_IDLE;
            step = 0;
            e->setAccepted(false);
        }
    qDebug()<<"CDevPointEdit state"<<state;
        break;
    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
        if (text().isEmpty()) {
            if(emitTimer->isActive()) emitTimer->stop();
            usrKey = usrKey * 10 + (k - Qt::Key_0);
            emitTimer->start(timerInterval);
        }else {
            QLineEdit::keyPressEvent(e);
        }
        state = DPES_EDITING;
        break;
    case Qt::Key_Period:
        if (-1 == text().indexOf('.')){//second level
            int l = CDevPosMgr::instance()->getLineNo(text());
            if (-1 < l){
                list<int> mList = CDevPosMgr::instance()->getAvailableMachines(l, portType);
                QString strInfo;
                for (auto it = mList.begin(); it != mList.end(); it++){
                 if (mList.begin() == it)
                      strInfo.append(QString::number(*it + 1));
                 else
                      strInfo.append(", " + QString::number(*it + 1));
                }
                if (!strInfo.isEmpty()){
                    QToolTip::showText(mapToGlobal(QPoint(0, height()/2)), "可用的下位机:\n" + strInfo);
                }
                QLineEdit::keyPressEvent(e);
            }else {
                QToolTip::showText(mapToGlobal(QPoint(0, height()/2)), tipInfo());
            }
        }else if (1 == text().count('.')){//third level
            QStringList devList = text().split('.');
            bool bOK =false;
            int l = CDevPosMgr::instance()->getLineNo(devList[0]);
            int m = devList[1].toInt(&bOK);
            if (-1 == l){
                QToolTip::showText(mapToGlobal(QPoint(0, height()/2)), "中位机设置错误\n");
            }else if (!bOK){
                QToolTip::showText(mapToGlobal(QPoint(0, height()/2)), "下位机设置错误\n");
            }else{
                bOK = false; m--;
                list<int> mList = CDevPosMgr::instance()->getAvailableMachines(l, portType);
                for (auto it = mList.begin(); it != mList.end(); it++){
                    if (m == *it){ bOK = true; break; }
                }
                if (!bOK){
                    QToolTip::showText(mapToGlobal(QPoint(0, height()/2)), "下位机设置错误\n");
                }else {
                QString strInfo;
                list<bool> puList = CDevPosMgr::instance()->getMachinePorts(l, m, portType);
                int i = 0;
                for (auto it = puList.begin(); it!= puList.end(); it++, i++){
                    if (!*it) {
                        if (strInfo.isEmpty()) strInfo.append(QString::number(i+1));
                        else strInfo.append(", " + QString::number(i + 1));
                    }
                }
                QToolTip::showText(mapToGlobal(QPoint(0, height()/2)), "可用的口号:\n" + strInfo);
                QLineEdit::keyPressEvent(e);
                }
            }
        }
        state = DPES_EDITING;
        break;
    case Qt::Key_Backspace:
        QLineEdit::keyPressEvent(e);
        state = DPES_EDITING;
        break;
    default:
        if ((Qt::Key_A <= k && k <= Qt::Key_Z))
            e->setAccepted(false);
        else{
            qDebug()<<k;
            QLineEdit::keyPressEvent(e);
        }
    }
}
void CDevPointEdit::mouseReleaseEvent(QMouseEvent *e)
{
    state = DPES_EDITING;
    QPoint pt = mapToGlobal(QPoint(0,0));
    int w = width(), h = height();
    CLineSelector::instance()->adjustPosition(pt.rx(), pt.ry(), w, h);
    uint32_t val = *pVal + (*(pVal + 1)<<16);
    qDebug()<<"read addr "<<pVal<<" val "<<val;
    CLineSelector::instance()->show();
    //clearFocus();

    CPortSelector::instance()->setAttachEdit(this);
    CPortSelector::instance()->setPortType(portType);
    if (CDevPosMgr::instance()->isDevPointValid(val, portType)){
        int l = get_line_from_dev_point(val) - 1;
        int m = get_machine_from_dev_point(val);
        int p = get_port_from_dev_point(val);
        CLineSelector::instance()->selectButtonByIndex(l);
        CMachineSelector::instance()->selectButtonByIndex(m);
        CPortSelector::instance()->activateWindow();
    }else{
        CLineSelector::instance()->selectButtonByIndex(-1);
        CLineSelector::instance()->activateWindow();
    }
}

void CDevPointEdit::focusInEvent(QFocusEvent *event)
{
    qDebug()<<"CDevPointEdit::focusInEvent";
    //setStyleSheet("background-color:rgba(200,60,60,100%)");
    QLineEdit::focusInEvent(event);
}
void CDevPointEdit::focusOutEvent(QFocusEvent *event)
{
    qDebug()<<"CDevPointEdit::focusOutEvent";
    qDebug()<<"CDevPointEdit state"<<state;
    if (DPES_EDITING == state){
        //setEditText(text());
            uint32_t devPt = CDevPosMgr::instance()->makeDevPoint(text());//enrich
            if (0 == devPt){
                setValue(devPt);
            }else if (CDevPosMgr::instance()->isDevPointAvailable(devPt, portType)){
                setValue(devPt);
            }else{
                showText();
            }
    }
        QLineEdit::focusOutEvent(event);
        state = DPES_IDLE;
}

void CDevPointEdit::endEdit()
{
    qDebug()<<"CDevPointEdit endEdit state"<<state;
    //if (DPES_EDITING == state) state = DPES_CONFIRM;
    state = DPES_CONFIRM;
    qDebug()<<"CDevPointEdit state"<<state;
    parentWidget()->activateWindow();
}

QString CDevPointEdit::tipInfo()
{
    if (CDevPosMgr::PORTTYPE_OUT == portType)
        return "键1=CS1, 2=CS2, 3=DIO1, 4=CS1_BS, 5=CS2_BS\n  6=COM1, 7=COM2, 8=COM3, 9=TCP,\n  10=CS3, 11= DIO2, 12=CS3_BS, 13=CS4, 14= DIO3, 15=CS4_BS, 16=CS5, 17= DIO4, 18=CS5_BS\n用*设置中位机，下位机，口号";
    return "键1=CS1, 2=CS2, 3=DIO1, 4=CS1_BS, 5=CS2_BS\n  10=CS3, 11= DIO2, 12=CS3_BS, 13=CS4, 14= DIO3, 15=CS4_BS, 16=CS5, 17= DIO4, 18=CS5_BS\n用*设置中位机，下位机，口号";
    return "键1=CS1, 2=CS2, 3=CS3, 4=CS4, 5=CS1_BS, 6=CS2_BS, 7=CS3_BS, 8=CS4_CS, A=DIO1, B= DIO2, C=DIO2\n用*设置中位机，下位机，口号";
}
void CDevPointEdit::slot_dataSetChanged(uint16_t* pAddr, uint16_t setSize)
{
    if (m_itemIdx < setSize)
        setValuePtr(pAddr + m_itemIdx);
}
