#include "ckeydateedit.h"
#include <QKeyEvent>
#include <QLineEdit>
#include <QDebug>

CKeyDateEdit::CKeyDateEdit(QWidget* parent): QDateEdit(parent)
{

}

void CKeyDateEdit::keyPressEvent(QKeyEvent *event)
{
    QWidget *focus = focusWidget();
    qDebug()<<lineEdit()->cursorPosition();
    int pos = lineEdit()->cursorPosition();
    switch (event->key()) {
    case Qt::Key_Left:
        if (0 == pos)
            event->setAccepted(false);
        else
            QDateEdit::keyPressEvent(event);
        break;
    case Qt::Key_Right:
        if (0 < pos && pos == lineEdit()->text().length())
            event->setAccepted(false);
        else
            QDateEdit::keyPressEvent(event);
        break;
    default:
        QDateEdit::keyPressEvent(event);
        break;
    }
}
