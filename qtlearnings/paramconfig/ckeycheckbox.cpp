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
