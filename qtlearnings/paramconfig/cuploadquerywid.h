#ifndef CUPLOADQUERYWID_H
#define CUPLOADQUERYWID_H

#include "UiCommon/cwidgetbutton.h"
#include <QMainWindow>
#include <vector>
using namespace std;

class QLabel;
class PageCfgList;
class UiCfgItem;
namespace Ui {
class CUploadQueryWid;
}

class CUploadQueryWid : public QMainWindow
{
    Q_OBJECT

public:
    explicit CUploadQueryWid(QWidget *parent = nullptr);
    ~CUploadQueryWid();

    void showUi();
    void setParamAddr(uint16_t* srvAddr, uint16_t* lclAddr) { paramSrvAddr = srvAddr; paramLclAddr = lclAddr; }
    void setDeviceUiCfg(PageCfgList* cfgList) { devUiCfgList = cfgList; }

protected:
    void initMenu();
    void onResize(int width, int height);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

protected slots:
    void slot_menu2_clicked(CStateButton* pBtn);

private:
    Ui::CUploadQueryWid *ui;
    vector<CKeyWidgetButton*> menu2List;
    CStateButtonMgr     menu2Mgr;
    QLabel             *bkLbl;
    int                 menuWidth;
    int                 menuHeight;
    PageCfgList        *devUiCfgList;
    uint16_t           *paramSrvAddr;
    uint16_t           *paramLclAddr;
    list<UiCfgItem*>    itemList;
};

#endif // CUPLOADQUERYWID_H
