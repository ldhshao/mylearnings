#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtDebug>
#include <QTimer>
#include "fruitmodel.h"

int main(int argc, char *argv[])
{
    {//test
        float i = 2.5;           // current file's number

        //QString status = QString("Processing file %1 of %2: %3")
        //          .arg(i).arg(total).arg(fileName);
        QString strNumber = QString("2.5 is %1").arg(i, 0, 'f', 3);
        qDebug()<<strNumber;
        i = 1.0 / 3;
        strNumber = QString("1/3 is %1").arg(i, 0, 'f', 3);
        qDebug()<<strNumber;
        i = 3.345;
        strNumber = QString("3.345 is %1").arg(i, 0, 'f', 2);
        qDebug()<<strNumber;
    }
    QGuiApplication app(argc, argv);

    FruitModel model;
    //model.addFruit(Fruit("apple", "3.3"));
    //model.addFruit(Fruit("pear", "2.5"));
    //model.addFruit(Fruit("orange", "2"));
    //model.addFruit(Fruit("banana", "1.5"));
    model.addFruit(Fruit("apple", 3.3));
    model.addFruit(Fruit("pear", 2.5));
    model.addFruit(Fruit("orange", 2));
    model.addFruit(Fruit("banana", 1.5));
    QTimer *timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), &model, SLOT(update()));
    timer->start(2000);
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("mymodel", &model);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
