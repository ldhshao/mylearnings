#ifndef MYTEST_H
#define MYTEST_H
#include <QTimer>
#include <QtDebug>
#include "fruitmodel.h"
#include "myapp.h"

class MyTest : public QObject
{
    Q_OBJECT
public:
    MyTest(QObject* parent=0);
    ~MyTest();
    void setFruit(Fruit *msg){
        m_msg = msg;
    }
    void setModel(FruitModel* pModel){
        m_model = pModel;
    }
    void startTest(){
        m_timer->start(2000);
    }
    void stopTest(){
        m_timer->stop();
    }

public slots:
    void update(){
        if(m_msg){
            static int i = 0;
            qDebug()<<"fruit "<<m_msg->name()<<" price "<<i;
            m_msg->setPrice(i++);
            if(m_model) m_model->rowReset(0);
        }
    }
    void onClosing(){
        stopTest();
    }

private:
    Fruit *m_msg;
    FruitModel *m_model;
    QTimer *m_timer;
};

class WorkerThreadTest : public MyThread
  {
    Q_OBJECT
public:
    WorkerThreadTest(QObject* parent= 0);
    void setFruit(Fruit *msg){
        m_msg = msg;
    }
    void setModel(FruitModel* pModel){
        m_model = pModel;
    }
    void run() override {
        int i = 0;
        while(running()){
            if(m_msg){
                qDebug()<<"fruit "<<m_msg->name()<<" price "<<i;
                m_msg->setPrice(i++);
                //if(m_model) m_model->rowReset(0);
            }
            msleep(2000);
        }
        quit();
    }
private:
    Fruit *m_msg;
    FruitModel *m_model;
  };

#endif // MYTEST_H
