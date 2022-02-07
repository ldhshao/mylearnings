#include "cmodparamquery.h"
#include "ui_cmodparamquery.h"
#include <QResizeEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QDir>
#include <list>
#include <fstream>
#include <QDebug>

#define WIDGET_BKCOLOR         "background-color:rgba(255,255,255,100%);"
CModParamQuery::CModParamQuery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CModParamQuery)
{
    setWindowFlags(Qt::FramelessWindowHint);
    bkLbl = new QLabel(this);
    bkLbl->setStyleSheet(WIDGET_BKCOLOR);
    ui->setupUi(this);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->dateEdit->setDate(QDate::currentDate());
    initTable();
    //ui->dateEdit->setFocus();
    qDebug()<<focusWidget();
}

CModParamQuery::~CModParamQuery()
{
    delete ui;
}

void CModParamQuery::initTable()
{
    QStringList headList;
    headList<<"时间"<<"操作记录";
        ui->tableWidget->setRowCount(0);
        ui->tableWidget->setColumnCount(headList.count());
        ui->tableWidget->setHorizontalHeaderLabels(headList);
    ui->tableWidget->resizeColumnsToContents();
    ui->label_lineno->setText("");
}

void CModParamQuery::resizeEvent(QResizeEvent *event)
{
    int m = 16, s = 8;
    int w = event->size().width(), h = event->size().height();
    int btnW = ui->pushButton_query->width();
    int btnH = ui->pushButton_query->height();

    bkLbl->resize(w, h);
    bkLbl->move(0, 0);
    ui->label_date->move(m,m);
    ui->dateEdit->move(m+ui->label_date->width(), m);
    ui->pushButton_query->move(w - m - btnW, m);
    ui->tableWidget->resize(w - 2*m, h - 2*(m+ s + btnH));
    autoAdjustTableColumns();
    ui->tableWidget->move(m, m+btnH+s);
    ui->label_lineno->move(m, h - m - btnH);
}

void CModParamQuery::autoAdjustTableColumns()
{
    ui->tableWidget->resizeColumnsToContents();
    list<int> widths;
    int colWidthTotal = 0;
    for (int i = 0; i < 2; i++) {
        colWidthTotal += ui->tableWidget->columnWidth(i);
        widths.push_back(ui->tableWidget->columnWidth(i));
    }
    qDebug()<<"test: "<<ui->tableWidget->width()<<colWidthTotal;
    if (ui->tableWidget->width() - colWidthTotal > 80){
        list<float> fList;
        fList.push_back(0.3);
        fList.push_back(0.7);
        int rest = ui->tableWidget->width() - colWidthTotal - 80;
        int i = 0;
        auto itFct = fList.begin();
        for (auto it = widths.begin(); it != widths.end(); it++, itFct++, i++){
            ui->tableWidget->setColumnWidth(i, *it + rest * (*itFct));
        }
    }
}

void CModParamQuery::keyPressEvent(QKeyEvent *event)
{
    if (this == focusWidget())
        autoSetFocus();

    QTableWidget* pTbl = dynamic_cast<QTableWidget*>(focusWidget());
    QDateEdit* pEdit = dynamic_cast<QDateEdit*>(focusWidget());
    CKeyButton* pBtn = dynamic_cast<CKeyButton*>(focusWidget());
    qDebug()<<focusWidget();
    qDebug()<<pBtn;
    switch (event->key()) {
        case Qt::Key_Up:
            if (nullptr == pTbl){
                if (0< ui->tableWidget->rowCount()){
                    ui->tableWidget->setFocus();
                    ui->tableWidget->setCurrentCell(ui->tableWidget->rowCount()-1, 1);
                }
            }else {
                if (0 == pTbl->rowCount() || 0 == pTbl->currentRow()){
                    ui->tableWidget->setCurrentCell(-1, -1);
                    ui->pushButton_query->setFocus();
                }
            }
            return ;
        case Qt::Key_Down:
            if (nullptr != pTbl){
                if (0 == pTbl->rowCount() || pTbl->rowCount()-1 == pTbl->currentRow()){
                    ui->tableWidget->setCurrentCell(-1, -1);
                    ui->pushButton_query->setFocus();
                }
            }else {
                if (0< ui->tableWidget->rowCount()){
                    ui->tableWidget->setFocus();
                    ui->tableWidget->setCurrentCell(0, 1);
                }
            }
            return ;
        case Qt::Key_Left:
        case Qt::Key_Right:
            if (nullptr != pEdit)
                ui->pushButton_query->setFocus();
            if (ui->pushButton_query == pBtn){
                ui->dateEdit->setFocus();
            }
            return ;
        case Qt::Key_Escape:
            setEnabled(false);
            return ;
    }
}
void CModParamQuery::on_pushButton_query_clicked()
{
    qDebug()<<__FUNCTION__;
    int count = 0;
    ui->tableWidget->clear();
    QStringList headList;
    headList<<"时间"<<"操作记录";
    ui->tableWidget->setRowCount(count);
    ui->tableWidget->setColumnCount(headList.count());
    ui->tableWidget->setHorizontalHeaderLabels(headList);
#if 0
    count = 1000;
    for (int i = 0; i < count; i++) {
        QTableWidgetItem *item = new QTableWidgetItem("2021/12/18 09:01:30");
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget->setItem(i, 0, item);

        item = new QTableWidgetItem("系统-系统配置-本机地址修改为10");
        //item = new QTableWidgetItem("系统-系统配置-本机地址由8修改为10");
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget->setItem(i, 1, item);
    }
#endif
    map<QString, QStringList> mapTimeRecords;
    int y = ui->dateEdit->date().year();
    int m = ui->dateEdit->date().month();
    int d = ui->dateEdit->date().day();
    readModifiedRecord(y, m, d, &mapTimeRecords);
    for (auto it = mapTimeRecords.begin(); it != mapTimeRecords.end(); it++) {
        count += it->second.count();
    }
    ui->tableWidget->setRowCount(count);
    int row = 0;
    for (auto it = mapTimeRecords.begin(); it != mapTimeRecords.end(); it++) {
        char strTemp[20] = {0};
        sprintf(strTemp, "%04d/%02d/%02d ", y, m, d);
        QString strTime(strTemp);
        strTime.append(it->first.mid(0,2) + ":" + it->first.mid(2,2) + ":" + it->first.mid(4,2));
        for (int i = 0; i < it->second.count(); i++) {
            QTableWidgetItem *item = new QTableWidgetItem(strTime);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableWidget->setItem(row, 0, item);

            item = new QTableWidgetItem(it->second[i]);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableWidget->setItem(row, 1, item);
            row++;
        }
    }

    ui->label_lineno->setText(QString("共").append(QString::number(ui->tableWidget->rowCount()) + "行"));
    autoAdjustTableColumns();
}

void CModParamQuery::changeEvent(QEvent *event)
{
    switch (event->type()) {
    case QEvent::EnabledChange:
        if (isEnabled() && 0 < ui->tableWidget->rowCount()) ui->tableWidget->setCurrentCell(0, 1);
        break;
    default:
        QWidget::changeEvent(event);
    }
}

void CModParamQuery::autoSetFocus()
{
    if (0 < ui->tableWidget->rowCount()){
        ui->tableWidget->setFocus();
    }else{
        ui->dateEdit->setFocus();
    }

}

void CModParamQuery::readModifiedRecord(int y, int m, int d, map<QString, QStringList> *mapTimeRecords)
{
    QString strParam("/opt/data/paramconfig");
    char strTemp[20] = {0};
    sprintf(strTemp, "%04d%02d%02d", y, m, d);
    QString strDate(strTemp);
    QDir temp(strParam);
    QFileInfoList dirList = temp.entryInfoList(QDir::Files);
    for (auto it = dirList.begin(); it != dirList.end(); it++) {
        if ("." == it->fileName() || ".." == it->fileName()) continue;
        qDebug()<<it->fileName();
        if (strDate == it->fileName().left(strDate.length()) && "modified.dat" == it->fileName().right(12)){
            QString time = it->fileName().mid(8,6);
            QStringList records;
            ifstream inFile;
            QString filePath = strParam + "/" + it->fileName();
            inFile.open(filePath.toStdString(), ios_base::in);
            qDebug()<<filePath;
            char buf[512];
            while(inFile.getline(buf, sizeof(buf))){
                records.push_back(buf);
                if (inFile.eof()) break;
            }
            mapTimeRecords->insert(pair<QString, QStringList>(time, records));

            inFile.close();
        }
    }
}
