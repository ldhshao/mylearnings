#include "cmodparampreview.h"
#include "Util/UiCfgItem.h"
#include "UiCommon/cenablemngr.h"
#include "ui_cmodparampreview.h"
#include <QResizeEvent>
#include <QDebug>

#define PROPERTY_INDEX "index"
#define PROPERTY_VALUE "value"
#define PROPERTY_ITEM  "item"
CModParamPreview::CModParamPreview(list<UiCfgItem*>* iLst, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CModParamPreview), itemList(iLst)
{
    ui->setupUi(this);
    btnList.push_back(ui->pushButton_sync);
    btnList.push_back(ui->pushButton_cancel);
    btnList.push_back(ui->pushButton_ok);
    for (int i = 0; i < btnList.size(); i++) {
        btnList[i]->setProperty(PROPERTY_INDEX, i);
    }
    initTable();
    //ui->tableWidget->setFocus();
}

CModParamPreview::~CModParamPreview()
{
    delete ui;
}

void CModParamPreview::initTable()
{
    QStringList headList;
    headList<<"名称"<<"数值"<<"是否上传"<<"操作";
    if (nullptr != itemList && !itemList->empty()){
        ui->tableWidget->setRowCount(itemList->size());
        ui->tableWidget->setColumnCount(headList.count());
        ui->tableWidget->setHorizontalHeaderLabels(headList);

        int row = 0;
        for (auto it = itemList->begin(); it != itemList->end(); it++) {
            QTableWidgetItem *item = new QTableWidgetItem((*it)->getFullName());
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableWidget->setItem(row, 0, item);

            item = new QTableWidgetItem((*it)->strDataValue());
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableWidget->setItem(row, 1, item);

            item = new QTableWidgetItem("上传");
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableWidget->setItem(row, 2, item);

            QPushButton* btn = new QPushButton(ui->tableWidget);
            btn->setText("取消上传");
            btn->setProperty(PROPERTY_INDEX, row);
            btn->setProperty(PROPERTY_VALUE, true);
            QVariant var;
            var.setValue<void*>(*it);
            btn->setProperty(PROPERTY_ITEM, var);
            connect(btn, SIGNAL(clicked()), this, SLOT(slot_operateParam()));
            ui->tableWidget->setCellWidget(row, 3, btn);
            row++;
        }
    }
    ui->tableWidget->resizeColumnsToContents();
}

void CModParamPreview::slot_operateParam()
{
    QPushButton *btn = dynamic_cast<QPushButton*>(sender());
    if (nullptr != btn){
        bool upload = btn->property(PROPERTY_VALUE).toBool();
        int  row = btn->property(PROPERTY_INDEX).toInt();
        upload = !upload;
        btn->setText(upload?"取消上传":"上传");
        btn->setProperty(PROPERTY_VALUE, upload);
        ui->tableWidget->item(row, 2)->setText(upload?"上传":"不上传");

        UiCfgItem* item = static_cast<UiCfgItem*>(btn->property(PROPERTY_ITEM).value<void*>());
        if (nullptr != item){
        //deal relation param
        //as B
            UiCfgItem* srcItem = item->enableSource();
            if (nullptr != srcItem){
                bool srcExist = false;
                list<QPushButton*> btnList;
                for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
                    QPushButton* btn0 = dynamic_cast<QPushButton*>(ui->tableWidget->cellWidget(i, 3));
                    UiCfgItem* itemTmp = nullptr;
                    if (nullptr != btn0){
                        itemTmp = static_cast<UiCfgItem*>(btn0->property(PROPERTY_ITEM).value<void*>());
                    }
                    if (itemTmp == srcItem){
                        btnList.push_back(btn0);
                        srcExist = true;
                    }else if (srcItem == itemTmp->enableSource()){
                        btnList.push_back(btn0);
                    }
                }

                if (srcExist){
                    for (auto it = btnList.begin(); it != btnList.end(); it++) {
                        int  r = (*it)->property(PROPERTY_INDEX).toInt();
                        (*it)->setText(upload?"取消上传":"上传");
                        (*it)->setProperty(PROPERTY_VALUE, upload);
                        ui->tableWidget->item(r, 2)->setText(upload?"上传":"不上传");
                    }
                }
            }
        //as A
            uint16_t* pVal = item->paramAddress();
            if (CEnableMngr::instance()->isEnableSource(pVal)){
                list<QPushButton*> btnList;
                for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
                    QPushButton* btn0 = dynamic_cast<QPushButton*>(ui->tableWidget->cellWidget(i, 3));
                    UiCfgItem* itemTmp = nullptr;
                    if (nullptr != btn0){
                        itemTmp = static_cast<UiCfgItem*>(btn0->property(PROPERTY_ITEM).value<void*>());
                    }
                    if (item == itemTmp->enableSource()){
                        btnList.push_back(btn0);
                    }
                }

                for (auto it = btnList.begin(); it != btnList.end(); it++) {
                    int  r = (*it)->property(PROPERTY_INDEX).toInt();
                    (*it)->setText(upload?"取消上传":"上传");
                    (*it)->setProperty(PROPERTY_VALUE, upload);
                    ui->tableWidget->item(r, 2)->setText(upload?"上传":"不上传");
                }
            }
        }
    }
}

QWidget* CModParamPreview::getCloseWidget(int x, int y)
{
    int iMin = width() * width() + height() * height();
    QWidget* pFind = nullptr;
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

int CModParamPreview::getCloseCell(int x, int r)
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

void CModParamPreview::resizeEvent(QResizeEvent *event)
{
    int m = 16, s = 8;
    int w = event->size().width(), h = event->size().height();
    int btnW = ui->pushButton_ok->width();
    int btnH = ui->pushButton_ok->height();

    ui->tableWidget->resize(w - 2*m, h - 2*m- s - btnH);
    ui->tableWidget->move(m, m);
    ui->pushButton_sync->move(m, h - m - btnH);
    ui->pushButton_cancel->move(w - m - 2*btnW - s, h - m - btnH);
    ui->pushButton_ok->move(w - m - btnW, h - m - btnH);
}

void CModParamPreview::keyPressEvent(QKeyEvent *event)
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
        case Qt::Key_Return:
            if (nullptr != pBtn){
                if (pBtn == ui->pushButton_cancel)
                    on_pushButton_cancel_clicked();
                else if (pBtn == ui->pushButton_ok)
                    on_pushButton_ok_clicked();
            }
            return ;
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
            if (nullptr != pBtn){
                int idx = pBtn->property(PROPERTY_INDEX).toInt();
                do{
                    idx = (idx - 1 + btnList.size()) % btnList.size();
                }while (!btnList[idx]->isEnabled());
                btnList[idx]->setFocus();
            }
            return ;
        case Qt::Key_Right:
            if (nullptr != pBtn){
                int idx = pBtn->property(PROPERTY_INDEX).toInt();
                do {
                    idx = (idx + 1) % btnList.size();
                }while (!btnList[idx]->isEnabled());
                qDebug()<<btnList[idx];
                btnList[idx]->setFocus();
            }
            return ;
    }
}
void CModParamPreview::on_pushButton_ok_clicked()
{
    if (nullptr != itemList){
        itemList->clear();
        for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
            QPushButton* btn0 = dynamic_cast<QPushButton*>(ui->tableWidget->cellWidget(i, 3));
            bool upload = false;
            if (nullptr != btn0){
                upload = btn0->property(PROPERTY_VALUE).toBool();
            }
            if (upload){
                UiCfgItem* item = static_cast<UiCfgItem*>(btn0->property(PROPERTY_ITEM).value<void*>());
                itemList->push_back(item);
            }
        }
    }
    accept();
}

void CModParamPreview::on_pushButton_cancel_clicked()
{
    reject();
}

void CModParamPreview::on_pushButton_sync_clicked()
{

}
