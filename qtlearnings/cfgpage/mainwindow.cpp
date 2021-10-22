#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Util/ItemBase.h"
#include "Util/Page.h"
#include "UiCommon/uipage.h"
#include "UiCommon/qkeytools.h"

#define MENU_CFG_FILEPATH "/home/hndz-dhliu/test/menu.xml"
#define PAGE_CFG_FILEPATH "/home/hndz-dhliu/test/page.xml"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet("MainWindow {background-color:rgba(40,40,60,100%);}");
    ui->labelCopyright->setStyleSheet("QLabel{color:rgba(255,255,255,100%)}");
    onResize(width(), height());

    initMenu();
    initPage();

    QkeyTools::getInstance();

    menu1Mgr.selectButton(menu1List.front());
}

MainWindow::~MainWindow()
{
    //destroy butttons
    list<CStateButton*>::iterator it = menu1List.begin();
    for (; it != menu1List.end(); it++) {
        delete *it;
    }
    it = menu2List.begin();
    for (; it != menu2List.end(); it++) {
        delete *it;
    }
    //destroy pages
    list<UiPage*>::iterator itPage = pageList.begin();
    for (; itPage != pageList.end(); itPage++) {
        delete *itPage;
    }

    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    //XmlList list;
    //list.readXmlFile("/home/hndz-dhliu/test/menu.txt");
    //list.dump();
}

void MainWindow::initMenu()
{
    menuList.readXmlFile(MENU_CFG_FILEPATH);

    //heduicaidan, zhiyunxuliangjicaidan

    //find max second menu count and create menu1
    int iMenu2Max = 0;
    XmlItem* pItem = menuList.getHead();
    while(nullptr != pItem){
        XmlList *pList = dynamic_cast<XmlList*>(pItem);
        if (nullptr != pList && iMenu2Max < pList->getChildrenCount()){
            iMenu2Max = pList->getChildrenCount();
        }
        CStateButton *pBtn = new CStateButton();
        pBtn->setText(pItem->getName());
        mapBtnToItem[pBtn] = pItem;
        menu1List.push_back(pBtn);
        menu1Mgr.registerButton(pBtn);
        ui->verticalLayout->addWidget(pBtn);
        pItem = menuList.getNext();
    }
    //QSpacerItem* vSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    //ui->verticalLayout->addItem(vSpacer);

    //create menu2
    for(int i = 0; i < iMenu2Max; i++){
        CStateButton *pBtn = new CStateButton();
        mapBtnToItem[pBtn] = nullptr;
        menu2List.push_back(pBtn);
        menu2Mgr.registerButton(pBtn);
        ui->horizontalLayout->addWidget(pBtn);
    }

    //connect
    connect(&menu1Mgr, SIGNAL(sig_button_clicked(CStateButton*)), this, SLOT(slot_button_clicked(CStateButton*)));
    connect(&menu2Mgr, SIGNAL(sig_button_clicked(CStateButton*)), this, SLOT(slot_button_clicked(CStateButton*)));
}
void MainWindow::initPage()
{
    PageCfgList pgeCfgList(paramTable);
    pgeCfgList.readXmlFile(PAGE_CFG_FILEPATH);
    pgeCfgList.getChildrenCount();
    pgeCfgList.createAllPage(pageList);
    //bind page to menu
    UiCfgItem* pItem = pgeCfgList.getHead();
    while(nullptr != pItem){
        PageCfg* page = dynamic_cast<PageCfg*>(pItem);
        if (nullptr != page){
            XmlItem *pMenuItem = menuList.getItemById(page->getId());
            if (nullptr != pMenuItem)
                mapMenuToPage[pMenuItem] = page->getPage();
        }
        pItem = pgeCfgList.getNext();
    }
}

void MainWindow::slot_button_clicked(CStateButton* pBtn)
{
    XmlItem *pItem = mapBtnToItem[pBtn];
    XmlList *pList = dynamic_cast<XmlList*>(pItem);
    if (nullptr != pList){
        pItem = pList->getHead();
        for (list<CStateButton*>::iterator it=menu2List.begin(); it!= menu2List.end(); it++) {
            if (nullptr != pItem){
                (*it)->setText(pItem->getName());
                mapBtnToItem[*it] = pItem;
                (*it)->show();
            }else{
                mapBtnToItem[*it] = nullptr;
                (*it)->hide();
            }
            pItem = pList->getNext();
        }
        menu2Mgr.selectButton(menu2List.front());
    }else{//menu2
        if (mapMenuToPage.find(pItem) != mapMenuToPage.end()){
            UiPage * page = mapMenuToPage[pItem];
            pageContainer.setContent(page);
        }
    }
}

void MainWindow::onResize(int width,int height)
{
    int l = 90, t = 40, lw=270, lh = 24;
    ui->horizontalLayoutWidget->move(l, 0);
    pageContainer.setParent(this);
    pageContainer.resize(width - l, height - t - lh);
    pageContainer.move(l,t);
    pageContainer.show();
    ui->labelCopyright->move((width - lw)/2, height - lh);
}
void MainWindow::resizeEvent(QResizeEvent *event)
{
    onResize(event->size().width(), event->size().height());
    QWidget::resizeEvent(event);
}
