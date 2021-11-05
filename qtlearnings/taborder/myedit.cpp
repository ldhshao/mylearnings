#include "myedit.h"
#include <QKeyEvent>

MyEdit::MyEdit(QWidget*parent)
    :QLineEdit(parent)
{

}

void MyEdit::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Down:
    case Qt::Key_Up:
        event->setAccepted(false);
        break;
    default:
        QLineEdit::keyPressEvent(event);
    }
}
