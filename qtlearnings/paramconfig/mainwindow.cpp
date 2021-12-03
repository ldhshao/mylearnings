#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Util/PageCfg.h"
#include "UiCommon/uipage.h"
#include "UiCommon/pagecontainer.h"
#include "UiCommon/qkeytools.h"
#include "cdevposctl1.h"
#include "cdevposctl2.h"
#include "clineselector.h"
#include <QComboBox>
#include <QCoreApplication>
#include <fstream>
#define SYSTEM_CFG_FILEPATH "system.xml"
#define UITEMP_CFG_FILEPATH "uitemplate.xml"
#define PARAMS_FILEPATH     "param.dat"
#define MENU1_STYLE         "QPushButton {font-size:32px; font:bold;color:rgba(255,255,255,100%);background-color:rgba(80,80,100,100%);}"
#define MENU1_CMB_STYLE     "QComboBox {font-size:32px; font:bold;color:rgba(255,255,255,100%);background-color:rgba(80,80,100,100%);}\
                             QComboBox QAbstractItemView{background-color: #646478;color: #FFFFFF;selection-background-color: #505064;selection-color: #FFFFFF;}"
// #define MENU1_CMB_STYLE     "QComboBox QAbstractItemView{\
//        background-color: #4f4f4f;\
//        color: #999999;\
//     \
//        selection-background-color: #999999;\
//        selection-color: #4f4f4f;\
//    }"
#define MENU2_STYLE         "QPushButton {font-size:28px; font:bold;}"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    currMenuIdx = -1;
    pageMgr = new PageContainer(this);
    paramAddr = nullptr;
    paramCount = 0;
    appDir = QCoreApplication::applicationDirPath();
    loadParam();
    initMenu();
    //QkeyTools::getInstance();
    onResize(width(), height());
    slot_systemClicked(false);
}

MainWindow::~MainWindow()
{
    saveParam();
    delete []paramAddr;
    deleteAll();
    delete ui;
}

void MainWindow::initMenu()
{
    QString strDevCfg = appDir + "/" + SYSTEM_CFG_FILEPATH;
    devCfg.readDevCfgFile(strDevCfg);
    QPushButton *pBtn = new QPushButton (this);
    QComboBox   *pCmb = new QComboBox (this);
    int i = 0;
    pBtn->setText("系统");
    pBtn->setStyleSheet(MENU1_STYLE);
    pBtn->resize(120, 40);
    pCmb->setStyleSheet(MENU1_CMB_STYLE);
    pCmb->resize(120, 40);
    pCmb->insertItem(i++, "选择设备");
    menu2Lists.resize(devCfg.getChildrenCount());
    menu2MgrList.resize(devCfg.getChildrenCount());
    DevCfgItem *pItem = devCfg.getHead();
    while (nullptr != pItem) {
        int idx = i;
        if (DevCfgItem::DevTypeSystem != pItem->getType()){
            pCmb->insertItem(i++, pItem->getName());
        }else{
            idx = 0;
        }
        CStateButtonMgr* pBtnMgr = new CStateButtonMgr();
        DevCfgList* pList = dynamic_cast<DevCfgList*>(pItem);
        if (nullptr != pList){
            DevCfgItem* pSubItem = pList->getHead();
            list<CStateButton*> btnList;
            while(nullptr != pSubItem){
                CStateButton* pSubBtn = new CStateButton();
                QString strStyle = pSubBtn->styleSheet();
                pSubBtn->setVisible(false);
                pSubBtn->setStyleSheet(strStyle.append(MENU2_STYLE));
                pSubBtn->setText(pSubItem->getName());
                btnList.push_back(pSubBtn);
                ui->verticalLayout->addWidget(pSubBtn);
                pBtnMgr->registerButton(pSubBtn);
                pSubItem = pList->getNext();
            }
            menu2Lists[idx] = btnList;
            menu2MgrList[idx] = pBtnMgr;
        }
        connect(pBtnMgr, SIGNAL(sig_button_clicked(CStateButton*)), this, SLOT(slot_menu2Clicked(CStateButton*)));
        pItem = devCfg.getNext();
    }

    connect(pBtn, SIGNAL(clicked(bool)), this, SLOT(slot_systemClicked(bool)));
    connect(pCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_deviceChanged(int)));
    ui->horizontalLayout->addWidget(pBtn);
    ui->horizontalLayout->addWidget(pCmb);
    cmbDevices = pCmb;

    pBtn = new QPushButton(this);
    pBtn->setText("通讯");
    pBtn->setStyleSheet(MENU1_STYLE);
    pBtn->resize(120, 40);
    ui->horizontalLayout->addWidget(pBtn);
    //pBtn = new QPushButton(this);
    //pBtn->setText("帮助");
    //pBtn->setStyleSheet(MENU1_STYLE);
    //pBtn->resize(120, 40);
    //ui->horizontalLayout->addWidget(pBtn);
    pBtn = new QPushButton(this);
    pBtn->setText("关于我们");
    pBtn->setStyleSheet(MENU1_STYLE);
    pBtn->resize(120, 40);
    ui->horizontalLayout->addWidget(pBtn);

    initPage();
}

void MainWindow::initPage()
{
    PageCfgList pageCfgList(paramAddr);
    PageCfg cfgTemplate;
    QString strTemplate = appDir + "/" + UITEMP_CFG_FILEPATH;
    cfgTemplate.readXmlFile(strTemplate);
    DevCfgItem *pItem = devCfg.getHead();
    while (nullptr != pItem) {
        DevCfgList* pList = dynamic_cast<DevCfgList*>(pItem);
        if (nullptr != pList){
            DevCfgItem* pSubItem = pList->getHead();
            while(nullptr != pSubItem){
                PageCfg* pageCfg = new PageCfg();
                pageCfg->setName(pSubItem->getName());
                //add 2nd leve device base info
                GroupCfgItem *pGroup = cfgTemplate.findGroupByName(pSubItem->getType());
                if (nullptr != pGroup){
                    UiCfgItem* grp = pGroup->createMyself();
                    grp->setPos(1, 1);
                    grp->setName(pSubItem->getName());
                    pageCfg->addChild(grp);
                }

                DevCfgList* pLel2Dev = dynamic_cast<DevCfgList*>(pSubItem);
                if (nullptr != pLel2Dev){
                    //add 3nd leve device base info
                    int top = 2;
                    DevCfgItem* pLel3Item = pLel2Dev->getHead();
                    while(nullptr != pLel3Item){
                        GroupCfgItem* pGrp3 = cfgTemplate.findGroupByName(pLel3Item->getType());
                        if (nullptr != pGrp3){
                            UiCfgItem* grp = pGrp3->createMyself();
                            grp->setPos(1, top++);
                            grp->setName(pLel3Item->getName());
                            pageCfg->addChild(grp);
                        }
                        pLel3Item = pLel2Dev->getNext();
                    }
                }
                pageCfgList.addChild(pageCfg);
                pSubItem = pList->getNext();
            }
        }
        pItem = devCfg.getNext();
    }

    //check data count
    int iCnt = pageCfgList.getDataCount();
    if (nullptr != paramAddr){
        if (paramCount != iCnt){
            delete[] paramAddr;
            paramAddr = nullptr;
        }
    }
    paramCount = iCnt;
    if(nullptr == paramAddr){
        paramAddr = new uint16_t[paramCount];
        pageCfgList.setParamTbl(paramAddr);
    }
    pageCfgList.createAllPage(pageList);

    vector<list<CStateButton*>>::iterator itList = menu2Lists.begin();
    for (; itList != menu2Lists.end(); itList++){
        list<CStateButton*>::iterator itBtnPtr = itList->begin();
        for(; itBtnPtr != itList->end(); itBtnPtr++){
            GroupCfgItem* pGroup = dynamic_cast<GroupCfgItem*>(pageCfgList.findGroupByName((*itBtnPtr)->text()));
            PageCfg* pageCfg = dynamic_cast<PageCfg*>(pGroup);
            if (nullptr != pageCfg){
                menu2ToPageMap[(*itBtnPtr)] = pageCfg->getPage();
            }
        }
    }
}

void MainWindow::deleteAll()
{
    int iCnt = devCfg.getChildrenCount();
    for (int i = 0; i < iCnt; i++) {
        list<CStateButton*> btnList = menu2Lists[i];
        for (list<CStateButton*>::iterator it = btnList.begin(); it != btnList.end(); it++){
            delete (*it);
        }
    }
    menu2Lists.clear();

    vector<CStateButtonMgr*>::iterator it = menu2MgrList.begin();
    for (; it != menu2MgrList.end(); it++) {
        delete (*it);
    }
    menu2MgrList.clear();

    //delete pages
    //list<UiPage*>::iterator itPage = pageList.begin();
    //for (; itPage != pageList.end(); itPage++) {
    //    delete (*it);
    //}
    //pageList.clear();
}

void MainWindow::selectMenu(int menuIdx)
{
    if (-1 < currMenuIdx){
        list<CStateButton*> btnList = menu2Lists[currMenuIdx];
        for (list<CStateButton*>::iterator it = btnList.begin(); it != btnList.end(); it++){
            (*it)->setVisible(false);
        }
    }

    list<CStateButton*> btnList = menu2Lists[menuIdx];
    for (list<CStateButton*>::iterator it = btnList.begin(); it != btnList.end(); it++){
        (*it)->setVisible(true);
    }
    ui->verticalLayoutWidget->resize(ui->verticalLayoutWidget->width(), menu2Lists[menuIdx].size() * 40);

    currMenuIdx = menuIdx;

    //show page
    CStateButton* currBtn = menu2MgrList[currMenuIdx]->currentButton();
    if(nullptr == currBtn){
        currBtn = menu2Lists[currMenuIdx].front();
    }
    menu2MgrList[currMenuIdx]->selectButton(currBtn);
}

bool MainWindow::saveParam()
{
    if (nullptr != paramAddr){
       ofstream outFile;
       QString strParam = appDir + "/" + PARAMS_FILEPATH;
       outFile.open(strParam.toStdString(), ios_base::out|ios_base::binary);
       outFile.write(reinterpret_cast<char*>(paramAddr), paramCount*sizeof (uint16_t));
       outFile.close();
    }
    return true;
}
bool MainWindow::loadParam()
{
    if (nullptr == paramAddr){
        ifstream inFile;
        QString strParam = appDir + "/" + PARAMS_FILEPATH;
        inFile.open(strParam.toStdString(), ios_base::in | ios_base::binary);
        inFile.seekg(0, inFile.end);
        paramCount = inFile.tellg() / sizeof(uint16_t);
        if (paramCount > 0){
        inFile.seekg(0, inFile.beg);
            paramAddr = new uint16_t[paramCount];
            inFile.read(reinterpret_cast<char*>(paramAddr), paramCount*sizeof(uint16_t));
        }
        inFile.close();
    }
    return true;
}
void MainWindow::onResize(int width, int height)
{
    int l = 120, t = 140, m = 4;
    pageMgr->move(l, t);
    ui->horizontalLayoutWidget->resize(width - l -l, 40);
    ui->horizontalLayoutWidget->move(l, t - 45);
    ui->verticalLayoutWidget->resize(l, menu2Lists[currMenuIdx].size()*40);
    ui->verticalLayoutWidget->move(0, t);
    pageMgr->resize(width - l - m, height - t - m);
}
void MainWindow::resizeEvent(QResizeEvent *event)
{
    onResize(event->size().width(), event->size().height());
    QMainWindow::resizeEvent(event);
}

void MainWindow::slot_systemClicked(bool)
{
    cmbDevices->setCurrentIndex(0);
    selectMenu(0);
}
void MainWindow::slot_deviceChanged(int newIdx)
{
    //if (0 < newIdx){
        selectMenu(newIdx);
    //}
}
void MainWindow::slot_menu2Clicked(CStateButton* btn)
{
    map<CStateButton*, UiPage*>::iterator it = menu2ToPageMap.find(btn);
    if (menu2ToPageMap.end() != it){
        pageMgr->setContent(it->second);
        it->second->show();
    }
}

static int idx = 0;
void MainWindow::on_pushButton_clicked()
{
   // CDevPosCtl1 *testCtl = new CDevPosCtl1();
    //CDevPosCtl2 *testCtl = new CDevPosCtl2();
    //CDevPosCtl1::instance()->show();
    qDebug()<<"pos "<<CLineSelector::instance()->pos();
    idx++;
    CLineSelector::instance()->adjustPosition(idx*100, 100, 100, 40);
    CLineSelector::instance()->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    qDebug()<<"pos "<<CLineSelector::instance()->pos();
    idx--;
    CLineSelector::instance()->adjustPosition(idx*100, 100, 100, 40);
    CLineSelector::instance()->show();

}
