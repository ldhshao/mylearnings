#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "UiCommon/uipage.h"
#include "UiCommon/pagecontainer.h"
#include "UiCommon/qkeytools.h"
#include "UiCommon/ckeylabel.h"
#include "cdevposctl1.h"
#include "cdevposctl2.h"
#include "clineselector.h"
#include "cdeviceconfig.h"
#include <QComboBox>
#include <QCoreApplication>
#include <fstream>
#include <math.h>
#include <QPixmap>
#include <QBitmap>
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
#define PROPERTY_DEVICE "device"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), devUiCfgList(0)
{
    ui->setupUi(this);
    paramAddr = nullptr;
    paramCount = 0;
    appDir = QCoreApplication::applicationDirPath();
    loadParam();
    initMenu();
    //QkeyTools::getInstance();
    deviceUi = new CDeviceConfig();
    deviceUi->hide();
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
    DevCfgItem *pItem = devCfg.getHead();
    while (nullptr != pItem) {
        CKeyLabel* pBtn = new CKeyLabel(this);
        pBtn->setText(pItem->getName());
        QVariant var;
        var.setValue<void*>(pItem);
        pBtn->setProperty(PROPERTY_DEVICE, var);
        connect(pBtn, SIGNAL(clicked(QLabel*)), this, SLOT(slot_deviceClicked(QLabel*)));
        devList.push_back(pBtn);
        pItem = devCfg.getNext();
    }

    QLabel* pBtn = new QLabel(this);
    pBtn->setText("通讯");
    devList.push_back(pBtn);
    pBtn = new QLabel(this);
    pBtn->setText("帮助");
    devList.push_back(pBtn);

    initPage();
}

void MainWindow::initPage()
{
    GroupCfgList cfgTemplate;
    QString strTemplate = appDir + "/" + UITEMP_CFG_FILEPATH;
    cfgTemplate.readXmlFile(strTemplate);
    devUiCfgList.setParamTbl(paramAddr);
    DevCfgItem *pItem = devCfg.getHead();
    while (nullptr != pItem) {
        GroupCfgList *uiList = new GroupCfgList();
        uiList->setName(pItem->getName());
        DevCfgList* pList = dynamic_cast<DevCfgList*>(pItem);//1level
        if (nullptr != pList){
            DevCfgItem* pSubItem = pList->getHead();//2level
            while(nullptr != pSubItem){
                GroupCfgItem* grp = nullptr;
                GroupCfgItem *pGroup = cfgTemplate.findGroupByName(pSubItem->getType());
                if (nullptr != pGroup){
                    grp = dynamic_cast<GroupCfgItem*>(pGroup->createMyself());
                }else {
                    grp = new GroupCfgList();
                }
                grp->setName(pSubItem->getName());
                uiList->addChild(grp);

                DevCfgList* pLel2Dev = dynamic_cast<DevCfgList*>(pSubItem);
                if (nullptr != pLel2Dev){
                    DevCfgItem* pLel3Item = pLel2Dev->getHead();//3level
                    while(nullptr != pLel3Item){
                        GroupCfgItem* pGrp3 = cfgTemplate.findGroupByName(pLel3Item->getType());
                        if (nullptr != pGrp3){
                            UiCfgItem* item3 = pGrp3->createMyself();
                            item3->setName(pLel3Item->getName());
                            grp->addChild(item3);
                        }
                        pLel3Item = pLel2Dev->getNext();
                    }
                }
                pSubItem = pList->getNext();
            }
        }
        devUiCfgList.addChild(uiList);
        pItem = devCfg.getNext();
    }

    //check data count
    int iCnt = devUiCfgList.getDataCount();
    if (nullptr != paramAddr){
        if (paramCount != iCnt){
            delete[] paramAddr;
            paramAddr = nullptr;
        }
    }
    paramCount = iCnt;
    if(nullptr == paramAddr){
        paramAddr = new uint16_t[paramCount];
        devUiCfgList.setParamTbl(paramAddr);
    }
    devUiCfgList.initData(0, true);//use default value
    devUiCfgList.createAllPage(pageList);

}

void MainWindow::deleteAll()
{
    for(auto it = devList.begin(); it != devList.end(); it++){
        delete (*it);
    }

    delete deviceUi;
    //delete pages
    //list<UiPage*>::iterator itPage = pageList.begin();
    //for (; itPage != pageList.end(); itPage++) {
    //    delete (*it);
    //}
    //pageList.clear();
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
    int t0 = 100, t1 = height - 50, t2 = 200;
    int w0 = ui->pushButton_load->width();
    int w1 = ui->label_copyright->width();
    int s0 = (0.4 * width - w0 * 5) / 4;
    ui->pushButton_load->move(0.3*width, t0);
    ui->pushButton_send->move(0.3*width + w0 + s0, t0);
    ui->pushButton_preview->move(0.3*width + 2*(w0 + s0), t0);
    ui->pushButton_save->move(0.3*width + 3*(w0 + s0), t0);
    ui->pushButton_queryrecord->move(0.3*width + 4*(w0 + s0), t0);
    ui->label_copyright->move((width - w1)/2, t1);

    //layout device
    QPixmap pixmap(":/images/device1.png");
    int cols, cnt = 0;
    cols = sqrt(devCfg.getChildrenCount() + 2);
    if (3 > cols) cols = 3;
    int rows = (devCfg.getChildrenCount() + 2 + cols - 1) / cols;
    int w2 = 200;
    int s2 = (0.4*width - w2 * cols) / (cols - 1);
    qDebug()<<"s2 "<<s2;
    for (auto it = devList.begin(); it!= devList.end(); it++) {
        (*it)->resize(w2, w2);
        (*it)->setPixmap(pixmap.scaled(w2, w2));
        if (cnt / cols < rows - 1 || (0 == (devCfg.getChildrenCount()+2)%cols))
            (*it)->move(0.3*width + (cnt%cols)*(w2+s2), t2 + (cnt/cols)*(w2+s2));
        else {
            int lost = cols - ((devCfg.getChildrenCount()+2)%cols);
            int offset = (lost * (w2 + s2)) / 2;
            (*it)->move(offset + 0.3*width + (cnt%cols)*(w2+s2), t2 + (cnt/cols)*(w2+s2));
        }
        cnt++;
    }
}
void MainWindow::resizeEvent(QResizeEvent *event)
{
    onResize(event->size().width(), event->size().height());
    QMainWindow::resizeEvent(event);
}

void MainWindow::slot_deviceClicked(QLabel* lbl)
{
    QVariant var = lbl->property(PROPERTY_DEVICE);
    DevCfgList* list = static_cast<DevCfgList*>(var.value<void*>());
    qDebug()<<"label "<<lbl<<" device "<<list;

    deviceUi->updateUi(list, &devUiCfgList);
    deviceUi->showUi(1);
}

void MainWindow::on_pushButton_load_clicked()
{

}

void MainWindow::on_pushButton_send_clicked()
{

}

void MainWindow::on_pushButton_preview_clicked()
{

}

void MainWindow::on_pushButton_save_clicked()
{

}
