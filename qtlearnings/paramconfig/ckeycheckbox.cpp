#include "ckeycheckbox.h"
#include <QKeyEvent>

CKeyCheckBox::CKeyCheckBox(QWidget *parent): QCheckBox(parent)
{

}

CKeyCheckBox::CKeyCheckBox(const QString &text, QWidget *parent): QCheckBox(text, parent)
{

}

void CKeyCheckBox::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
    case Qt::Key_Down:
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
        event->setAccepted(false);
        break;
    case Qt::Key_Return:
        setChecked(!isChecked());
        break;
    default:
        QCheckBox::keyPressEvent(event);
        break;
    }

}
