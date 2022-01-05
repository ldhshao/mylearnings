#include "ckeytablewidget.h"
#include <QTimer>
#include <QKeyEvent>
#include <QDebug>

CKeyTableWidget::CKeyTableWidget(QWidget *parent):QTableWidget(parent)
{
    //setSelectionBehavior(QAbstractItemView::SelectRows);
    usrKey = 0;
    timerInterval = 300;
    emitTimer = new QTimer(this);
    connect(emitTimer, SIGNAL(timeout()), this, SLOT(slot_emitTimer()));

}

void CKeyTableWidget::slot_emitTimer()
{
    emitTimer->stop();
    qDebug()<<__FUNCTION__<<usrKey;
    usrKey--;
    if (usrKey < 0) usrKey = 0;
    if (usrKey >= rowCount())
        usrKey = rowCount() - 1;
    if(hasFocus()) setCurrentCell(usrKey, 0);
    usrKey = 0;
}

void CKeyTableWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
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
            if(emitTimer->isActive()) emitTimer->stop();
            usrKey = usrKey * 10 + (event->key() - Qt::Key_0);
            emitTimer->start(timerInterval);
            return ;
    //case Qt::Key_Up:
    //case Qt::Key_Down:
    //        qDebug()<<currentIndex();
    //        QTableWidget::keyPressEvent(event);
    //        return ;
        default:
            QTableWidget::keyPressEvent(event);
    }
}
