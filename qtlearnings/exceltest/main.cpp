#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <ActiveQt/QAxBase>
#include <ActiveQt/QAxObject>
#include <ole2.h>
#include <QtXml>

#include "exceloperator.h"

void testExcel1()
{
    //QApplication a(argc, argv);
    HRESULT r = OleInitialize(0);
    if(r!=S_OK && r != S_FALSE)
    {
        return;
    }

    QAxObject excel("Excel.Application");
    excel.setProperty("Visible", false);
    QAxObject * workbooks = excel.querySubObject("WorkBooks");
    workbooks->dynamicCall("Open (const QString&)", QString("d:/cashway/test002.xlsx"));
    QAxObject * workbook = excel.querySubObject("ActiveWorkBook");
    QAxObject * worksheets = workbook->querySubObject("WorkSheets");
    int intCount = worksheets->property("Count").toInt();
    QAxObject * worksheet = workbook->querySubObject("Worksheets(int)", 1);
    QAxObject * usedrange = worksheet->querySubObject("UsedRange");
    QAxObject * rows = usedrange->querySubObject("Rows");
    QAxObject * columns = usedrange->querySubObject("Columns");
    int intRowStart = usedrange->property("Row").toInt();
    int intColStart = usedrange->property("Column").toInt();
    int intCols = columns->property("Count").toInt();
    int intRows = rows->property("Count").toInt();
    for (int i = intRowStart; i < intRowStart + intRows; i++)
    {
        for (int j = intColStart; j <= intColStart + intCols; j++)
        {
            QAxObject * range = worksheet->querySubObject("Cells(int,int)", i, j );
            if (5 == i && 1 == j) range->setProperty("Value", "SN:000051");
            qDebug() << i << j << range->property("Value");
        }
    }
    excel.setProperty("DisplayAlerts", 0);
    workbook->dynamicCall("SaveAs (const QString&)", QDir::toNativeSeparators("d:/xlsbyqt.xlsx"));
    qDebug() << "finish save";
    excel.setProperty("DisplayAlerts", 1);
    workbook->dynamicCall("Close (Boolean)", false);
    qDebug() << "finish close";
    excel.dynamicCall("Quit (void)");
    qDebug() << "finish quit";
}
void testExcel2()
{
    CExcelOperator excelOperator;
    QAxObject *pSheet = nullptr;
    excelOperator.open("d:/cashway/test002.xlsx");
    qDebug()<<"opened file";
    pSheet = excelOperator.getSheet(1);
    qDebug()<<"get the sheet";
    excelOperator.setCell(pSheet, 5, 2, "sn:000052");
    excelOperator.setCell(pSheet, "D5", "SN:00D5");
    qDebug()<<"set the cell 5 2";
    excelOperator.saveAs("d:/test002.xlsx");
    qDebug()<<"save as";
    excelOperator.close(false);
    qDebug()<<"close";
}
void readXml()
{
    QFile file("d:/coremodule.xml"); //相对路径、绝对路径、资源路径都行
    if(!file.open(QFile::ReadOnly))
        return;

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement(); //返回根节点
    qDebug()<<root.nodeName();
    QDomNode node=root.firstChild(); //获得第一个子节点
    while(!node.isNull())  //如果节点不空
    {
        if(node.isElement()) //如果节点是元素
        {
            QDomElement e=node.toElement(); //转换为元素，注意元素和节点是两个数据结构，其实差不多
            qDebug()<<e.attribute("type").toUInt()<<" "<<e.attribute("row").toUInt()<<" "<<e.attribute("col").toUInt(); //打印键值对，tagName和nodeName是一个东西
        }
        node=node.nextSibling(); //下一个兄弟节点,nextSiblingElement()是下一个兄弟元素，都差不多
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //testExcel1();
    //testExcel2();
    readXml();

    return a.exec();
}
