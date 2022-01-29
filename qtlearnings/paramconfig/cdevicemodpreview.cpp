#include "cdevicemodpreview.h"
#include "ui_cdevicemodpreview.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QResizeEvent>
#include <math.h>
#include <QDebug>

CDeviceModPreview::CDeviceModPreview(list<struct SDeviceInfoItem>* pItemList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDeviceModPreview)
{
    this->pItemList = pItemList;
    ui->setupUi(this);
    initTable();
}

CDeviceModPreview::~CDeviceModPreview()
{
    delete ui;
}

void CDeviceModPreview::initTable()
{
    QStringList headList;
    headList<<"名称"<<"数值";
    if (nullptr != pItemList && !pItemList->empty()){
        ui->tableWidget->setRowCount(pItemList->size());
        ui->tableWidget->setColumnCount(headList.count());
        ui->tableWidget->setHorizontalHeaderLabels(headList);

        int row = 0;
        for (auto it = pItemList->begin(); it != pItemList->end(); it++) {
            QTableWidgetItem *item = new QTableWidgetItem(it->name);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableWidget->setItem(row, 0, item);

            item = new QTableWidgetItem(it->currVal);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableWidget->setItem(row, 1, item);

            row++;
        }
    }
    ui->tableWidget->resizeColumnsToContents();
    ui->label_lineno->setText("共" + QString::number(ui->tableWidget->rowCount()) + "行");
    autoSetSize();
}

void CDeviceModPreview::autoSetSize()
{
    int m = 16, s = 8;
    int sbw = 40;
    QRect deskRect = QApplication::desktop()->availableGeometry();
    int btnH = ui->pushButton_cancel->height();
    int tblHMax = deskRect.height() - 2*m - btnH - s, tblH = (ui->tableWidget->rowCount()+1) * ui->tableWidget->rowHeight(0)+sbw;
    int tblW = ui->tableWidget->columnWidth(0) + ui->tableWidget->columnWidth(1) + ui->tableWidget->columnWidth(2) + ui->tableWidget->columnWidth(3) + sbw;
    if (tblH > tblHMax) tblH = tblHMax;
    else if (tblH < height()) tblH = height();
    ui->tableWidget->resize(tblW, tblH);
    resize(tblW + 2*m, tblH+2*m+s+btnH);
}

QWidget* CDeviceModPreview::getCloseWidget(int x, int y)
{
    int iMin = width() * width() + height() * height();
    QWidget* pFind = nullptr;
    list<QPushButton*> btnList;

    btnList.push_back(ui->pushButton_cancel);
    btnList.push_back(ui->pushButton_ok);
    qDebug()<<"(x y)"<<x<<y;
    for (auto it = btnList.begin(); it != btnList.end(); it++) {
        if (!(*it)->isEnabled()) continue;
        int x0 = (*it)->pos().x() + (*it)->width() / 2;
        int y0 = (*it)->pos().y() + (*it)->height() / 2;
        int dist = (x0 - x) * (x0 - x) + (y0 - y) * (y0 - y);
        qDebug()<<"(x y dist)"<<x0<<y0<<dist;
        if (dist < iMin){
            iMin = dist;
            pFind = (*it);
        }
    }
    qDebug()<<pFind;
    return pFind;
}

int CDeviceModPreview::getCloseCell(int x, int r)
{
    int col = -1;
    int iMin = width();
    for (int c=0; c<ui->tableWidget->columnCount(); c++) {
        QRect rct = ui->tableWidget->visualRect(ui->tableWidget->model()->index(r, c));
        qDebug()<<rct;
        int x0 = ui->tableWidget->pos().x() + rct.left() + rct.width() / 2;
        int dist = fabs(x0 - x);
        if (dist < iMin){
            iMin = dist;
            col = c;
        }
    }

    return col;
}

void CDeviceModPreview::resizeEvent(QResizeEvent *event)
{
    int m = 16, s = 8;
    int w = event->size().width(), h = event->size().height();
    int btnW = ui->pushButton_ok->width();
    int btnH = ui->pushButton_ok->height();

    ui->tableWidget->resize(w - 2*m, h - 2*m- s - btnH);
    ui->tableWidget->move(m, m);
    ui->label_lineno->move(m, h - m - btnH);
    ui->pushButton_cancel->move(w - m - 2*btnW - s, h - m - btnH);
    ui->pushButton_ok->move(w - m - btnW, h - m - btnH);
}

void CDeviceModPreview::keyPressEvent(QKeyEvent *event)
{
    QTableWidget* pTbl = nullptr;
    CKeyButton* pBtn = dynamic_cast<CKeyButton*>(focusWidget());
    qDebug()<<focusWidget();
    if (nullptr == pBtn){
        pTbl = ui->tableWidget;
    }
    qDebug()<<pBtn;
    qDebug()<<pTbl;
    switch (event->key()) {
        case Qt::Key_Up:
            if (nullptr != pTbl){
                if (0 == pTbl->currentRow()){
                    QRect rct = pTbl->visualRect(pTbl->currentIndex());
                    int x = pTbl->pos().x();
                    pTbl->setCurrentCell(-1, -1);
                    getCloseWidget(x + rct.left() + rct.width()/2, rct.top() + rct.height()/2)->setFocus();
                }
            }else {
                int row = ui->tableWidget->rowCount() - 1;
                int x = pBtn->pos().x() + pBtn->width() / 2;
                int col = getCloseCell(x, row);
                ui->tableWidget->setCurrentCell(row, col);
                ui->tableWidget->setFocus();
            }
            return ;
        case Qt::Key_Down:
            if (nullptr != pTbl){
                if (pTbl->rowCount()-1 == pTbl->currentRow()){
                    QRect rct = pTbl->visualRect(pTbl->currentIndex());
                    int x = pTbl->pos().x();
                    pTbl->setCurrentCell(-1, -1);
                    getCloseWidget(x + rct.left() + rct.width()/2, rct.top() + rct.height()/2)->setFocus();
                }
            }else {
                int row = 0;
                int x = pBtn->pos().x() + pBtn->width() / 2;
                int col = getCloseCell(x, row);
                ui->tableWidget->setCurrentCell(row, col);
                ui->tableWidget->setFocus();
            }
            return ;
        case Qt::Key_Left:
        case Qt::Key_Right:
            if (nullptr != pBtn){
                if (pBtn == ui->pushButton_cancel)
                    ui->pushButton_ok->setFocus();
                else
                    ui->pushButton_cancel->setFocus();
            }
            return ;
    }
}
void CDeviceModPreview::on_pushButton_ok_clicked()
{
    accept();
}

void CDeviceModPreview::on_pushButton_cancel_clicked()
{
    reject();
}
