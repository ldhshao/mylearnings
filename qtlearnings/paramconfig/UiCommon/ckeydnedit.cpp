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
    connect(this, SIGNAL(textEdited(const QString &)), this, SLOT(slot_textEdited(const QString&)));
    connect(this, SIGNAL(textChanged(const QString &)), this, SLOT(slot_textChanged(const QString&)));
}

void CKeyDnEdit::focusInEvent(QFocusEvent *ev)
{
    //setStyleSheet("background-color:rgba(200,60,60,100%)");
    QLineEdit::focusInEvent(ev);
    qDebug()<<__func__;
}

void CKeyDnEdit::focusOutEvent(QFocusEvent *ev)
{
    QLineEdit::focusOutEvent(ev);
    qDebug()<<__func__;
}
////qh滤波处理 不处理tab按键
//bool CKeyDnEdit::event(QEvent *ev)
//{
//    if (ev->type() == QEvent::KeyPress)
//    {
//        QKeyEvent *ke = static_cast<QKeyEvent *>(ev);
//        if (ke->key() == Qt::Key_Tab || ke->key() == Qt::Key_Backtab)
//        {
//            return true;
//        }
//    }
//    return QWidget::event(ev);
//}
//
//void CKeyDnEdit::needDisplay(QKeyEvent *ev)
//{
//    /*字符加数字则直接显示*/
//    if((ev->key() >= Qt::Key_0 && ev->key() <= Qt::Key_9) || (ev->key() >= Qt::Key_A && ev->key() <= Qt::Key_Z) || ev->key() == Qt::Key_Backspace){
//        QLineEdit::keyPressEvent(ev);
//    }
//}

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
    }
    if(pBind == NULL)
    {
        emit this->keydown(ev);
        return;
    }

    pBind->setState(IBindObj::BS_SOURCEKEY);
    pBind->keyEventFilter(ev);
    setText(pBind->showSet());
}

void CKeyDnEdit::showText()
{
    if(pBind != NULL)
        this->setText(pBind->showSet());
}

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
        }

        BindUint16 *pBindU16 = dynamic_cast<BindUint16*>(pBind);
        if (nullptr != pBindU16){
            emit sig_valueChanged(pBindU16->valPtr(), *(pBindU16->valPtr()));
        }

        pBind->clearState();
    }
    qDebug()<<"text changed";
}
bool CKeyDnEdit::setEditText(const QString &strText)
{
    bool bRet = true;
    if (nullptr != pBind){
        bRet = pBind->setValue(strText);
        setText(pBind->showSet());
    }else {
        setText(strText);
    }

    return bRet;
}
