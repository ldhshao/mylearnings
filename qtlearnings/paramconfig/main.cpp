#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.setStyleSheet("MainWindow{ border-image: url(:/images/background2.png);}");
    w.showMaximized();
    return a.exec();
}
