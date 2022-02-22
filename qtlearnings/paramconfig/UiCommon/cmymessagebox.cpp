#include "cmymessagebox.h"


CMyMessageBox::CMyMessageBox(QWidget *parent)
    :QMessageBox(parent)
{
    //
}

CMyMessageBox::CMyMessageBox(Icon icon, const QString &title, const QString &text,
                StandardButtons buttons, QWidget *parent,
                Qt::WindowFlags flags)
    :QMessageBox(icon, title, text, buttons, parent, flags)
{
    //
}

void CMyMessageBox::keyPressEvent(QKeyEvent *event)
{

    QMessageBox::keyPressEvent(event);
}
