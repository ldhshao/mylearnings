#include "mainwindow.h"

#include <QApplication>
#include "splitertest.h"
#include "pagecontainer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if 1
    MainWindow w;
    w.show();
#else
    //SpliterTest test;
    PageContainer test;
    test.show();
#endif

    return a.exec();
}
