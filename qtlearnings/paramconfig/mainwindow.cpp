#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "UiCommon/uipage.h"
#include "UiCommon/pagecontainer.h"
#include "UiCommon/qkeytools.h"
#include "UiCommon/ckeylabel.h"
#include "cdevicepreview.h"
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
#include <QDir>
#include <QFileInfo>
#include <QKeyEvent>
#define WORK_FILEPATH "/opt/data/paramconfig"
//#define WORK_FILEPATH "/home/hndz-dhliu"
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
#define PROPERTY_INDEX "index"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), devUiCfgList(0)
{
    ui->setupUi(this);
    paramLocalAddr = nullptr;
    paramServerAddr = nullptr;
    paramCount = 0;
    deviceUi = new CDeviceConfig();
    deviceUi->hide();

    initWorkDir();
    initMenu();
    //QkeyTools::getInstance();
}

MainWindow::~MainWindow()
{
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
        for (auto it = dirList.rbegin(); it != dirList.rend(); it++) {
            if ("." == it->fileName() || ".." == it->fileName()) continue;
            QString strDir(it->fileName());
            if (!temp->exists(strDir + "/" + SYSTEM_CFG_FILEPATH))
                continue;
            qDebug()<<strDir;
            if (!temp->exists(strDir + "/" + UITEMP_CFG_FILEPATH))
                continue;
            if (!temp->exists(strDir + "/" + PARAMS_FILEPATH))
                continue;
            workDir.append("/").append(it->fileName());
            qDebug()<<workDir;
            return true;;
        }
    }

    return false;
}

void MainWindow::initMenu()
{
    qDebug()<<workDir;
    QString strDevCfg = workDir + "/" + SYSTEM_CFG_FILEPATH;
    devCfg.readDevCfgFile(strDevCfg);
    DevCfgItem *pItem = devCfg.getHead();
    int idx = 0;
    while (nullptr != pItem) {
        CKeyLabel* pBtn = new CKeyLabel(this);
        pBtn->setText(pItem->getName());
        pBtn->setFocusPolicy(Qt::StrongFocus);
        QVariant var;
        var.setValue<void*>(pItem);
        pBtn->setProperty(PROPERTY_DEVICE, var);
        pBtn->setProperty(PROPERTY_INDEX, idx++);
        connect(pBtn, SIGNAL(clicked(QLabel*)), this, SLOT(slot_deviceClicked(QLabel*)));
        devList.push_back(pBtn);
        pItem = devCfg.getNext();
    }

    QLabel* pBtn = new CKeyLabel(this);
    pBtn->setText("通讯");
    pBtn->setFocusPolicy(Qt::StrongFocus);
    pBtn->setProperty(PROPERTY_INDEX, idx++);
    devList.push_back(pBtn);
    pBtn = new CKeyLabel(this);
    pBtn->setText("帮助");
    pBtn->setFocusPolicy(Qt::StrongFocus);
    pBtn->setProperty(PROPERTY_INDEX, idx++);
    devList.push_back(pBtn);

    initPage();
}

void MainWindow::initPage()
{
    GroupCfgList cfgTemplate;
    QString strTemplate = workDir + "/" + UITEMP_CFG_FILEPATH;
    cfgTemplate.readXmlFile(strTemplate);
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
    loadParam();

    devUiCfgList.setParamTbl(paramLocalAddr);
    devUiCfgList.initData(0, false);//
    devUiCfgList.createAllPage(pageList);

    connectPages();
}

void MainWindow::connectPages()
{
    for (auto it = pageList.begin(); it !=pageList.end(); it++) {
        QObject::connect(*it, SIGNAL(sig_configFinished()), deviceUi->getPreview(), SLOT(slot_configFinished()));
        QObject::connect(*it, SIGNAL(sig_modifiedParamAddrList(list<uint16_t*> *)), this, SLOT(slot_modifiedParamAddrList(list<uint16_t*> *)));
        QObject::connect(*it, SIGNAL(sig_rollBack_paramAddrList(list<uint16_t*> *)), this, SLOT(slot_rollBack_paramAddrList(list<uint16_t*> *)));
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
    }
    return true;
}
bool MainWindow::loadParam()
{
    bool ret = false;
    if (nullptr == paramLocalAddr){
        ifstream inFile;
        QString strParam = workDir + "/" + PARAMS_FILEPATH;
        inFile.open(strParam.toStdString(), ios_base::in | ios_base::binary);
        inFile.seekg(0, inFile.end);
        paramCount = inFile.tellg() / sizeof(uint16_t);
        int params = devUiCfgList.getDataCount();
        if (paramCount == params){//check param count or config md5
            inFile.seekg(0, inFile.beg);
            paramLocalAddr = new uint16_t[paramCount];
            paramServerAddr = new uint16_t[paramCount];
            inFile.read(reinterpret_cast<char*>(paramLocalAddr), paramCount*sizeof(uint16_t));
            ret = true;
        }else {//get params from remote
        }
        inFile.close();
        memcpy(static_cast<void*>(paramServerAddr),static_cast<void*>(paramLocalAddr),paramCount*sizeof(uint16_t));
    }
    qDebug()<<__FUNCTION__<<" param addr "<<paramLocalAddr<<" param count "<<paramCount<<" result "<<ret;
    return ret;
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
    int cols = getDeviceCols(), cnt = 0;
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

int MainWindow::getDeviceCols()
{
    int cols = sqrt(devList.size());
    if (3 > cols) cols = 3;
    return cols;
}

QWidget* MainWindow::getCloseWidget(QWidget* wid, bool up)
{
    QPushButton* pBtn = dynamic_cast<QPushButton*>(wid);
    QLabel* pLbl = dynamic_cast<QLabel*>(wid);
    int x0 = wid->pos().rx();
    int cols = getDeviceCols();
    int rows = (devList.size() + cols - 1) / cols + 1;
    int row = 0;
    int distMin = 0;
    QWidget *pTarget = nullptr;

    if (nullptr != pBtn){
        row = up? (rows -1) : (1);
    } else if (nullptr != pLbl){
        int idx = pLbl->property(PROPERTY_INDEX).toInt();
        row = idx / cols + 1;
        row = up ? (row -1) : ((row+1)%rows);
    }
    if (0 == row){
        vector<QWidget*> btnList;
        btnList.push_back(ui->pushButton_load);
        btnList.push_back(ui->pushButton_send);
        btnList.push_back(ui->pushButton_preview);
        btnList.push_back(ui->pushButton_save);
        btnList.push_back(ui->pushButton_queryrecord);
        bool init =false;
        for (int i = 0; i < btnList.size(); i++) {
            int dist = abs(btnList[i]->pos().rx() - x0);
            if (!init){
                distMin = dist;
                pTarget = btnList[i];
                init = true;
            }else if (dist < distMin){
                distMin = dist;
                pTarget = btnList[i];
            }
        }
    }else {
        row--;
        bool init =false;
        for (int i = row*cols; i < (row+1)*cols && i < devList.size(); i++) {
            int dist = abs(devList[i]->pos().rx() - x0);
            if (!init){
                distMin = dist;
                pTarget = devList[i];
                init = true;
            } else if (dist < distMin){
                distMin = dist;
                pTarget = devList[i];
            }
        }
    }
    return pTarget;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QPushButton* pBtn = dynamic_cast<QPushButton*>(focusWidget());
    QLabel* pLbl = dynamic_cast<QLabel*>(focusWidget());
    qDebug()<<"MainWindow "<<__FUNCTION__;
    switch (event->key()) {
        case Qt::Key_Return:
            if (nullptr != pBtn){
                if (pBtn == ui->pushButton_load) on_pushButton_load_clicked();
                else if (pBtn == ui->pushButton_send) on_pushButton_send_clicked();
                else if (pBtn == ui->pushButton_preview) on_pushButton_preview_clicked();
                else if (pBtn == ui->pushButton_save) on_pushButton_save_clicked();
                else if (pBtn == ui->pushButton_queryrecord) on_pushButton_queryrecord_clicked();
            }
            if (nullptr != pLbl){
                qDebug()<<__FUNCTION__<<" lbl "<<pLbl;
                slot_deviceClicked(pLbl);
            }
            return ;
        case Qt::Key_Up:
            getCloseWidget(focusWidget(), true)->setFocus();
            return ;
        case Qt::Key_Down:
            getCloseWidget(focusWidget(), false)->setFocus();
            return ;
        case Qt::Key_Left:
            focusNextPrevChild(false);
            return ;
        case Qt::Key_Right:
            focusNextPrevChild(true);
            return ;
    }
}
void MainWindow::slot_deviceClicked(QLabel* lbl)
{
    QVariant var = lbl->property(PROPERTY_DEVICE);
    DevCfgList* list = static_cast<DevCfgList*>(var.value<void*>());
    qDebug()<<"label "<<lbl<<" device "<<list;

    deviceUi->updateUi(list, &devUiCfgList);
    deviceUi->showUi(1);
}

void MainWindow::slot_modifiedParamAddrList(list<uint16_t*> *pMparamAddrList)
{
    qDebug()<<__FUNCTION__<<*pMparamAddrList;
    for (auto it = pMparamAddrList->begin(); it != pMparamAddrList->end(); it++) {
        uint32_t idx = (*it) - paramLocalAddr;
        addModifiedParamIndex(idx);
    }
}

void MainWindow::slot_rollBack_paramAddrList(list<uint16_t*> *pMparamAddrList)
{
    qDebug()<<__FUNCTION__<<*pMparamAddrList;
    for (auto it = pMparamAddrList->begin(); it != pMparamAddrList->end(); it++) {
        uint32_t idx = (*it) - paramLocalAddr;
        qDebug()<<"modified "<<*(*it)<<" old "<<*(paramServerAddr+idx);
        *(*it) = *(paramServerAddr + idx);
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
    qDebug()<<__FUNCTION__;
}

void MainWindow::on_pushButton_send_clicked()
{
    qDebug()<<__FUNCTION__;

}

void MainWindow::on_pushButton_preview_clicked()
{
    qDebug()<<__FUNCTION__;

}

void MainWindow::on_pushButton_save_clicked()
{
    qDebug()<<__FUNCTION__;

}

void MainWindow::on_pushButton_queryrecord_clicked()
{
    qDebug()<<__FUNCTION__;

}
