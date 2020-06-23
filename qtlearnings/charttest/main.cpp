#include "mainwindow.h"
#include "mydata.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CMyData data;
    CCurveView curveView(std::bind(&CMyData::getDesc, &data, std::placeholders::_1));
    curveView.setTitle("Curve View");
    curveView.setAxisxTitle("X Axis");
    curveView.setAxisyTitle("Y Axis");
    curveView.setAxisxRange(0, 100);
    curveView.setAxisyRange(0, 4);
    QList<QPointF> lstPoint;
    lstPoint.append(QPointF(0, 2));
    lstPoint.append(QPointF(10, 3));
    lstPoint.append(QPointF(20, 2.5));
    lstPoint.append(QPointF(30, 2.8));
    lstPoint.append(QPointF(40, 2.5));
    lstPoint.append(QPointF(50, 3));
    lstPoint.append(QPointF(60, 2));
    lstPoint.append(QPointF(70, 1));
    curveView.addSeries(&lstPoint);
    //curveView.updateSeries(&lstPoint);
    curveView.show();

    return a.exec();
}
