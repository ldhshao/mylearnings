#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    int iPos0 =  QString("0.1(1-247)").indexOf(QRegExp("(\\d+-\\d+)"), 0);
    int iPos1 =  QString("0.1(1-247)").indexOf(QRegExp("\\d+-\\d+"), 0);
    int iPos2 =  QString("0.1(1-247)").indexOf(QRegExp("\("), 0);
    int iPos3 = QString("0.1(1-247)").indexOf("(");
    MainWindow w;
    w.show();
    return a.exec();
}
