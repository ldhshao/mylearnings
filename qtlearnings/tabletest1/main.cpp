#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtDebug>
#include "fruitmodel.h"

int main(int argc, char *argv[])
{
    {//test
        float i = 2.5;           // current file's number
        QString total = "123";       // number of files to process
        QString fileName = "selfoilng.cpp";    // current file's name

        QString status = QString("Processing file %1 of %2: %3")
                  .arg(i).arg(total).arg(fileName);
        qDebug()<<status;
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
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("mymodel", &model);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
