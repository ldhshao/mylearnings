#include "ckeybutton.h"
#include <QKeyEvent>
#include <QDebug>

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
    case Qt::Key_Return:
        emit clicked();
        break;
    default:
        break;
    }
}

void CKeyButton::focusInEvent(QFocusEvent *event)
{
    QPushButton::focusInEvent(event);
}
void CKeyButton::focusOutEvent(QFocusEvent *event)
{
    QPushButton::focusOutEvent(event);
}

//////////////////////CKeyButtonEx//////////////////////
CKeyButtonEx::CKeyButtonEx(QWidget *parent)
    :CKeyButton(parent)
{

}
void CKeyButtonEx::focusInEvent(QFocusEvent *event)
{
    m_preStyle = styleSheet();
    QString strStyle = removeStyle(styleSheet(), "background-color");
    strStyle.append("background-color: rgba(200, 60, 60, 100%)");
    setStyleSheet(strStyle);
    qDebug()<<styleSheet();
}
void CKeyButtonEx::focusOutEvent(QFocusEvent *event)
{
    setStyleSheet(m_preStyle);
    qDebug()<<styleSheet();
}

QString CKeyButtonEx::removeStyle(const QString& strStyle, QString strAttr)
{
    QStringList dstList;
    QStringList styleList = strStyle.split(';');
    for (int i = 0; i < styleList.count(); i++) {
        if (-1 == styleList[i].indexOf(strAttr))
            dstList.append(styleList[i]);
    }
    return dstList.join(';');
}
