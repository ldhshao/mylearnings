#include <server.h>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server *piServer = new Server;
    return a.exec();//开始主线程事件循环
}
