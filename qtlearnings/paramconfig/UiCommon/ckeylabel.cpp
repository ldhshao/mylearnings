#include "ckeylabel.h"

CKeyLabel::CKeyLabel(QWidget *parent, Qt::WindowFlags f):QLabel(parent,f)
{

}

void CKeyLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)
    emit clicked(this);
}
