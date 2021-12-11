#include "ckeylabel.h"
#include <QDebug>

CKeyLabel::CKeyLabel(QWidget *parent, Qt::WindowFlags f):QLabel(parent,f)
{

}

void CKeyLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)
    emit clicked(this);
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
