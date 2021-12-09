#include "ckeybutton.h"
#include <QKeyEvent>

CKeyButton::CKeyButton(QWidget *parent)
    :QPushButton(parent)
{

}
void CKeyButton::keyPressEvent(QKeyEvent *event)
{
    QWidget *focus = focusWidget();
    switch (event->key()) {
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_Return:
    //case Qt::Key_Escape:
    //case Qt::Key_0:
    //case Qt::Key_1:
    //case Qt::Key_2:
    //case Qt::Key_3:
    //case Qt::Key_4:
    //case Qt::Key_5:
    //case Qt::Key_6:
    //case Qt::Key_7:
    //case Qt::Key_8:
    //case Qt::Key_9:
        event->setAccepted(false);
        break;
    default:
        break;
    }
}
