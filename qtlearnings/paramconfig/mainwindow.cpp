#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "UiCommon/uipage.h"
#include "UiCommon/pagecontainer.h"
#include "UiCommon/qkeytools.h"
#include "UiCommon/ckeylabel.h"
#include "cdevicepreview.h"
#include "cdevposctl2.h"
#include "cdevpointedit.h"
#include "clineselector.h"
#include "cdeviceconfig.h"
#include "cmodparampreview.h"
#include "cmodparamquery.h"
#include "cdeviceiconwidget.h"
#include "cuploadquerywid.h"
#include "cbakeuprestorewid.h"
#include "chelpwid.h"
#include <QComboBox>
#include <QCoreApplication>
#include <fstream>
#include <math.h>
#include <time.h>
#include <QPixmap>
#include <QBitmap>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QPushButton>
#define WORK_FILEPATH "/opt/data/paramconfig"
//#define WORK_FILEPATH "/home/hndz-dhliu"
#define SYSTEM_CFG_FILEPATH "system.json"
#define UITEMP_CFG_FILEPATH "uitemplate.xml"
#define PARAMS_FILEPATH     "param.dat"
#define UITEMPLATE_ELECTRIC     "electric"
#define DEVNAME_ELECTRIC        "电气系统"
#define UITEMPLATE_COMM        "comm"
#define DEVNAME_COMM           "通讯"
#define DEVNAME_PREWARN           "预警"
#define DEVNAME_AUTOTENSION       "自动张紧"
#define DEVNAME_BRAKE             "制动闸"
#define DEVNAME_ASENSOR            "模拟量传感器"
#define DEVNAME_DSENSOR            "开关量传感器"
#define PARNAME_DEVICE1        "HDevice1"
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
#define BTN_STYLE           "font-size:20px;color:rgba(255,255,255,100%);background-color:rgba(80,80,100,100%);"
#define VER_STYLE           "font-size:20px;color:rgba(255,255,255,100%);"
#define TITLE_STYLE         "font:bold 28px;color:rgba(255,255,255,100%);"
#define COPYRIGHT_STYLE     "color:rgba(255,255,255,100%);"
//#define WIDGET_BKCOLOR      "background-color: rgba(12, 33, 107, 0%);"
#define WIDGET_BKCOLOR        "border-image: url(:/images/mainback.png);"
#define PROPERTY_DEVICE "device"
#define PROPERTY_INDEX "index"
#define PROPERTY_IMAGE "image"
#define PROPERTY_WIDGET "widget"
#define PROPERTY_WORKDIR "workdir"
#define PROPERTY_PARCNT  "paramcount"
#define PROPERTY_LOGGER  "logger"

#define make_index(row,col) (((row)<<16)+(col))
#define get_row_from_index(index) ((index)>>16)
#define get_col_from_index(index) ((index)&0xFFFF)


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), devUiCfgList(0)
{
    initTitle();
    setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
    QPixmap pmClose(":/images/close.png");//close connect failed when before setuoUI, unkown reason
    closeLbl = new CKeyLabel(this);
    closeLbl->setFocusPolicy(Qt::ClickFocus);
    closeLbl->resize(32,32);
    closeLbl->setPixmap(pmClose.scaled(28,28));
    connect(closeLbl, SIGNAL(clicked(QLabel*)), this, SLOT(slot_closeClicked(QLabel*)));
    ui->label_copyright->setStyleSheet(COPYRIGHT_STYLE);
    paramLocalAddr = nullptr;
    paramServerAddr = nullptr;
    paramCount = 0;
    rowMax = 5;
    deviceUi = new CDeviceConfig();
    deviceUi->hide();
    timerInterval = 1000;
    emitTimer = new QTimer(this);
    connect(emitTimer, SIGNAL(timeout()), this, SLOT(slot_emitTimer()));

    initWorkDir();
    initMenu();
    //QkeyTools::getInstance();
    CDevPosMgr::instance()->initDevPosMgr(&devUiCfgList);
    emitTimer->start(timerInterval);
}

MainWindow::~MainWindow()
{
    emitTimer->stop();
    saveParam();
    delete []paramLocalAddr;
    delete []paramServerAddr;
    deleteAll();
    delete ui;
}

bool MainWindow::initWorkDir()
{
    workDir = WORK_FILEPATH;
    //查找最新的配置版本
    QDir *temp = new QDir;
    bool exist = temp->exists(workDir);
    if(!exist)
    {
        if (!temp->mkdir(workDir)){
            return false;
        }
        //get newest config data
    }else {
        temp->setPath(workDir);
        QFileInfoList dirList = temp->entryInfoList(QDir::Dirs);
        for (auto it = dirList.begin(); it != dirList.end(); it++) {
            if ("." == it->fileName() || ".." == it->fileName()) continue;
            QString strDir(it->fileName());
            if (!temp->exists(strDir + "/" + SYSTEM_CFG_FILEPATH))
                continue;
            if (!temp->exists(strDir + "/" + UITEMP_CFG_FILEPATH))
                continue;
            //if (!temp->exists(strDir + "/" + PARAMS_FILEPATH))
            //    continue;
            workDir.append("/").append(it->fileName());
            qDebug()<<workDir;
            QCoreApplication::instance()->setProperty(PROPERTY_WORKDIR, workDir);
            return true;;
        }
    }

    return false;
}

void MainWindow::initTitle()
{
    bkLbl = new QLabel(this);
    topLbl = new QLabel(this);
    logoLbl = new QLabel(this);
    titleLbl = new QLabel(u8"KTC236通信控制系统参数设置",this);
    verLbl = new QLabel(u8"版本:VL0.0.0",this);
    timeLbl = new QLabel(this);
    bkLbl->setStyleSheet(WIDGET_BKCOLOR);
    titleLbl->setStyleSheet(TITLE_STYLE);
    verLbl->setStyleSheet(VER_STYLE);
    timeLbl->setStyleSheet(VER_STYLE);
}
void MainWindow::initMenu()
{
    qDebug()<<workDir;
    QString strDevCfg = workDir + "/" + SYSTEM_CFG_FILEPATH;
    bool loaded = devCfg.readDevCfgFile(strDevCfg);
    //qDebug()<<strDevCfg;
    //devCfg.dump();
    DevCfgItem *pItem = devCfg.getHead();
    int idx = 0;
    CDeviceIconWidget* pBtn = new CDeviceIconWidget(this);
    pBtn->setTitle(DEVNAME_ELECTRIC);
    pBtn->setProperty(PROPERTY_IMAGE, ":/images/system.png");
    connect(pBtn, SIGNAL(clicked(QWidget*)), this, SLOT(slot_systemClicked(QWidget*)));
    pBtn->installEventFilter(this);
    devList.push_back(pBtn);

    pBtn = new CDeviceIconWidget(this);
    pBtn->setTitle(DEVNAME_COMM);
    pBtn->setImagePath(":/images/comm.png");
    pBtn->installEventFilter(this);
    devList.push_back(pBtn);
    connect(pBtn, SIGNAL(clicked(QWidget*)), this, SLOT(slot_commClicked(QWidget*)));
    pBtn = new CDeviceIconWidget(this);
    pBtn->setTitle("参数表备份与恢复");
    pBtn->setImagePath(":/images/help.png");
    pBtn->installEventFilter(this);
    devList.push_back(pBtn);
    connect(pBtn, SIGNAL(clicked(QWidget*)), this, SLOT(slot_bakeupRestoreClicked(QWidget*)));
    pBtn = new CDeviceIconWidget(this);
    pBtn->setTitle("参数变更上传与查询");
    pBtn->setImagePath(":/images/help.png");
    connect(pBtn, SIGNAL(clicked(QWidget*)), this, SLOT(slot_uploadQueryClicked(QWidget*)));
    pBtn->installEventFilter(this);
    devList.push_back(pBtn);
    pBtn = new CDeviceIconWidget(this);
    pBtn->setTitle("帮助");
    pBtn->setImagePath(":/images/help.png");
    connect(pBtn, SIGNAL(clicked(QWidget*)), this, SLOT(slot_helpClicked(QWidget*)));
    pBtn->installEventFilter(this);
    devList.push_back(pBtn);

    while (nullptr != pItem) {
        if (!pItem->getName().isEmpty()){
            CDeviceIconWidget* pBtn = new CDeviceIconWidget(this);
            pBtn->setTitle(pItem->getName());
            if (pItem->getName() == "破碎机")
                pBtn->setProperty(PROPERTY_IMAGE, ":/images/crusher.png");
            else
                pBtn->setProperty(PROPERTY_IMAGE, ":/images/system.png");
            QVariant var;
            var.setValue<void*>(pItem);
            pBtn->setProperty(PROPERTY_DEVICE, var);
            connect(pBtn, SIGNAL(clicked(QWidget*)), this, SLOT(slot_deviceClicked(QWidget*)));
            pBtn->installEventFilter(this);
            devList.push_back(pBtn);
        }
        pItem = devCfg.getNext();
    }
    for (idx =devList.size(); idx < 20; idx++) {
        CDeviceIconWidget* pBtn = new CDeviceIconWidget(this);
        pBtn->setTitle("test");
        pBtn->setImagePath(":/images/system.png");
        pBtn->installEventFilter(this);
        devList.push_back(pBtn);
    }

    initPage();
}

void MainWindow::initPage()
{
    GroupCfgList cfgTemplate;
    QString strTemplate = workDir + "/" + UITEMP_CFG_FILEPATH;
    bool loaded = cfgTemplate.readXmlFile(strTemplate);
    {
    //add electrics
        GroupCfgItem *pGroup = cfgTemplate.findGroupByName(UITEMPLATE_ELECTRIC);
        if (nullptr != pGroup){
            GroupCfgItem *grp = dynamic_cast<GroupCfgItem*>(pGroup->createMyself());
            grp->setName(DEVNAME_ELECTRIC);
            devUiCfgList.addChild(grp);
        }
    //add comm
        pGroup = cfgTemplate.findGroupByName(UITEMPLATE_COMM);
        if (nullptr != pGroup){
            GroupCfgItem *grp = dynamic_cast<GroupCfgItem*>(pGroup->createMyself());
            grp->setName(DEVNAME_COMM);
            devUiCfgList.addChild(grp);
        }
    }
    DevCfgItem *pItem = devCfg.getHead();
    while (nullptr != pItem) {
        GroupCfgList *uiList = new GroupCfgList();
        uiList->setName(pItem->getName());
        uiList->setParamName(pItem->paramName());
        DevCfgList* pList = dynamic_cast<DevCfgList*>(pItem);//1level
        if (nullptr != pList){
            GroupCfgItem* grp = nullptr;
            GroupCfgItem *pGroup = cfgTemplate.findGroupByName(pList->getType());
            if (nullptr != pGroup){
                grp = dynamic_cast<GroupCfgItem*>(pGroup->createMyself());
            }else {
                grp = new GroupCfgList();
            }
            grp->setName("基本信息");
            uiList->addChild(grp);
            GroupCfgList *preWarningList = new GroupCfgList();
            GroupCfgList *autoTensionList = new GroupCfgList();
            GroupCfgList *brakingList = new GroupCfgList();
            GroupCfgList *analogSensorList = new GroupCfgList();
            GroupCfgList *switchSensorList = new GroupCfgList();
            GroupCfgList *motorSensorList = new GroupCfgList();
            GroupCfgList *motorGrp = nullptr;
            DevCfgItem* pSubItem = pList->getHead();//2level
            while(nullptr != pSubItem){
                QString dev2Type = pSubItem->getType();
                pGroup = cfgTemplate.findGroupByName(dev2Type);
                if (nullptr != pGroup){
                    grp = dynamic_cast<GroupCfgItem*>(pGroup->createMyself());
                }else {
                    //grp = new GroupCfgList();
                    grp = nullptr;
                }
                if (nullptr != grp){
                    grp->setName(pSubItem->getName());
                    grp->setParamName(pSubItem->paramName());
                    if (DevCfgItem::Dev2TypePrewarning == dev2Type){
                        preWarningList->addChild(grp);
                    }else if (DevCfgItem::Dev2TypeAutoTension == dev2Type){
                        autoTensionList->addChild(grp);
                    }else if (DevCfgItem::Dev2TypeBrake == dev2Type){
                        brakingList->addChild(grp);
                    }else if (DevCfgItem::Dev2TypeSensor == dev2Type){
                        switchSensorList->addChild(grp);
                    }else if (0 < dev2Type.indexOf("motorsensor")){
                        grp->setDeviceLevel(1);
                        motorSensorList->addChild(grp);
                    }else if (0 < dev2Type.indexOf("sensor")){
                        analogSensorList->addChild(grp);
                    }else{
                        uiList->addChild(grp);
                    }

                    if (DevCfgItem::Dev2TypeMotor == dev2Type)
                        motorGrp = dynamic_cast<GroupCfgList*>(grp);
                }

                pSubItem = pList->getNext();
            }
            if (0 < preWarningList->getChildrenCount()){
                preWarningList->setName(DEVNAME_PREWARN);
                uiList->addChild(preWarningList);
            }else{
                delete preWarningList;
            }
            if (0 < autoTensionList->getChildrenCount()){
                autoTensionList->setName(DEVNAME_AUTOTENSION);
                uiList->addChild(autoTensionList);
            }else{
                delete autoTensionList;
            }
            if (0 < brakingList->getChildrenCount()){
                brakingList->setName(DEVNAME_BRAKE);
                uiList->addChild(brakingList);
            }else{
                delete brakingList;
            }
            if (0 < switchSensorList ->getChildrenCount()){
                switchSensorList ->setName(DEVNAME_DSENSOR);
                uiList->addChild(switchSensorList );
            }else{
                delete switchSensorList;
            }
            if (0 < analogSensorList ->getChildrenCount()){
                analogSensorList ->setName(DEVNAME_ASENSOR);
                uiList->addChild(analogSensorList );
            }else{
                delete analogSensorList;
            }
            if (0 < motorSensorList->getChildrenCount()){
                //put motor sensor under motor
                if (nullptr != motorGrp){
                    int motorIdx = 0;
                    UiCfgItem* motorCfg = motorGrp->getHead();
                    while (nullptr != motorCfg){
                        GroupCfgList* pMotor = dynamic_cast<GroupCfgList*>(motorCfg);
                        if (nullptr != pMotor){
                            UiCfgItem* pSensorItem = motorSensorList->getHead();
                            while(nullptr != pSensorItem){
                                QString strName("motor");
                                strName.append(QString::number(motorIdx));
                                if (-1 < pSensorItem->paramName().indexOf(strName))
                                    pMotor->addChild(pSensorItem);
                                pSensorItem = motorSensorList->getNext();
                            }
                            motorIdx++;
                        }
                        motorCfg = motorGrp->getNext();
                    }
                }
                motorSensorList->clear();
            }
            delete motorSensorList;
        }
        devUiCfgList.addChild(uiList);
        pItem = devCfg.getNext();
    }

    paramCount = devUiCfgList.getDataCount();
    paramLocalAddr = new uint16_t[paramCount];
    paramServerAddr = new uint16_t[paramCount];
    QCoreApplication::instance()->setProperty(PROPERTY_PARCNT, paramCount);
    QVariant varDevice;
    varDevice.setValue<void*>(&devUiCfgList);
    QCoreApplication::instance()->setProperty(PROPERTY_DEVICE, varDevice);//set global device
    devUiCfgList.setParamTbl(paramLocalAddr);
    devUiCfgList.initData(0, true);//
    //check data count
    loaded = loadParam();

    qWarning()<<"param address "<<paramLocalAddr<<" param count "<<paramCount;
    memcpy(paramServerAddr, paramLocalAddr, paramCount*sizeof(uint16_t));
    devUiCfgList.createAllPage(pageList);

    connectPages();
    deviceUi->setParamAddr(paramServerAddr, paramLocalAddr);
}

void MainWindow::connectPages()
{
    for (auto it = pageList.begin(); it !=pageList.end(); it++) {
        QObject::connect(*it, SIGNAL(sig_configFinished()), deviceUi->getPreview(), SLOT(slot_configFinished()));
        QObject::connect(*it, SIGNAL(sig_modifiedParamAddrList(list<uint16_t*> *)), this, SLOT(slot_modifiedParamAddrList(list<uint16_t*> *)));
        QObject::connect(*it, SIGNAL(sig_rollBack_paramAddrList(list<uint16_t*> *)), this, SLOT(slot_rollBack_paramAddrList(list<uint16_t*> *)));
        //qDebug()<<"bind uipage "<<*it;
    }
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
    if (nullptr != paramLocalAddr){
       ofstream outFile;
       QString strParam = workDir + "/" + PARAMS_FILEPATH;
       outFile.open(strParam.toStdString(), ios_base::out|ios_base::binary);
       outFile.write(reinterpret_cast<char*>(paramLocalAddr), paramCount*sizeof (uint16_t));
       outFile.close();

       UiCfgItem* pItem = devUiCfgList.getHead();
       while (nullptr != pItem){
           GroupCfgItem* grp = dynamic_cast<GroupCfgItem*>(pItem);
           grp->saveJsonFile(workDir + "/he.json");
           pItem = devUiCfgList.getNext();
           break;
       }
       while (nullptr != pItem){
           GroupCfgItem* grp = dynamic_cast<GroupCfgItem*>(pItem);
           grp->saveJsonFile(workDir + "/hc.json");
           pItem = devUiCfgList.getNext();
           break;
       }
       int i = 0;
       while (nullptr != pItem){
           GroupCfgItem* grp = dynamic_cast<GroupCfgItem*>(pItem);
           grp->saveJsonFile(workDir + QString("/hp").append(QString::number(i)).append(".json"));
           pItem = devUiCfgList.getNext();
           i++;
       }

       //save to /opt/data/paramconfig/
       QString strDir("/opt/data/paramconfig");
       pItem = devUiCfgList.getHead();
       while (nullptr != pItem){
           GroupCfgItem* grp = dynamic_cast<GroupCfgItem*>(pItem);
           grp->saveJsonFile(strDir + "/he.json");
           pItem = devUiCfgList.getNext();
           break;
       }
       while (nullptr != pItem){
           GroupCfgItem* grp = dynamic_cast<GroupCfgItem*>(pItem);
           grp->saveJsonFile(strDir + "/hc.json");
           pItem = devUiCfgList.getNext();
           break;
       }
       i = 0;
       while (nullptr != pItem){
           GroupCfgItem* grp = dynamic_cast<GroupCfgItem*>(pItem);
           grp->saveJsonFile(strDir + QString("/hp").append(QString::number(i)).append(".json"));
           pItem = devUiCfgList.getNext();
           i++;
       }
    }
    return true;
}
bool MainWindow::loadParam()
{
    bool ret = false;
    UiCfgItem* pItem = devUiCfgList.getHead();
    while (nullptr != pItem){
        GroupCfgItem* grp = dynamic_cast<GroupCfgItem*>(pItem);
        grp->readJsonFile(workDir + "/he.json");
        pItem = devUiCfgList.getNext();
        break;
    }
    while (nullptr != pItem){
        GroupCfgItem* grp = dynamic_cast<GroupCfgItem*>(pItem);
        grp->readJsonFile(workDir + "/hc.json");
        pItem = devUiCfgList.getNext();
        break;
    }
    int i = 0;
    while (nullptr != pItem){
        GroupCfgItem* grp = dynamic_cast<GroupCfgItem*>(pItem);
        grp->readJsonFile(workDir + QString("/hp").append(QString::number(i)).append(".json"));
        pItem = devUiCfgList.getNext();
        i++;
    }
    return ret;
}
void MainWindow::getRowsAndCols(int* pRows, int *pCols)
{
    int cnt = devList.size();
    int cols = 5, rows = (cnt + cols - 1)/cols;
    if (rows >= rowMax){
        rows = 5;
        cols = (cnt-rows) / (rows - 1) + 1;
    }

    *pRows = rows;
    *pCols = cols;
}

void MainWindow::onResize(int width, int height)
{
    int t1 = height - 50;
    int th = 154, logol = 10, verl = 300;
    int w1 = ui->label_copyright->width();
    int lW = 204, lH = 41;
    QPixmap pmTop(":/images/top.png");
    QPixmap pmLogo(":/images/logo.png");
    bkLbl->resize(width, height);
    bkLbl->move(0,0);
    topLbl->resize(width, th);
    topLbl->setPixmap(pmTop.scaled(width, th));
    topLbl->move(0,0);
    logoLbl->resize(lW, lH);
    logoLbl->setPixmap(pmLogo);
    logoLbl->move(logol, 0);
    closeLbl->move(width - 50, 10);
    int tWidth = QFontMetrics(titleLbl->font()).width(titleLbl->text());
    int tHeight = QFontMetrics(titleLbl->font()).height();
    titleLbl->resize(tWidth, tHeight);
    tWidth = QFontMetrics(verLbl->font()).width(verLbl->text());
    tHeight = QFontMetrics(verLbl->font()).height();
    verLbl->resize(tWidth, tHeight);
    titleLbl->move((width - titleLbl->width())/2,20);
    verLbl->move(verl, 30);
    timeLbl->resize(200, 30);
    timeLbl->move(width - 450, 30);
    ui->label_copyright->move((width - w1)/2, t1);

    //layout device
    int rowMax = 5;
    int cols = 0, rows = 0, cnt = 0;
    int m2 = 80, s2 = 20, t2 = th;
    getRowsAndCols(&rows, &cols);

    int w2 = (width - 2*m2 - (cols -1)*s2) / cols;
    int h2 = (height - t2 - (height - t1) - m2 - (rows - 1)*s2) / rows;
    //qDebug()<<"w2 "<<w2<<"h2"<<h2;
    if (rowMax > rows){
        for (auto it = devList.begin(); it!= devList.end(); it++) {
            (*it)->resize(w2, h2);
            (*it)->move(m2 + (cnt%cols)*(w2+s2), t2 + (cnt/cols)*(h2+s2));
            (*it)->setProperty(PROPERTY_INDEX, make_index(cnt/cols, cnt%cols));
            cnt++;
        }
    }else {
        auto it = devList.begin();
        //layout first col
        for (; it!= devList.end() && cnt<rowMax; it++) {
            (*it)->resize(w2, h2);
            (*it)->move(m2, t2 + cnt*(h2+s2));
            (*it)->setProperty(PROPERTY_INDEX, make_index(cnt, 0));
            cnt++;
        }
        //layout rest, by row
        cnt = 0;
        for (; it!= devList.end(); it++) {
            (*it)->resize(w2, h2);
            (*it)->move(m2 + (cnt%(cols-1) + 1)*(w2+s2), t2 + (cnt/(cols-1))*(h2+s2));
            (*it)->setProperty(PROPERTY_INDEX, make_index(cnt/(cols-1), cnt%(cols-1)+1));
            cnt++;
        }
    }
}
void MainWindow::resizeEvent(QResizeEvent *event)
{
    onResize(event->size().width(), event->size().height());
    QMainWindow::resizeEvent(event);
}

int MainWindow::getDeviceCols()
{
    int cols = sqrt(devList.size());
    if (3 > cols) cols = 3;
    return cols;
}

QWidget* MainWindow::getCloseWidget(QWidget* wid, int direction)
{
    CDeviceIconWidget* pBtn = dynamic_cast<CDeviceIconWidget*>(wid);
    if (nullptr != pBtn && DIRECTION_UP <= direction && direction < DIRECTION_CNT){
        int index = pBtn->property(PROPERTY_INDEX).toInt(), nIdx;
        int row = get_row_from_index(index);
        int col = get_col_from_index(index);
        int cnt = devList.size();
        int cols = 0, rows = 0, currCols;
        getRowsAndCols(&rows, &cols);
        switch (direction) {
        case DIRECTION_UP:
            row = (row - 1 + rows) % rows;
            if (row * cols + col >= cnt)
                row = (row - 1 + rows) % rows;
            break;
        case DIRECTION_DOWN:
            row = (row + 1) % rows;
            if (row * cols + col >= cnt)
                row = (row + 1) % rows;
            break;
        case DIRECTION_LEFT:
            currCols = cols;
            if (row == rows -1 && 0 != cnt % cols) currCols = cnt % cols;
            col = (col - 1 + currCols) % currCols;
            break;
        case DIRECTION_RIGHT:
            currCols = cols;
            if (row == rows -1 && 0 != cnt % cols) currCols = cnt % cols;
            col = (col + 1) % currCols;
            break;
        }

        nIdx = make_index(row, col);
        for (auto it = devList.begin(); it != devList.end(); it++) {
            int tIdx = (*it)->property(PROPERTY_INDEX).toInt();
            if (tIdx == nIdx)
                return *it;
        }
    }

    return wid;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //qDebug()<<"MainWindow "<<__FUNCTION__<<event->key();
    switch (event->key()) {
        case Qt::Key_Up:
            getCloseWidget(focusWidget(), DIRECTION_UP)->setFocus();
            return ;
        case Qt::Key_Down:
            getCloseWidget(focusWidget(), DIRECTION_DOWN)->setFocus();
            return ;
        case Qt::Key_Left:
            getCloseWidget(focusWidget(), DIRECTION_LEFT)->setFocus();
            return ;
        case Qt::Key_Right:
            getCloseWidget(focusWidget(), DIRECTION_RIGHT)->setFocus();
            return ;
        case Qt::Key_Escape:
            if (!closeLbl->hasFocus())
                closeLbl->setFocus();
            else
                devList[0]->setFocus();
            return ;
    }
}
bool MainWindow::eventFilter(QObject * watched, QEvent * event)
{
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
    if (nullptr != dynamic_cast<CDeviceIconWidget*>(watched)) {
        CDeviceIconWidget* wid = dynamic_cast<CDeviceIconWidget*>(watched);
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            int key = keyEvent->key();
            if (Qt::Key_Return == key)
                return false;
            else{
                keyPressEvent(keyEvent);
                return true;
            }
        } else {
            return false;
        }
    } else {
        return QMainWindow::eventFilter(watched, event);
    }
#else
    return QMainWindow::eventFilter(watched, event);
#endif
}
void MainWindow::slot_systemClicked(QWidget* w)
{
        deviceUi->updateUi(devUiCfgList.getHead()->getName(), &devUiCfgList);
        deviceUi->showUi(1);
}
void MainWindow::slot_deviceClicked(QWidget* w)
{
    QVariant var = w->property(PROPERTY_DEVICE);
    if (var.isValid()){
        DevCfgList* list = static_cast<DevCfgList*>(var.value<void*>());
        //qDebug()<<" device "<<list;

        deviceUi->updateUi(list->getName(), &devUiCfgList);
        deviceUi->showUi(1);
    }
}

void MainWindow::slot_uploadQueryClicked(QWidget* w)
{
    CUploadQueryWid* pWid = static_cast<CUploadQueryWid*>(w->property(PROPERTY_WIDGET).value<void*>());
    if (nullptr == pWid){
        CUploadQueryWid* wid = new CUploadQueryWid();
        wid->setDeviceUiCfg(&devUiCfgList);
        wid->setParamAddr(paramServerAddr, paramLocalAddr);
        QVariant var;
        var.setValue<void*>(wid);
        w->setProperty(PROPERTY_WIDGET, var);
        wid->resize(width(), height());
        wid->move(0,0);
        pWid = wid;
    }

    if (nullptr != pWid){
        pWid->showUi();
    }
}

void MainWindow::slot_bakeupRestoreClicked(QWidget* w)
{
    CBakeupRestoreWid* pWid = static_cast<CBakeupRestoreWid*>(w->property(PROPERTY_WIDGET).value<void*>());
    if (nullptr == pWid){
        CBakeupRestoreWid* wid = new CBakeupRestoreWid();
        wid->setDeviceUiCfg(&devUiCfgList);
        wid->setParamAddr(paramServerAddr, paramLocalAddr);
        QVariant var;
        var.setValue<void*>(wid);
        w->setProperty(PROPERTY_WIDGET, var);
        wid->resize(width(), height());
        wid->move(0,0);
        pWid = wid;
    }

    if (nullptr != pWid){
        pWid->showUi();
    }
}
void MainWindow::slot_commClicked(QWidget* w)
{
    deviceUi->updateUi(DEVNAME_COMM, &devUiCfgList);
    deviceUi->showUi(1);
}

void MainWindow::slot_helpClicked(QWidget* w)
{
    CHelpWid* pWid = static_cast<CHelpWid*>(w->property(PROPERTY_WIDGET).value<void*>());
    if (nullptr == pWid){
        CHelpWid* wid = new CHelpWid();
        wid->setDeviceUiCfg(&devUiCfgList);
        QVariant var;
        var.setValue<void*>(wid);
        w->setProperty(PROPERTY_WIDGET, var);
        wid->resize(width(), height());
        wid->move(0,0);
        pWid = wid;
    }

    if (nullptr != pWid){
        pWid->showUi();
    }
}

void MainWindow::slot_modifiedParamAddrList(list<uint16_t*> *pMparamAddrList)
{
    //qDebug()<<__FUNCTION__<<*pMparamAddrList;
    for (auto it = pMparamAddrList->begin(); it != pMparamAddrList->end(); it++) {
        uint32_t idx = (*it) - paramLocalAddr;
        addModifiedParamIndex(idx);
    }
}

void MainWindow::slot_rollBack_paramAddrList(list<uint16_t*> *pMparamAddrList)
{
    //qDebug()<<__FUNCTION__<<*pMparamAddrList;
    for (auto it = pMparamAddrList->begin(); it != pMparamAddrList->end(); it++) {
        uint32_t idx = (*it) - paramLocalAddr;
        //deal device point
        UiCfgItem *pItem = devUiCfgList.findItemByDataIdx(idx);
        if (nullptr != pItem && pItem->isType(UiCfgItem::strTypeDevPointEdit)){
            uint32_t devPt = (*(paramServerAddr + idx + 1) << 16) + *(paramServerAddr + idx);
            CDevPointEdit* pEdit = dynamic_cast<CDevPointEdit*>(pItem->getWidget());
            pEdit->setValue(devPt);
            it++;
        }else {
            *(*it) = *(paramServerAddr + idx);
        }
        //qDebug()<<"modified "<<*(*it)<<" old "<<*(paramServerAddr+idx);
    }
}

void MainWindow::addModifiedParamIndex(uint32_t idx)
{
    auto it = mparamIdxList.begin();
    for (; it != mparamIdxList.end(); it++) {
        if (*it == idx)
            break;
    }

    if (it == mparamIdxList.end())
        mparamIdxList.push_back(idx);
}

void MainWindow::on_pushButton_load_clicked()
{
    //qDebug()<<__FUNCTION__;
    //获取应用程序的路径
    QString dlgTitle="选择一个文件"; //对话框标题
    QString filter="数据文件(*.dat);;所有文件(*.*)"; //文件过滤器
    QString fileName=QFileDialog::getOpenFileName(this,dlgTitle,workDir,filter);
    if (!fileName.isEmpty()){
        //qDebug()<<fileName;
        ifstream inFile;
        int count = 0;
        inFile.open(fileName.toStdString(), ios_base::in | ios_base::binary);
        inFile.seekg(0, inFile.end);
        int len = inFile.tellg();
        char * buf = new char [len];
        if (0 != len%6) qDebug()<<"error: wrond modified param data with len "<<len;

        mparamIdxList.clear();
        inFile.seekg(0, inFile.beg);
        inFile.read (buf,len);
        for (int i = 0; i < len; i+= 6) {
            uint32_t *pIdx = static_cast<uint32_t*>((void*)(buf+i));
            uint16_t *pVal = static_cast<uint16_t*>((void*)(buf+i+4));
            qDebug()<<*pIdx<<*pVal;
            *(paramLocalAddr + *pIdx) = *pVal;
            mparamIdxList.push_back(*pIdx);
        }

        inFile.close();
        delete [] buf;
        //all page update
        //以下语句在加载变更表后会引起关闭异常，原因待确定 2021.12.15
        for(auto it = pageList.begin(); it != pageList.end(); it++){
            (*it)->updateUi();
        }
    }
}

void MainWindow::on_pushButton_send_clicked()
{
    qDebug()<<__FUNCTION__;

}

void MainWindow::on_pushButton_preview_clicked()
{
    qDebug()<<__FUNCTION__;
    //if (mparamIdxList.empty()){
    //    QMessageBox msgBox(QMessageBox::Information, "通知", QString("参数未修改:"));
    //    msgBox.addButton("确定", QMessageBox::AcceptRole);
    //    msgBox.exec();
    //    return ;
    //}

    list<UiCfgItem*> itemList;
    for (auto it = mparamIdxList.begin(); it != mparamIdxList.end(); it++) {
        UiCfgItem* item = devUiCfgList.findItemByDataIdx(*it);
        if (nullptr != item)
            itemList.push_back(item);
    }

    //qDebug()<<itemList;
    //CModParamPreview dlg(&itemList);
    //if (QDialog::Accepted == dlg.exec()){
    //    qDebug()<<mparamIdxList;
    //    mparamIdxList.clear();
    //    for (auto it = itemList.begin(); it != itemList.end(); it++) {
    //        uint32_t idx = static_cast<uint32_t>((*it)->dataidx() + (*it)->parent()->dataidx());
    //        int cnt = (*it)->datacount();
    //        while(cnt-- > 0)
    //            mparamIdxList.push_back(idx++);
    //    }

    //    qDebug()<<mparamIdxList;
    //}
}

void MainWindow::on_pushButton_save_clicked()
{
    qDebug()<<__FUNCTION__;
    //save params modified
    if (!mparamIdxList.empty()){
        ofstream outFile;
        QString strParam = workDir + "/";
        time_t rawtime;
        struct tm * timeinfo;
        char strTemp[100] = {0};
        //qDebug()<<mparamIdxList;

        time (&rawtime);
        timeinfo = localtime (&rawtime);
        sprintf(strTemp, "%d%02d%02d%02d%02d%02dmodified.dat",
                 timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        strParam.append(strTemp);
        outFile.open(strParam.toStdString(), ios_base::out|ios_base::binary);
        char *buf = new char[6*mparamIdxList.size()];
        int i = 0;
        for(auto it = mparamIdxList.begin(); it != mparamIdxList.end(); it++){
            uint32_t* pIdx = static_cast<uint32_t*>((void*)(buf+i));
            uint16_t* pVal = static_cast<uint16_t*>((void*)(buf + i+4));
            *pIdx = *it;
            *pVal = *(paramLocalAddr + *it);
            i += 6;
        }
        outFile.write(buf, i);
        outFile.close();
        qDebug()<<buf<<" len "<<i;
        //notify to user file path
        QMessageBox msgBox(QMessageBox::Information, "通知", QString("文件已保存到:").append(strParam));
        msgBox.addButton("确定", QMessageBox::AcceptRole);
        msgBox.exec();
    }
}

void MainWindow::on_pushButton_queryrecord_clicked()
{
    qDebug()<<__FUNCTION__;
    CModParamQuery dlg;
    //Dialog dlg;
    //dlg.exec();
}

void MainWindow::slot_emitTimer()
{
    emitTimer->stop();
    QString strTime;
        time_t rawtime;
        struct tm * timeinfo;
        char strTemp[100] = {0};
        time (&rawtime);
        timeinfo = localtime (&rawtime);
        sprintf(strTemp, "%d-%02d-%02d %02d:%02d:%02d",
                 timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        strTime.append(strTemp);
        timeLbl->setText(strTime);
    emitTimer->start(timerInterval);
}

void MainWindow::slot_closeClicked(QLabel* lbl)
{
    qDebug()<<__FUNCTION__;
    //if modified, tip ""
    if (0){
        ;
        return ;
    }

    close();
}
