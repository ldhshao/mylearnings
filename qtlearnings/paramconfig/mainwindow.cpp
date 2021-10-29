#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Util/PageCfg.h"
#include "UiCommon/uipage.h"
#include "UiCommon/pagecontainer.h"
#include "UiCommon/qkeytools.h"
#include <QComboBox>
#include <fstream>
#define SYSTEM_CFG_FILEPATH "/home/hndz-dhliu/test/system.xml"
#define UITEMP_CFG_FILEPATH "/home/hndz-dhliu/test/uitemplate.xml"
#define PARAMS_FILEPATH     "/home/hndz-dhliu/test/param.dat"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    currMenuIdx = -1;
    pageMgr = new PageContainer(this);
    paramAddr = nullptr;
    paramCount = 0;
    loadParam();
    initMenu();
    pageMgr->move(90, 200);
    QkeyTools::getInstance();
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
    PageCfgList pageCfg(0);
    PageCfg cfgTemplate;
    cfgTemplate.readXmlFile(UITEMP_CFG_FILEPATH);
    devCfg.readDevCfgFile(SYSTEM_CFG_FILEPATH);
    QPushButton *pBtn = new QPushButton (this);
    QComboBox   *pCmb = new QComboBox (this);
    int i = 0;
    pBtn->setText("系统");
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
                pSubBtn->setVisible(false);
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

    initPage();
}

void MainWindow::initPage()
{
    PageCfgList pageCfgList(paramAddr);
    PageCfg cfgTemplate;
    cfgTemplate.readXmlFile(UITEMP_CFG_FILEPATH);
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
       outFile.open(PARAMS_FILEPATH, ios_base::out|ios_base::binary);
       outFile.write(reinterpret_cast<char*>(paramAddr), paramCount*sizeof (uint16_t));
       outFile.close();
    }
    return true;
}
bool MainWindow::loadParam()
{
    if (nullptr == paramAddr){
        ifstream inFile;
        inFile.open(PARAMS_FILEPATH, ios_base::in | ios_base::binary);
        inFile.seekg(0, inFile.end);
        paramCount = inFile.tellg() / sizeof(uint16_t);
        inFile.seekg(0, inFile.beg);
        paramAddr = new uint16_t[paramCount];
        inFile.read(reinterpret_cast<char*>(paramAddr), paramCount*sizeof(uint16_t));
        inFile.close();
    }
    return true;
}
void MainWindow::resizeEvent(QResizeEvent *event)
{
    int l = 90, t = 200, m = 4;
    int width = event->size().width();
    int height = event->size().height();
    pageMgr->resize(width - l - m, height - t - m);
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
    }
}
