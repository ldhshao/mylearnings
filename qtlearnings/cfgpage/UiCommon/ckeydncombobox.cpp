#include "ckeydncombobox.h"
#include <QKeyEvent>
#include <QDebug>
//#include <QMessageBox>


CKeyDnComboBox::CKeyDnComboBox(QWidget *parent) :
    CStateComboBox(parent)
{
}

bool CKeyDnComboBox::setComboBoxText(const QString& strText)
{
    return false;
}

void CKeyDnComboBox::focusInEvent(QFocusEvent *ev)
{
    CStateComboBox::focusInEvent(ev);
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
        break;
    default:
        break;
    }

    setCurrentIndex(iCurr);
}
