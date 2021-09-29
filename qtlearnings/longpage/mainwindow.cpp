#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pageposuiji.h"
#include "splitertest.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pageContainer = new PageContainer(this);

    fstButtonMgr.registerButton(ui->btnSystem);
    fstButtonMgr.registerButton(ui->btnWork);
    fstButtonMgr.registerButton(ui->btnBelt);
    pBtnMenus[0] = ui->btnScd1;
    pBtnMenus[1] = ui->btnScd2;
    pBtnMenus[2] = ui->btnScd3;
    pBtnMenus[3] = ui->btnScd4;
    pBtnMenus[4] = ui->btnScd5;
    pBtnMenus[5] = ui->btnScd6;
    pBtnMenus[6] = ui->btnScd7;
    pBtnMenus[7] = ui->btnScd8;
    for (int i =0; i < MENU2_COUNTMAX; i++){
        pBtnMenus[i]->setVisible(false);
        scdButtonMgr.registerButton(pBtnMenus[i]);
    }

    connect(&scdButtonMgr, SIGNAL(sig_button_clicked(CStateButton*)), this, SLOT(slot_btnClicked(CStateButton*)));
    //set page container position
    int w = width(), h=height();
    pageContainer->resize(w - 100, h - 40);
    pageContainer->move(100, 40);

    on_btnSystem_clicked();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete pageContainer;
}

void MainWindow::initMenu2(QStringList *pMenuList)
{
    for (int i =0; i < MENU2_COUNTMAX; i++)
        pBtnMenus[i]->setVisible(false);

    int i = 0, iCnt = pMenuList->count();
    for (; i < iCnt; i++){
        pBtnMenus[i]->setText((*pMenuList)[i]);
        pBtnMenus[i]->setVisible(true);
    }
}

void MainWindow::on_btnSystem_clicked()
{
    QStringList menuList = {
        "系统配置",
        "运行参数",
        "水电联动",
    };
    initMenu2(&menuList);
    fstButtonMgr.selectButton(ui->btnSystem);
    scdButtonMgr.selectButton(pBtnMenus[0]);
}

void MainWindow::on_btnWork_clicked()
{
    QStringList menuList = {
        "基本参数",
        "破碎机",
        "转载机",
        "前溜",
        "后溜"
    };
    initMenu2(&menuList);
    fstButtonMgr.selectButton(ui->btnWork);
    scdButtonMgr.selectButton(pBtnMenus[0]);
}

void MainWindow::on_btnBelt_clicked()
{
    QStringList menuList = {
        "1#皮带",
        "2#皮带",
        "3#皮带",
        "4#皮带",
        "5#皮带",
        "6#皮带",
        "7#皮带",
        "8#皮带",
    };
    initMenu2(&menuList);
    fstButtonMgr.selectButton(ui->btnBelt);
    scdButtonMgr.selectButton(pBtnMenus[0]);
}

void MainWindow::slot_btnClicked(CStateButton* pBtn)
{
    if (ui->btnWork == fstButtonMgr.currentButton() && ui->btnScd2 == scdButtonMgr.currentButton()){
        PagePosuiji * page = new  PagePosuiji(pageContainer);
        pageContainer->setContent(page);
        //pageContainer.addWidgetLink()
    }else if (ui->btnSystem == fstButtonMgr.currentButton() && ui->btnScd2 == scdButtonMgr.currentButton()){
        SpliterTest *page = new SpliterTest(pageContainer);
        pageContainer->setContent(page);
    }else{
        pageContainer->clear();
    }
}
