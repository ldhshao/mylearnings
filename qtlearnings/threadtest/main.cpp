#include "myapp.h"
#include "worker.h"
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    MyApp app(argc, argv);
    //CWorkerThread *worker = new CWorkerThread();
    //app.appendThread(worker);
    //worker->startThread();

    Controller c1;
    c1.dealstring("liu donghao");
    c1.dealstring("liu jiahe");
    c1.dealstring("liu lili");
    c1.dealstring("li jin");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
