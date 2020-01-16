#include <QGuiApplication>
#include <QQuickView>
#include "message.h"
#include <QQmlEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    //use a C++ class which derive from QObject, use its property
    Message msg;
    QQuickView view;
    view.engine()->rootContext()->setContextProperty("msg", &msg);

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:///main.qml"));
    view.show();

    return app.exec();
}
