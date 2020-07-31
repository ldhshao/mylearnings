#include "exceloperator.h"
#include <ole2.h>
#include <QDebug>
#include <QDir>
//#include <objbase.h>

CExcelOperator::CExcelOperator(QObject *parent) : QObject(parent)
  , m_pExcel(nullptr)
  , m_pWorksheets(nullptr)
  , m_pWorkbook(nullptr)
{

}

CExcelOperator::~CExcelOperator()
{
    close();
}

bool CExcelOperator::open(const QString& path)
{
    m_strPath = path;
    //check path exist
    QAxObject *pWorkbooks = nullptr;
    ///////////////////////OLE 初始化，此处在GUI下不需要，原因未知/////////////////////////
    if(S_OK != OleInitialize(nullptr))
    {
        qDebug()<<"Failed In Initialize Ole";
        //return false;
    }
    ///////////////////////////////////////////////

    m_pExcel = new(std::nothrow) QAxObject();
    if (nullptr == m_pExcel) {
        qCritical()<<"创建Excel对象失败...";
        return false;
    }
    try {
        m_pExcel->setControl("Excel.Application");
        m_pExcel->dynamicCall("SetVisible(bool)", false); //true 表示操作文件时可见，false表示为不可见
        m_pExcel->setProperty("DisplayAlerts", false);
        pWorkbooks = m_pExcel->querySubObject("WorkBooks");
        pWorkbooks->dynamicCall("Open (const QString&)", path);
        m_pWorkbook = m_pExcel->querySubObject("ActiveWorkBook");
        m_pWorksheets = m_pWorkbook->querySubObject("WorkSheets");
        qDebug()<<"excel path: "<<m_strPath;
    } catch (...) {
        qCritical()<<"打开文件失败...";
        return false;
    }

    return true;
}

bool CExcelOperator::close(bool bSave)
{
    qDebug()<<"excel close...";
    if (m_pExcel)
    {
        qDebug()<<"closing...";
        if(bSave)
            m_pWorkbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(m_strPath));
        m_pWorkbook->dynamicCall("Close()");
        m_pExcel->dynamicCall("Quit()");
        OleUninitialize();
        delete m_pExcel;
        m_pExcel = nullptr;
    }
    return true;
}

bool CExcelOperator::saveAs(const QString& path)
{
    qDebug()<<"excel save as "<< path;
    if (m_pExcel)
    {
        qDebug()<<"saving...";
        m_pWorkbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(path));
    }
    return true;
}

int CExcelOperator::getSheetsCount()
{
    int count =  0;
    count = m_pWorksheets->property("Count").toInt();
    return count;
}


QAxObject* CExcelOperator::addSheet(QString name)
{
    QAxObject *pWorkSheet = nullptr;
    try {
        int count = m_pWorksheets->property("Count").toInt();  //获取工作表数目
        QAxObject *pLastSheet = m_pWorksheets->querySubObject("Item(int)", count);
        pWorkSheet = m_pWorksheets->querySubObject("Add(QVariant)", pLastSheet->asVariant());
        pLastSheet->dynamicCall("Move(QVariant)", pWorkSheet->asVariant());
        pWorkSheet->setProperty("Name", name);  //设置工作表名称
    } catch (...) {
        qCritical()<<"创建sheet失败...";
    }
    return pWorkSheet;
}

bool CExcelOperator::delSheet(QString name)
{
    try {
        QAxObject *pFirstSheet = m_pWorksheets->querySubObject("Item(QString)", name);
        pFirstSheet->dynamicCall("delete");
    } catch (...) {
        qCritical()<<"删除sheet失败...";
        return false;
    }
    return true;
}

bool CExcelOperator::delSheet(int index)
{
    try {
        QAxObject *pFirstSheet = m_pWorksheets->querySubObject("Item(int)", index);
        pFirstSheet->dynamicCall("delete");
    } catch (...) {
        qCritical()<<"删除sheet失败...";
        return false;
    }
    return true;
}

QAxObject* CExcelOperator::getSheet(QString name)
{
    QAxObject* pWorkSheet = nullptr;
    try {
        pWorkSheet = m_pWorksheets->querySubObject("Item(QString)", name);
    } catch (...) {
        qCritical()<<"获取sheet失败...";
    }
    return pWorkSheet;
}

QAxObject* CExcelOperator::getSheet(int index)
{
    QAxObject* pWorkSheet = nullptr;
    try {
        pWorkSheet = m_pWorksheets->querySubObject("Item(int)", index);
    } catch (...) {
        qCritical()<<"获取sheet失败...";
    }
    return pWorkSheet;
}

QAxObject* CExcelOperator::getRows(QAxObject* pSheet)
{
    QAxObject* pRows = nullptr;
    try {
        pRows = pSheet->querySubObject("Rows");
    } catch (...) {
        qCritical()<<"获取行失败...";
    }
    return pRows;
}

int CExcelOperator::getRowsCount(QAxObject* pSheet)
{
    int rows = 0;
    try {
        QAxObject* pRows = getRows(pSheet);
        rows = pRows->property("Count").toInt();
    } catch (...) {
        qCritical()<<"获取行数失败...";
    }
    return rows;
}

QAxObject* CExcelOperator::getColumns(QAxObject* pSheet)
{
    QAxObject* pColumns = nullptr;
    try {
        pColumns = pSheet->querySubObject("Columns");
    } catch (...) {
        qCritical()<<"获取列失败...";
    }
    return pColumns;
}

int CExcelOperator::getColumnsCount(QAxObject* pSheet)
{
    int columns = 0;
    try {
        QAxObject* pColumns = getColumns(pSheet);
        columns = pColumns->property("Count").toInt();
    } catch (...) {
        qCritical()<<"获取列数失败...";
    }
    return columns;
}

QString CExcelOperator::getCell(QAxObject* pSheet, int row, int column)
{
    QString strCell = "";
    try {
        QAxObject* pCell = pSheet->querySubObject("Cells(int, int)", row, column);
        strCell = pCell->property("Value").toString();
    } catch (...) {
        qCritical()<<"获取单元格信息失败...";
    }

    return strCell;
}

QString CExcelOperator::getCell(QAxObject* pSheet, QString number)
{
    QString strCell = "";
    try {
        QAxObject* pCell = pSheet->querySubObject("Range(QString)", number);
        strCell = pCell->property("Value").toString();
    } catch (...) {
        qCritical()<<"获取单元格信息失败...";
    }

    return strCell;
}

bool CExcelOperator::setCell(QAxObject* pSheet, int row, int column, QString value)
{
    try {
        QAxObject* pCell = pSheet->querySubObject("Cells(int, int)", row, column);
        pCell->setProperty("Value", value);
    } catch (...) {
        qCritical()<<"写入单元格信息失败...";
        return false;
    }
    return true;
}

bool CExcelOperator::setCell(QAxObject* pSheet, QString number, QString value)
{
    try {
        QAxObject* pCell = pSheet->querySubObject("Range(QString)", number);
        pCell->setProperty("Value", value);
    } catch (...) {
        qCritical()<<"写入单元格信息失败...";
        return false;
    }
    return true;
}

