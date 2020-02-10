#include <QGuiApplication>
#include <QQuickView>
#include "message.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQuickView view;
    //register C++ class as QML type, use it as a QML conponet
    qmlRegisterType<Message>("Message", 1, 0, "Message");

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:///main.qml"));
    view.show();

    return app.exec();
}
