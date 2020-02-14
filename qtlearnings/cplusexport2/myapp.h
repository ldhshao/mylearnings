#ifndef MYAPP_H
#define MYAPP_H
#include <QGuiApplication>
#include <QList>
#include <QThread>

class MyThread : public QThread
{
  Q_OBJECT
  Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
public:
  MyThread(QObject* parent);
  void setRunning(const bool &a){
      m_running = a;
      emit runningChanged();
  }
  bool running() const{
      return m_running;
  }
  void run() override {
      while(m_running){
          //do something
          msleep(2000);
      }
      quit();
  }
signals:
  void runningChanged();
private:
  bool m_running;
};
class MyApp : public QGuiApplication
{
  Q_OBJECT
public:
    MyApp(int &argc, char **argv);
    bool appendThread(MyThread* pThread);
signals:
    void closing();
public slots:
    void onLastWindowClosed();
private:
    QList<MyThread*> m_listhread;
};

#endif // MYAPP_H
