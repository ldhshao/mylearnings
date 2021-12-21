#include "ckeydnedit.h"
#include "bindobj.h"
#include <QKeyEvent>
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
            if (nullptr != pBindU16){
                emit sig_valueChanged(pBindU16->valPtr(), *(pBindU16->valPtr()));
            }
            pBind->clearState();
        }
    }else {
        setText(strText);
    }

    return bRet;
}
