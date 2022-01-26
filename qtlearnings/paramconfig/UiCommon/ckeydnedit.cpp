#include "ckeydnedit.h"
#include "bindobj.h"
#include <QKeyEvent>
#include <set>
#include <QDebug>
//#include <QMessageBox>


CKeyDnEdit::CKeyDnEdit(QWidget *parent) :
    CStateLineEdit(parent)
{
    setAccessibleName("");
    pBind = NULL;
    XOffset = 0;
    YOffset = 0;
    m_editing = false;
    m_itemIdx = 0;
    //connect(this, SIGNAL(textEdited(const QString &)), this, SLOT(slot_textEdited(const QString&)));
    //connect(this, SIGNAL(textChanged(const QString &)), this, SLOT(slot_textChanged(const QString&)));
}

void CKeyDnEdit::focusInEvent(QFocusEvent *ev)
{
    //setStyleSheet("background-color:rgba(200,60,60,100%)");
    QLineEdit::focusInEvent(ev);
    qDebug()<<__func__;
}

void CKeyDnEdit::focusOutEvent(QFocusEvent *ev)
{
    setEditText(text());
    m_editing = false;
    QLineEdit::focusOutEvent(ev);
    qDebug()<<__func__;
}

void CKeyDnEdit::keyPressEvent(QKeyEvent *ev)
{
    // 未绑定数据直接返回按键事件
    qDebug()<<"key press";
    switch (ev->key()) {
        case Qt::Key_Down:
        case Qt::Key_Up:
        case Qt::Key_X:
        case Qt::Key_Escape:
            ev->setAccepted(false);
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
        case Qt::Key_Backspace:
            QLineEdit::keyPressEvent(ev);
            break;
        case Qt::Key_Left:
        case Qt::Key_Right:
            if (nullptr !=pBind) {
                pBind->keyEventFilter(ev);
                setText(pBind->showSet());
            }
            break;
        case Qt::Key_Return:
            if (m_editing) {
                setEditText(text());
                m_editing = false;
            }else ev->setAccepted(false);
            return ;
    }
    if(pBind == NULL)
    {
        emit this->keydown(ev);
        return;
    }

    pBind->setState(IBindObj::BS_SOURCEKEY);
    //pBind->keyEventFilter(ev);
    m_editing = true;
    //setText(pBind->showSet());
}

void CKeyDnEdit::showText()
{
    if(pBind != NULL)
        this->setText(pBind->showSet());
}

#if 0
void CKeyDnEdit::slot_textEdited(const QString& newTxt)
{
    qDebug()<<"text edited";
}
void CKeyDnEdit::slot_textChanged(const QString& newTxt)
{
    if (nullptr != pBind){
        if (!pBind->isFromKeyEvent()){
            //set from paragm write
            if(!pBind->setValue(newTxt))
                setText(pBind->showSet());
        }

        if (pBind->isModified()){
            //emit other data by UI
            BindUint16 *pBindU16 = dynamic_cast<BindUint16*>(pBind);
            if (nullptr != pBindU16){
                emit sig_valueChanged(pBindU16->valPtr(), *(pBindU16->valPtr()));
            }
        }

        pBind->clearState();
    }
    qDebug()<<"text changed";
}
#endif
bool CKeyDnEdit::setEditText(const QString &strText)
{
    bool bRet = true;
    if (nullptr != pBind){
        bRet = pBind->setValue(strText);
        setText(pBind->showSet());
        if (pBind->isModified()){
            BindUint16 *pBindU16 = dynamic_cast<BindUint16*>(pBind);
            BindUint16Ptr *pBindU16Ptr = dynamic_cast<BindUint16Ptr*>(pBind);
            if (nullptr != pBindU16){
                emit sig_valueChanged(pBindU16->valPtr(), *(pBindU16->valPtr()));
            }else if (nullptr != pBindU16Ptr){
                emit sig_valueChanged(pBindU16Ptr->valPtr(), *(pBindU16Ptr->valPtr()));
            }
            pBind->clearState();
        }
    }else {
        setText(strText);
    }

    return bRet;
}

void CKeyDnEdit::onRangeChanged()
{
    if (nullptr != pBind){
        pBind->onRangeChanged();
        if (pBind->isModified()){
            setText(pBind->showSet());
            pBind->clearState();

            BindUint16Ptr *pBindU16Ptr = dynamic_cast<BindUint16Ptr*>(pBind);
            if (nullptr != pBindU16Ptr){
                emit sig_valueChanged(pBindU16Ptr->valPtr(), *(pBindU16Ptr->valPtr()));
            }
        }
    }
}

void CKeyDnEdit::bindDataPtr(u_int16_t* pVal)
{
    if (nullptr != pVal && nullptr != pBind){
        BindUint16Ptr *pBindU16Ptr = dynamic_cast<BindUint16Ptr*>(pBind);
        if (nullptr != pBindU16Ptr){
            pBindU16Ptr->SetValPtr(pVal);
        }
        qWarning()<<pVal<<*pVal;

        setText(pBind->showSet());
    }
}

void CKeyDnEdit::slot_dataSetChanged(uint16_t* pAddr, uint16_t setSize)
{
    if (m_itemIdx < setSize)
        bindDataPtr(pAddr + m_itemIdx);
}

//CKeyDnSetIndexEdit
CKeyDnSetIndexEdit::CKeyDnSetIndexEdit(QWidget *parent) :
    CKeyDnEdit(parent)
{
    pMin = nullptr;
    pMax = nullptr;
    pData = nullptr;
    setSize = 0;
    setCnt = 0;
}

void CKeyDnSetIndexEdit::keyPressEvent(QKeyEvent *ev)
{
    switch (ev->key()) {
        case Qt::Key_Left:
            if (nullptr !=pData) {
                uint16_t index = text().toUShort();//from 1
                index--;
                if (!isValid(index))
                    index = closeValue(index);
                setText(QString::number(index));
                emit sig_dataSetChanged(pData + (index - 1)*setSize, setSize);
            }
            break;
        case Qt::Key_Right:
            if (nullptr !=pData) {
                uint16_t index = text().toUShort();//from 1
                index++;
                if (!isValid(index))
                    index = closeValue(index);
                setText(QString::number(index));
                emit sig_dataSetChanged(pData + (index - 1)*setSize, setSize);
            }
            break;
        default:
            CKeyDnEdit::keyPressEvent(ev);
            break;
    }
}

void CKeyDnSetIndexEdit::showText()
{
}

bool CKeyDnSetIndexEdit::isValid(uint16_t val)
{
    if (*pMin <= val && val <= *pMax)
        return true;
    return false;
}
uint16_t CKeyDnSetIndexEdit::closeValue(uint16_t val)
{
    if (val < *pMin)
        val = *pMin;
    else if (val > *pMax)
        val = *pMax;
    if (1 > val)
        val = 1;

    return val;
}

bool CKeyDnSetIndexEdit::setEditText(const QString &strText)
{
    QString currText = text();
    bool bRet = true;
    if (nullptr != pData && 0 < setSize && 0 < setCnt){
        uint16_t index = strText.toUShort();//from 1
        if (!isValid(index))
            index = closeValue(index);
        setText(QString::number(index));
        emit sig_dataSetChanged(pData + (index - 1)*setSize, setSize);
    }else {
        setText(strText);
    }

    return bRet;
}

//CKeyDnSetEdit
CKeyDnSetEdit::CKeyDnSetEdit(QWidget *parent) :
    CKeyDnEdit(parent)
{
    pMin = nullptr;
    pMax = nullptr;
    pData = nullptr;
    setSize = 0;
}

void CKeyDnSetEdit::keyPressEvent(QKeyEvent *ev)
{
    qWarning()<<"CKeyDnSetEdit::keyPressEvent"<<ev->key();
    switch (ev->key()) {
        case Qt::Key_Left:
        case Qt::Key_Right:
        case Qt::Key_Comma:
            QLineEdit::keyPressEvent(ev);
            break;
        default:
            CKeyDnEdit::keyPressEvent(ev);
            break;
    }
}

void CKeyDnSetEdit::showText()
{
    QString txt;
    if (nullptr != pData && nullptr != pMin && 0 < setSize){
        for (int i = 0; i < setSize; i++){
            if (!isValid(*(pData + i)))
                break;
            if (0 < i) txt.append(',');
            txt.append(QString::number(*(pData+i)));
        }
    }
    setText(txt);
}

bool CKeyDnSetEdit::isValid(uint16_t val)
{
    if (*pMin <= val && val <= *pMax)
        return true;
    return false;
}
bool CKeyDnSetEdit::setEditText(const QString &strText)
{
    QString currText = text();
    bool bRet = true;
    if (nullptr != pData && 0 < setSize ){
        int dataCnt = 0;
        set<uint16_t> valSet;
        QStringList itemList = currText.split(',');
        for (int i = 0; i < itemList.count() && i < setSize; i++){
            uint16_t val = static_cast<uint16_t>(itemList[i].trimmed().toInt());
            if (isValid(val) && valSet.find(val) == valSet.end()){
                *(pData + dataCnt) = val;
                dataCnt++;
                valSet.insert(val);
            }
        }

        *(pData + dataCnt) = *pMin - 1;//end;
        showText();
    }else {
        setText(strText);
    }

    return bRet;
}
QString CKeyDnSetEdit::tipInfo()
{
    QString strTip("用数字键输入\n可输入多项，用，分割");

    return strTip;
}
