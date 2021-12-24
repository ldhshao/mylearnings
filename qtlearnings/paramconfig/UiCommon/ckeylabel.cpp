#include "ckeylabel.h"
#include <QKeyEvent>
#include <QDebug>

CKeyLabel::CKeyLabel(QWidget *parent, Qt::WindowFlags f):QLabel(parent,f)
{

}

void CKeyLabel::mousePressEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)
    emit clicked(this);
}
void CKeyLabel::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Return:
            emit clicked(this);
            return ;
    }
    QLabel::keyPressEvent(event);
}

void CKeyLabel::focusInEvent(QFocusEvent *ev)
{
    Q_UNUSED(ev)
    qDebug()<<"CKeyLabel "<<__FUNCTION__<<" "<<this;
    setStyleSheet("background-color: rgba(200, 60, 60, 100%)");
}

void CKeyLabel::focusOutEvent(QFocusEvent *ev)
{
    Q_UNUSED(ev)
    qDebug()<<"CKeyLabel "<<__FUNCTION__<<" "<<this;
    setStyleSheet("");
}
