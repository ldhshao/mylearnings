#include "cdeviceconfig.h"
#include "ui_cdeviceconfig.h"
#include "cdevicepreview.h"
#include "Util/PageCfg.h"
#include "Util/devcfg.h"
#include <QResizeEvent>
#include <QKeyEvent>
#include <QDebug>

//#define WIDGET_BKCOLOR      "background-color: rgba(12, 33, 107, 0%);"
#define WIDGET_BKCOLOR        "border-image: url(:/images/mainback.png);"
#define TITLE_STYLE         "font-size:28px;color:rgba(255,255,255,100%);"
#define COPYRIGHT_STYLE     "color:rgba(255,255,255,100%);"
#define PROPERTY_DEVICE "device"
#define PROPERTY_INDEX "index"
CDeviceConfig::CDeviceConfig(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CDeviceConfig)
{
    bkLbl = new QLabel(this);
    bkLbl->setStyleSheet(WIDGET_BKCOLOR);
    ui->setupUi(this);
    ui->label_title->setStyleSheet(TITLE_STYLE);
    ui->label_copyright->setStyleSheet(COPYRIGHT_STYLE);
    devCfg = nullptr;
    devUiCfgList = nullptr;
    menu2Cnt = 0;
    menu3Cnt = 0;
    menu4Cnt = 0;
    menuWidth = 100;
    menuHeight = 40;
    menuExit = new CKeyStateButton(this);
    menuExit->setText("返回");
    menuExit->resize(menuWidth, menuHeight);
    menu2Mgr.registerButton(menuExit);
    preview = new CDevicePreview(this);

    connect(&menu2Mgr, SIGNAL(sig_button_clicked(CStateButton*)), this, SLOT(slot_menu2_clicked(CStateButton*)));
    //connect(&menu3Mgr, SIGNAL(sig_button_clicked(CStateButton*)), this, SLOT(slot_menu3_clicked(CStateButton*)));
    //connect(&menu4Mgr, SIGNAL(sig_button_clicked(CStateButton*)), this, SLOT(slot_menu4_clicked(CStateButton*)));
    //ui->horizontalLayout->addWidget(pBtn);
}

CDeviceConfig::~CDeviceConfig()
{
    delete ui;
    delete menuExit;
    delete preview;
    delete bkLbl;
}

void CDeviceConfig::updateUi(DevCfgList* dev, PageCfgList* uiCfg)
{
    devCfg = dev;
    devUiCfgList = uiCfg;
    ui->label_title->setText(devCfg->getName() + "参数配置");
    if (nullptr != devCfg && nullptr != devUiCfgList){
        GroupCfgItem* grpItem = devUiCfgList->findGroupByName(devCfg->getName());
        if (nullptr != grpItem)
            initMenu2(grpItem);
    }
}

void CDeviceConfig::showUi(int reason)
{
    if (1 == reason){
        showMaximized();
    }else {
        show();
    }

    onResize(width(), height());
}
void CDeviceConfig::initMenu2(GroupCfgItem* grpItem)
{
    int cnt0 = menu2List.size();
    int cnt1 = grpItem->getChildrenCount();
    if (cnt0 < cnt1){
        for (int i=0; i < cnt1-cnt0; i++) {
            CStateButton* pBtn = new CKeyStateButton(this);
            pBtn->setProperty(PROPERTY_INDEX, cnt0+i);
            pBtn->resize(menuWidth, menuHeight);
            menu2Mgr.registerButton(pBtn);
            menu2List.push_back(pBtn);
        }
    }
    int idx = 0;
    UiCfgItem* pItem = grpItem->getHead();
    while(nullptr != pItem){
        QVariant var;
        var.setValue<void*>(pItem);
        menu2List[idx]->setProperty(PROPERTY_DEVICE, var);
        menu2List[idx]->setText(pItem->getName());
        menu2List[idx]->setVisible(true);
        pItem = grpItem->getNext();
        idx++;
    }
    menu2Cnt = idx;
    qDebug()<<__FUNCTION__<<" idx "<<idx;
    for (; idx < cnt0; idx++) {
        menu2List[idx]->setVisible(false);
    }

    qDebug()<<__FUNCTION__;
    menu2Mgr.selectButton(menu2List[0]);
}

void CDeviceConfig::showPreview(GroupCfgItem* grpCfg)
{
    preview->clearConfigPage();

    GroupCfgList* list = dynamic_cast<GroupCfgList*>(grpCfg);
    if (nullptr != list){
        preview->updateByDevice(dynamic_cast<GroupCfgItem*>(list->getHead()));
    }
    else{
        preview->updateByDevice(grpCfg);
    }
}

void CDeviceConfig::showPreview(CStateButton* menu)
{
    GroupCfgItem* group = static_cast<GroupCfgItem*>(menu->property(PROPERTY_DEVICE).value<void*>());
    showPreview(group);
}

void CDeviceConfig::onResize(int width, int height)
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
        if (!(*it)->isVisible())
            break;
        (*it)->move(m, t + i * span);
        i++;
    }
    menuExit->move(m, t+i*span);
    qDebug()<<__FUNCTION__<<" idx "<<i;

    //layout menu3
    span = menuWidth + s;
    i = 0;
    for (auto it = menu3List.begin(); it != menu3List.end(); it++) {
        if (!(*it)->isVisible())
            break;
        (*it)->move(l + i*span, t);
        i++;
    }
    qDebug()<<__FUNCTION__<<" idx "<<i;

    //layout menu4
    t += menuHeight;
    i = 0;
    for (auto it = menu4List.begin(); it != menu4List.end(); it++) {
        if (!(*it)->isVisible())
            break;
        (*it)->move(l + i*span, t);
        i++;
    }
    qDebug()<<__FUNCTION__<<" idx "<<i;

    //layout preview area
    if (0 < i){
        t += menuHeight;//
    }
    preview->resize(width - l, height -50 - t);
    preview->move(l, t);

    //layout copyright
    ui->label_copyright->move((width - ui->label_copyright->width())/2, height - 50);
    update(0, 0, width, height);
}
void CDeviceConfig::resizeEvent(QResizeEvent *event)
{
    onResize(event->size().width(), event->size().height());
    QMainWindow::resizeEvent(event);
}

void CDeviceConfig::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<"deviceconfig "<<__FUNCTION__;
    bool bMenu4Show = (nullptr != menu4Mgr.currentButton());
    switch (event->key()) {
        case Qt::Key_Return:
            if (!bMenu4Show){//menu3 show
                slot_menu3_clicked(menu3Mgr.currentButton());
            }else {//menu4 show
                slot_menu4_clicked(menu4Mgr.currentButton());
            }
            return ;
        case Qt::Key_Up:
            if (!bMenu4Show){//menu3 show
                int idx = menu2Mgr.currentButton()->property(PROPERTY_INDEX).toInt();
                qDebug()<<"1 index "<<idx;
                idx = (idx - 1 + menu2Cnt + 1) % (menu2Cnt + 1);
                qDebug()<<"2 index "<<idx;
                if (idx == menu2Cnt)
                    menu2Mgr.selectButton(menuExit);
                else
                    menu2Mgr.selectButton(menu2List[idx]);
            }else {//menu4 show
                for (auto it = menu4List.begin(); it != menu4List.end(); it++) {
                    (*it)->hide();
                }
                menu4Mgr.selectButton(nullptr);
                onResize(width(),height());
            }
            return ;
        case Qt::Key_Down:
            if (!bMenu4Show){//menu3 show
                int idx = menu2Mgr.currentButton()->property(PROPERTY_INDEX).toInt();
                qDebug()<<"3 index "<<idx;
                idx = (idx + 1) % (menu2Cnt + 1);
                qDebug()<<"4 index "<<idx;
                if (idx == menu2Cnt)
                    menu2Mgr.selectButton(menuExit);
                else
                    menu2Mgr.selectButton(menu2List[idx]);
            }else {//menu4 show
            }
            return ;
        case Qt::Key_Left:
            if (!bMenu4Show){//menu3 show
                int idx = menu3Mgr.currentButton()->property(PROPERTY_INDEX).toInt();
                idx = (idx - 1 + menu3Cnt) % menu3Cnt;
                menu3Mgr.selectButton(menu3List[idx]);
                showPreview(menu3List[idx]);
            }else {//menu4 show
                int idx = menu4Mgr.currentButton()->property(PROPERTY_INDEX).toInt();
                idx = (idx - 1 + menu4Cnt) % menu4Cnt;
                menu4Mgr.selectButton(menu4List[idx]);
                showPreview(menu4List[idx]);
            }
            return ;
        case Qt::Key_Right:
            if (!bMenu4Show){//menu3 show
                int idx = menu3Mgr.currentButton()->property(PROPERTY_INDEX).toInt();
                idx = (idx + 1) % menu3Cnt;
                menu3Mgr.selectButton(menu3List[idx]);
                showPreview(menu3List[idx]);
            }else {//menu4 show
                int idx = menu4Mgr.currentButton()->property(PROPERTY_INDEX).toInt();
                idx = (idx + 1) % menu4Cnt;
                menu4Mgr.selectButton(menu4List[idx]);
                showPreview(menu4List[idx]);
            }
            return ;
    }
}
void CDeviceConfig::slot_menu2_clicked(CStateButton* pBtn)
{
    if (pBtn == menuExit){
        hide();
        return;
    }
    int cnt0 = menu3List.size();
    GroupCfgItem* group = static_cast<GroupCfgItem*>(pBtn->property(PROPERTY_DEVICE).value<void*>());
    GroupCfgList* list = dynamic_cast<GroupCfgList*>(group);
    int idx = 0;
    if (nullptr != list){
        int cnt1 = list->getChildrenCount();
        if (cnt0 < cnt1){
            for (int i=0; i < cnt1-cnt0; i++) {
                CStateButton* pBtn = new CKeyStateButton(this);
                pBtn->setProperty(PROPERTY_INDEX, cnt0+i);
                pBtn->resize(menuWidth, menuHeight);
                menu3Mgr.registerButton(pBtn);
                menu3List.push_back(pBtn);
            }
        }
        UiCfgItem* pItem = list->getHead();
        while(nullptr != pItem){
            QVariant var;
            var.setValue<void*>(pItem);
            menu3List[idx]->setProperty(PROPERTY_DEVICE, var);
            menu3List[idx]->setText(pItem->getName());
            menu3List[idx]->show();
            pItem = list->getNext();
            idx++;
        }
    }else {
        if (0 == cnt0){
            CStateButton* pBtn = new CKeyStateButton(this);
            pBtn->setProperty(PROPERTY_INDEX, 0);
            pBtn->resize(menuWidth, menuHeight);
            menu3Mgr.registerButton(pBtn);
            menu3List.push_back(pBtn);
        }
            QVariant var;
            var.setValue<void*>(group);
            menu3List[idx]->setProperty(PROPERTY_DEVICE, var);
            menu3List[idx]->setText("基本信息");
            menu3List[idx]->show();
            idx++;
    }
    menu3Cnt = idx;
    for (; idx < cnt0; idx++) {
        menu3List[idx]->hide();
    }

    menu3Mgr.selectButton(menu3List[0]);
    showPreview(menu3List[0]);
    qDebug()<<__FUNCTION__;
}
void CDeviceConfig::slot_menu3_clicked(CStateButton* pBtn)
{
    GroupCfgItem* group = static_cast<GroupCfgItem*>(pBtn->property(PROPERTY_DEVICE).value<void*>());
    GroupCfgList* list = dynamic_cast<GroupCfgList*>(group);
    if (nullptr != list){
        //show menu4
        int cnt0 = menu4List.size();
        int cnt1 = list->getChildrenCount();
        if (cnt0 < cnt1){
            for (int i=0; i < cnt1-cnt0; i++) {
                CStateButton* pBtn = new CKeyStateButton(this);
                pBtn->setProperty(PROPERTY_INDEX, cnt0+i);
                pBtn->resize(menuWidth, menuHeight);
                menu4Mgr.registerButton(pBtn);
                menu4List.push_back(pBtn);
            }
        }
        int idx = 0;
        UiCfgItem* pItem = list->getHead();
        while(nullptr != pItem){
            QVariant var;
            var.setValue<void*>(pItem);
            menu4List[idx]->setProperty(PROPERTY_DEVICE, var);
            menu4List[idx]->setText(pItem->getName());
            menu4List[idx]->setVisible(true);
            pItem = list->getNext();
            idx++;
        }
        menu4Cnt = idx;
        qDebug()<<__FUNCTION__<<" idx "<<idx;
        for (; idx < cnt0; idx++) {
            menu4List[idx]->setVisible(false);
        }
        menu4Mgr.selectButton(menu4List[0]);

        onResize(width(), height());
    }else {
        menu4Cnt = 1;
        //show config UI
        preview->showConfigPage(group);
    }
    qDebug()<<__FUNCTION__;
}
void CDeviceConfig::slot_menu4_clicked(CStateButton* pBtn)
{
    GroupCfgItem* group = static_cast<GroupCfgItem*>(pBtn->property(PROPERTY_DEVICE).value<void*>());
    preview->showConfigPage(group);
}
