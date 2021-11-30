#include "cdevpointedit.h"
#include "cdevposmgr.h"
#include "cdevposctl1.h"
#include <QKeyEvent>

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
    showText();
}

void CDevPointEdit::showText()
{
    if (nullptr != pVal){
        uint32_t val = *pVal + (*(pVal + 1)<<16);
        setText(CDevPosMgr::instance()->makeStrDevPoint(val));
    }
}

void CDevPointEdit::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Down:
    case Qt::Key_Up:
        e->setAccepted(false);
        break;
    default:
        QLineEdit::keyPressEvent(e);
    }
}
void CDevPointEdit::mousePressEvent(QMouseEvent *e)
{
    CDevPosCtl1::instance()->setAttachEdit(this);
    CDevPosCtl1::instance()->show();
}
