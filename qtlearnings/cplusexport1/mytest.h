#ifndef MYTEST_H
#define MYTEST_H
#include "message.h"
#include <QTimer>

class MyTest : public QObject
{
    Q_OBJECT
public:
    void setMessage(Message *msg){
        m_msg = msg;
    }
    void test(){
        QTimer *timer = new QTimer();
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));
        timer->start(2000);
    }

public slots:
    void update(){
        if(m_msg){
            static int i = 0;
            m_msg->setAuthor(QString("%1").arg(i++));
        }
    }

private:
    Message *m_msg;
};

#endif // MYTEST_H
