#include "cuploadquerywid.h"
#include "ui_cuploadquerywid.h"
#include "cmodparampreview.h"
#include "cmodparamquery.h"
#include "Util/PageCfg.h"
#include <QResizeEvent>
#include <QKeyEvent>
#include <QDebug>

#define WIDGET_BKCOLOR        "#bkground{border-image: url(:/images/mainback.png);}"
#define TITLE_STYLE         "font-size:28px;color:rgba(255,255,255,100%);"
#define COPYRIGHT_STYLE     "color:rgba(255,255,255,100%);"
#define PROPERTY_INDEX "index"
CUploadQueryWid::CUploadQueryWid(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CUploadQueryWid)
{
    bkLbl = new QLabel(this);
    bkLbl->setObjectName("bkground");
    bkLbl->setStyleSheet(WIDGET_BKCOLOR);
    ui->setupUi(this);
    ui->label_title->setStyleSheet(TITLE_STYLE);
    ui->label_copyright->setStyleSheet(COPYRIGHT_STYLE);
    menuWidth = 100;
    menuHeight = 40;
    devUiCfgList = nullptr;
    paramSrvAddr = nullptr;
    paramLclAddr = nullptr;
    //editing = false;
    initMenu();
    connect(&menu2Mgr, SIGNAL(sig_button_clicked(CStateButton*)), this, SLOT(slot_menu2_clicked(CStateButton*)));
}

CUploadQueryWid::~CUploadQueryWid()
{
    delete ui;
}

void CUploadQueryWid::showUi()
{
    //editing = false;
    for (auto it = menu2List.begin(); it != menu2List.end(); it++) {
        QWidget *w = (*it)->getWidget();
        if (nullptr != w) w->setEnabled(false);
        CModParamPreview* preview = dynamic_cast<CModParamPreview*>(w);
        if (nullptr != preview && nullptr != devUiCfgList && nullptr != paramSrvAddr && nullptr != paramLclAddr){
            itemList.clear();
            int idx = 0;
            int idx_end = devUiCfgList->datacount();
            while (idx < idx_end){
                UiCfgItem* pItem = devUiCfgList->findItemByDataIdx(idx);
                QString name, value;
                int dataCnt = 0;
                if (nullptr != pItem){
                    value = pItem->getDataValue(paramLclAddr+idx, &dataCnt);
                    if (0 != memcmp(paramLclAddr+idx, paramSrvAddr + idx, dataCnt*sizeof(uint16_t))){
                        itemList.push_back(pItem);
                    }
                    idx += dataCnt;
                }else {
                    idx++;
                }
                qDebug()<<name<<value<<dataCnt;
            }
            preview->updateItemList(&itemList);
        }
    }

    menu2Mgr.selectButton(menu2List[0]);
    show();
}

void CUploadQueryWid::initMenu()
{
    int i = 0;
    CModParamPreview* widPreview = new CModParamPreview(nullptr, this);
    CModParamQuery*   widQuery = new CModParamQuery(this);
    widPreview->hide();
    widQuery->hide();
    CKeyWidgetButton* pBtn = new CKeyWidgetButton(widPreview, this);
    pBtn->setText("参数变更上传");
    pBtn->setProperty(PROPERTY_INDEX, i++);
    pBtn->resize(menuWidth, menuHeight);
    menu2Mgr.registerButton(pBtn);
    menu2List.push_back(pBtn);

    pBtn = new CKeyWidgetButton(widQuery, this);
    pBtn->setText("参数变更查询");
    pBtn->setProperty(PROPERTY_INDEX, i++);
    pBtn->resize(menuWidth, menuHeight);
    menu2Mgr.registerButton(pBtn);
    menu2List.push_back(pBtn);

    pBtn = new CKeyWidgetButton(nullptr, this);
    pBtn->setText("返回");
    pBtn->setProperty(PROPERTY_INDEX, i++);
    pBtn->resize(menuWidth, menuHeight);
    menu2Mgr.registerButton(pBtn);
    menu2List.push_back(pBtn);
}

void CUploadQueryWid::slot_menu2_clicked(CStateButton* pBtn)
{
    int idx = pBtn->property(PROPERTY_INDEX).toInt();
    if (idx == menu2List.size() - 1){
        hide();
    }
}

void CUploadQueryWid::onResize(int width, int height)
{
    int l = 120, t = 100, m = 4, s = 10;
    int i = 0;

    //layout background
    bkLbl->resize(width, height);
    bkLbl->move(0, 0);

    //layout title
    int tWidth = QFontMetrics(ui->label_title->font()).width(ui->label_title->text());
    int tHeight = QFontMetrics(ui->label_title->font()).height();
    ui->label_title->resize(tWidth, tHeight);
    ui->label_title->move((width - tWidth)/2, 20);

    //layout menu2
    int span = menuHeight + s;
    for (auto it = menu2List.begin(); it != menu2List.end(); it++) {
        (*it)->move(m, t + i * span);
        QWidget *w = (*it)->getWidget();
        if (nullptr != w){
            w->resize(width - l, height -50 - t);
            w->move(l, t);
        }
        i++;
    }
    qDebug()<<__FUNCTION__<<" idx "<<i;

    //layout copyright
    ui->label_copyright->move((width - ui->label_copyright->width())/2, height - 50);
    update(0, 0, width, height);
}
void CUploadQueryWid::resizeEvent(QResizeEvent *event)
{
    onResize(event->size().width(), event->size().height());
    QMainWindow::resizeEvent(event);
}

void CUploadQueryWid::keyPressEvent(QKeyEvent *event)
{
    CKeyWidgetButton* pBtn = dynamic_cast<CKeyWidgetButton*>(menu2Mgr.currentButton());
    int idx = pBtn->property(PROPERTY_INDEX).toInt();
    switch (event->key()) {
    case Qt::Key_Return:
        {
            QWidget *w = pBtn->getWidget();
            w->setEnabled(true);
            w->setFocus();
        }
        break;
    case Qt::Key_Up:
        idx = (idx - 1 + menu2List.size()) % menu2List.size();
        menu2Mgr.selectButton(menu2List[idx]);
        break;
    case Qt::Key_Down:
        idx = (idx + 1) % menu2List.size();
        menu2Mgr.selectButton(menu2List[idx]);
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }
}
