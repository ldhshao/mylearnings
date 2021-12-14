#include "ckeydncombobox.h"
#include <QKeyEvent>
#include <QDebug>
//#include <QMessageBox>


CKeyDnComboBox::CKeyDnComboBox(QWidget *parent) :
    CStateComboBox(parent), pVal(nullptr)
{
    updating = false;
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_currentIndexChanged(int)));
}

void CKeyDnComboBox::updateText()
{
    updating = true;
    if (nullptr != pVal){
        setCurrentIndex(static_cast<int>(*pVal));
    }
    updating = false;
}

void CKeyDnComboBox::focusInEvent(QFocusEvent *ev)
{
    //setStyleSheet("background-color:rgba(200,60,60,100%)");
    QComboBox::focusInEvent(ev);
}
void CKeyDnComboBox::focusOutEvent(QFocusEvent *ev)
{
    //setStyleSheet("");
    QComboBox::focusOutEvent(ev);
}

void CKeyDnComboBox::keyPressEvent(QKeyEvent *ev)
{
    // 未绑定数据直接返回按键事件
    qDebug()<<"combobox key press";
    int iCurr = currentIndex();
    int iCnt = count();
    int keyVal = ev->key();
    switch (keyVal) {
    case Qt::Key_Left:
        iCurr = (iCurr + iCnt - 1) % iCnt;
        break;
    case Qt::Key_Right:
        iCurr = (iCurr + iCnt + 1) % iCnt;
        break;
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_Return:
    case Qt::Key_Escape:
        ev->setAccepted(false);
        break;
    default:
        break;
    }

    setCurrentIndex(iCurr);
}

void CKeyDnComboBox::slot_currentIndexChanged(int index)
{
    if (nullptr != pVal){
        if (index != *pVal){
            *pVal = index;
            if (!updating)
                emit sig_valueChanged(pVal, *pVal);
        }
    }
}
