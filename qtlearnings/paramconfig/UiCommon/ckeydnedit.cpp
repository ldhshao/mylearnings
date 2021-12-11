#include "ckeydnedit.h"
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
    setStyleSheet("background-color:rgba(200,60,60,100%)");
    qDebug()<<__func__;
}

void CKeyDnEdit::focusOutEvent(QFocusEvent *ev)
{
    setStyleSheet("");
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
    /*
    if(ret == kd_check) //是否发送按键事件给窗体
    {
        this->setText(pBind->checkSet());
    }
    else if(ret == kd_emit)
    {
        emit this->keydown(ev);
    }
    else if(ret == kd_check_emit)
    {
        this->setText(pBind->checkSet());
        emit this->keydown(ev);
    }
    else if(ret == kd_show) // 只show 不check 不发送按键事件
    {
        this->setText(pBind->showSet());
    }
    else if(ret == kd_show_emit)
    {
        this->setText(pBind->showSet());
        emit this->keydown(ev);
    }*/
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
