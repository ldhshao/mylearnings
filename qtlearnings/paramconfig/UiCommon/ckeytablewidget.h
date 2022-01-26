#ifndef CKEYTABLEWIDGET_H
#define CKEYTABLEWIDGET_H

#include <QTableWidget>

class CKeyTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit CKeyTableWidget(QWidget *parent = nullptr);

protected slots:
    void slot_emitTimer();

protected:
    virtual void keyPressEvent(QKeyEvent *event);

    QTimer               *emitTimer;
    int                   timerInterval;
    int                   usrKey;
};

#endif // CKEYTABLEWIDGET_H
