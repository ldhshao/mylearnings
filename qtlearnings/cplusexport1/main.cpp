#include <QGuiApplication>
#include <QQuickView>
#include "message.h"
#include "mytest.h"
#include "myapp.h"
#include <QQmlEngine>
#include <QQmlContext>
#include <QTimer>

int main(int argc, char *argv[])
{
    MyApp app(argc, argv);
    //use a C++ class which derive from QObject, use its property
    Message msg;
    QQuickView view;
    view.engine()->rootContext()->setContextProperty("msg", &msg);
#if 0
    MyTest mytst;
    mytst.setMessage(&msg);
    mytst.test();
#endif
#if 0
    WorkerThreadTest mytst;
    //QObject::connect(&mytst, &WorkerThreadTest::finished, &mytst, &QObject::deleteLater);
    app.appendThread(&mytst);
    mytst.setMessage(&msg);
    mytst.setRunning(true);
    mytst.start();
#endif
#if 1
    WorkerThreadTest *pTst = new WorkerThreadTest();
    app.appendThread(pTst);
    pTst->setMessage(&msg);
    pTst->setRunning(true);
    pTst->start();
#endif

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:///main.qml"));
    view.show();

    return app.exec();
}
