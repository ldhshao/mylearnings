#include "ckeycombobox.h"
#include <QKeyEvent>

CKeyComboBox::CKeyComboBox(QWidget *parent): QComboBox(parent)
{

}

void CKeyComboBox::keyPressEvent(QKeyEvent *event)
{
    int iCurr = currentIndex();
    int iCnt = count();
    switch (event->key()) {
    case Qt::Key_Up:
    case Qt::Key_Down:
        event->setAccepted(false);
        break;
    case Qt::Key_Left:
        iCurr = ((iCurr - 1 + iCnt) % iCnt);
        setCurrentIndex(iCurr);
        break;
    case Qt::Key_Right:
        iCurr = ((iCurr + 1) % iCnt);
        setCurrentIndex(iCurr);
        break;
    default:
        QComboBox::keyPressEvent(event);
        break;
    }

}
