#include "cdevpointedit.h"
#include "cdevposmgr.h"
#include "cdevposctl1.h"
#include "clineselector.h"
#include <QKeyEvent>
#include <QDebug>

CDevPointEdit::CDevPointEdit(QWidget *parent):QLineEdit(parent)
{
    pVal = nullptr;
}

CDevPointEdit::CDevPointEdit(const QString &text, QWidget *parent):QLineEdit(text, parent)
{
    pVal = nullptr;
    //
}

void CDevPointEdit::setValuePtr(uint16_t* val)
{
    pVal = val;
    showText();
}

void CDevPointEdit::setValue(uint32_t val)
{
    *pVal = val;
    *(pVal + 1) = (val >> 16);
    qDebug()<<"write addr "<<pVal<<" val "<<val;
    showText();
    emit sig_valueChanged(pVal, val);
    if (val < 0xFFFF)
        emit sig_valueChanged(pVal+1, *(pVal + 1));
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
        CDevPosMgr::instance()->setDevPoint(val, true);
        setText(CDevPosMgr::instance()->makeStrDevPoint(val));
    }
}

void CDevPointEdit::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Down:
    case Qt::Key_Up:
    case Qt::Key_Escape:
        e->setAccepted(false);
        break;
    default:
        QLineEdit::keyPressEvent(e);
    }
}
void CDevPointEdit::mouseReleaseEvent(QMouseEvent *e)
{
    //CDevPosCtl1::instance()->setAttachEdit(this);
    //CDevPosCtl1::instance()->show();
    QPoint pt = mapToGlobal(QPoint(0,0));
    int w = width(), h = height();
    CLineSelector::instance()->adjustPosition(pt.rx(), pt.ry(), w, h);
    uint32_t val = *pVal + (*(pVal + 1)<<16);
    qDebug()<<"read addr "<<pVal<<" val "<<val;
    CLineSelector::instance()->show();
    clearFocus();

    CPortSelector::instance()->setAttachEdit(this);
    if (CDevPosMgr::instance()->isDevPointValid(val)){
        int l = get_line_from_dev_point(val) - 1;
        int m = get_machine_from_dev_point(val);
        int p = get_port_from_dev_point(val);
        CLineSelector::instance()->selectButtonByIndex(l);
        CMachineSelector::instance()->selectButtonByIndex(m);
        CPortSelector::instance()->activateWindow();
    }else{
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
    //setStyleSheet("");
    QLineEdit::focusOutEvent(event);
}
