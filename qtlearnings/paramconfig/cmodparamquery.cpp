#include "cmodparamquery.h"
#include "ui_cmodparamquery.h"
#include <QResizeEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

CModParamQuery::CModParamQuery(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CModParamQuery)
{
    ui->setupUi(this);
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
    //autoSetSize();
}

void CModParamQuery::autoSetSize()
{
    int m = 16, s = 8;
    int sbw = 40;
    if (ui->tableWidget->rowCount() >= 1000) sbw += 40;
    QRect deskRect = QApplication::desktop()->availableGeometry();
    int btnH = ui->pushButton_query->height();
    int tblHMax = deskRect.height() - 2*(m + btnH + s), tblH = (ui->tableWidget->rowCount()+1) * ui->tableWidget->rowHeight(0)+sbw;
    int tblW = ui->tableWidget->columnWidth(0) + ui->tableWidget->columnWidth(1)+ sbw;
    if (tblH > tblHMax) tblH = tblHMax;
    else if (tblH < height()) tblH = height();
    qDebug()<<ui->tableWidget->columnWidth(0)<<ui->tableWidget->columnWidth(1);
    qDebug()<<"width height"<<width()<<height();
    ui->tableWidget->resize(tblW, tblH);
    resize(tblW + 2*m, tblH+2*(m+s+btnH));
    qDebug()<<"width height"<<width()<<height();
}

void CModParamQuery::resizeEvent(QResizeEvent *event)
{
    int m = 16, s = 8;
    int w = event->size().width(), h = event->size().height();
    int btnW = ui->pushButton_query->width();
    int btnH = ui->pushButton_query->height();

    ui->label_date->move(m,m);
    ui->dateEdit->move(m+ui->label_date->width(), m);
    ui->pushButton_query->move(w - m - btnW, m);
    ui->tableWidget->resize(w - 2*m, h - 2*(m+ s + btnH));
    ui->tableWidget->move(m, m+btnH+s);
    ui->label_lineno->move(m, h - m - btnH);
    ui->pushButton_close->move(w - m - btnW, h - m - btnH);
}

void CModParamQuery::keyPressEvent(QKeyEvent *event)
{
    QTableWidget* pTbl = dynamic_cast<QTableWidget*>(focusWidget());
    QDateEdit* pEdit = dynamic_cast<QDateEdit*>(focusWidget());
    CKeyButton* pBtn = dynamic_cast<CKeyButton*>(focusWidget());
    qDebug()<<focusWidget();
    qDebug()<<pBtn;
    switch (event->key()) {
        case Qt::Key_Up:
            if (ui->pushButton_query == pBtn){
                ui->pushButton_close->setFocus();
            }else if(ui->pushButton_close == pBtn){
                if (0< ui->tableWidget->rowCount()){
                    ui->tableWidget->setFocus();
                    ui->tableWidget->setCurrentCell(ui->tableWidget->rowCount()-1, 1);
                }
                else
                    focusNextPrevChild(false);
            }else if (nullptr != pTbl){
                if (0 == pTbl->rowCount() || 0 == pTbl->currentRow()){
                    ui->tableWidget->setCurrentCell(-1, -1);
                    ui->pushButton_query->setFocus();
                }
            }
            return ;
        case Qt::Key_Down:
            if (ui->pushButton_query == pBtn){
                if (0< ui->tableWidget->rowCount()){
                    ui->tableWidget->setFocus();
                    ui->tableWidget->setCurrentCell(0, 1);
                }
                else
                    focusNextPrevChild(true);
            }else if(ui->pushButton_close == pBtn){
                ui->pushButton_query->setFocus();
            }else if (nullptr != pTbl){
                if (0 == pTbl->rowCount() || pTbl->rowCount()-1 == pTbl->currentRow()){
                    ui->tableWidget->setCurrentCell(-1, -1);
                    ui->pushButton_close->setFocus();
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
    }
}
void CModParamQuery::on_pushButton_query_clicked()
{
    qDebug()<<__FUNCTION__;
    int count = 1000;
    ui->tableWidget->clear();
    QStringList headList;
    headList<<"时间"<<"操作记录";
    ui->tableWidget->setRowCount(count);
    ui->tableWidget->setColumnCount(headList.count());
    ui->tableWidget->setHorizontalHeaderLabels(headList);
    for (int i = 0; i < count; i++) {
        QTableWidgetItem *item = new QTableWidgetItem("2021/12/18 09:01:30");
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget->setItem(i, 0, item);

        item = new QTableWidgetItem("系统-系统配置-本机地址修改为10");
        //item = new QTableWidgetItem("系统-系统配置-本机地址由8修改为10");
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget->setItem(i, 1, item);
    }
    ui->tableWidget->resizeColumnsToContents();
    ui->label_lineno->setText(QString::asprintf("共%d行", ui->tableWidget->rowCount()));
    autoSetSize();
}

void CModParamQuery::on_pushButton_close_clicked()
{
    accept();
}
