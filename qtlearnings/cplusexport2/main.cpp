#include <QGuiApplication>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include "myapp.h"
#include "fruitmodel.h"
#include "mytest.h"

int main(int argc, char *argv[])
{
    MyApp app(argc, argv);
    QQuickView view;

    Fruit *pApple = new Fruit("apple", 2.5);
    FruitModel *pModel = new FruitModel(&app);
    MyTest     *pTest = new MyTest(&app);
    QObject::connect(&app, &MyApp::closing, pTest, &MyTest::onClosing);
    pTest->setFruit(pApple);
    pTest->setModel(pModel);
    pModel->addFruit(pApple);
    pModel->addFruit(new Fruit("pear", 2.3));
    pModel->addFruit(new Fruit("banana", 2.1));
    pModel->addFruit(new Fruit("orange", 3.3));

    view.engine()->rootContext()->setContextProperty("mymodel", pModel);

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:///main.qml"));
    view.show();

    pTest->startTest();
    return app.exec();
}
