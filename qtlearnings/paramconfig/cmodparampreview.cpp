#include "cmodparampreview.h"
#include "Util/UiCfgItem.h"
#include "UiCommon/cenablemngr.h"
#include "ui_cmodparampreview.h"
#include <QResizeEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <math.h>
#include <QDebug>

#define PROPERTY_INDEX "index"
#define PROPERTY_VALUE "value"
#define PROPERTY_ITEM  "item"
//#define LABEL_STYLE         "font-size:28px;color:rgba(255,255,255,100%);"
#define LABEL_STYLE         "color:rgba(255,255,255,100%);"
CModParamPreview::CModParamPreview(list<UiCfgItem*>* iLst, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CModParamPreview), itemList(iLst)
{
    setWindowFlags(/*Qt::Tool | Qt::WindowStaysOnTopHint |*/ Qt::FramelessWindowHint);
    ui->setupUi(this);
    ui->label_tableName->setText("参数变更预览表");
    ui->label_tableName->setStyleSheet(LABEL_STYLE);
    ui->label_lineno->setStyleSheet(LABEL_STYLE);
    //initTable();
    //ui->tableWidget->setFocus();
    //setAutoFillBackground(true);
}

CModParamPreview::~CModParamPreview()
{
    delete ui;
}

void CModParamPreview::updateItemList(list<UiCfgItem *> *iList)
{
    ui->tableWidget->clear();
    itemList = iList;
    initTable();
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

            CKeyButton* btn = new CKeyButton(ui->tableWidget);
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
    autoAdjustTableColumns();
    if (0 < ui->tableWidget->rowCount()){
        ui->label_lineno->setText(QString::asprintf("共%d行", ui->tableWidget->rowCount()));
    }else{
        ui->label_lineno->setText("无参数变更");
    }
}

void CModParamPreview::autoAdjustTableColumns()
{
    ui->tableWidget->resizeColumnsToContents();
    list<int> widths;
    int colWidthTotal = 0;
    for (int i = 0; i < 4; i++) {
        colWidthTotal += ui->tableWidget->columnWidth(i);
        widths.push_back(ui->tableWidget->columnWidth(i));
    }
    qDebug()<<"test: "<<ui->tableWidget->width()<<colWidthTotal;
    if (ui->tableWidget->width() - colWidthTotal > 80){
        list<float> fList;
        fList.push_back(0.5);
        fList.push_back(0.3);
        fList.push_back(0.1);
        fList.push_back(0.1);
        int rest = ui->tableWidget->width() - colWidthTotal - 80;
        int i = 0;
        auto itFct = fList.begin();
        for (auto it = widths.begin(); it != widths.end(); it++, itFct++, i++){
            ui->tableWidget->setColumnWidth(i, *it + rest * (*itFct));
        }
    }
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
    int btnW = ui->pushButton_upload->width();
    int btnH = ui->pushButton_upload->height();

    int tnW = QFontMetrics(font()).width(ui->label_tableName->text());
    ui->label_tableName->resize(tnW, btnH);
    ui->label_tableName->move(m, m);
    ui->pushButton_upload->move(w - m - btnW, m);
    ui->tableWidget->resize(w - 2*m, h - 2*m- s - 2*btnH);
    ui->tableWidget->move(m, m + btnH + s);
    autoAdjustTableColumns();
    tnW = QFontMetrics(font()).width(ui->label_lineno->text());
    ui->label_lineno->resize(tnW, btnH);
    ui->label_lineno->move(m, h - m - btnH);
    qDebug()<<"CModParamPreview"<<w<<h<<"table "<<ui->tableWidget->width()<<ui->tableWidget->height();
}

void CModParamPreview::keyPressEvent(QKeyEvent *event)
{
    QTableWidget* pTbl = nullptr;
    CKeyButton* pBtn = dynamic_cast<CKeyButton*>(focusWidget());
    qDebug()<<"CModParamPreview::keyPressEvent"<<focusWidget();
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
                    //getCloseWidget(x + rct.left() + rct.width()/2, rct.top() + rct.height()/2)->setFocus();
                    ui->pushButton_upload->setFocus();
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
                    //getCloseWidget(x + rct.left() + rct.width()/2, rct.top() + rct.height()/2)->setFocus();
                    ui->pushButton_upload->setFocus();
                }
            }else {
                int row = 0;
                int x = pBtn->pos().x() + pBtn->width() / 2;
                int col = getCloseCell(x, row);
                ui->tableWidget->setCurrentCell(row, col);
                ui->tableWidget->setFocus();
            }
            return ;
        //case Qt::Key_Left:
        //    if (nullptr != pBtn){
        //        int idx = pBtn->property(PROPERTY_INDEX).toInt();
        //        do{
        //            idx = (idx - 1 + btnList.size()) % btnList.size();
        //        }while (!btnList[idx]->isEnabled());
        //        btnList[idx]->setFocus();
        //    }
        //    return ;
        //case Qt::Key_Right:
        //    if (nullptr != pBtn){
        //        int idx = pBtn->property(PROPERTY_INDEX).toInt();
        //        do {
        //            idx = (idx + 1) % btnList.size();
        //        }while (!btnList[idx]->isEnabled());
        //        qDebug()<<btnList[idx];
        //        btnList[idx]->setFocus();
        //    }
        //    return ;
        case Qt::Key_Escape:
            setEnabled(false);
            return ;
        default:
            QWidget::keyPressEvent(event);
    }
}
void CModParamPreview::changeEvent(QEvent *event)
{
    switch (event->type()) {
    case QEvent::EnabledChange:
        qDebug()<<"CModParamPreview::changeEvent";
        if (isEnabled()){
            //if (0 < ui->tableWidget->rowCount()){
            //    ui->tableWidget->setFocus();
            //    ui->tableWidget->setCurrentCell(0, 0);
            //}else{
            //    ui->pushButton_upload->setFocus();
            //}
            ui->pushButton_upload->setFocus();
        }
        //break;
    default:
        QWidget::changeEvent(event);
    }
}

void CModParamPreview::on_pushButton_send_clicked()
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
    //accept();
}
