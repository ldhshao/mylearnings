#ifndef CBAKEUPRESTOREWID_H
#define CBAKEUPRESTOREWID_H

#include "UiCommon/cwidgetbutton.h"
#include <QMainWindow>
#include <vector>
using namespace std;

class QLabel;
class PageCfgList;

namespace Ui {
class CBakeupRestoreWid;
}

class CBakeupRestoreWid : public QMainWindow
{
    Q_OBJECT

public:
    explicit CBakeupRestoreWid(QWidget *parent = nullptr);
    ~CBakeupRestoreWid();

    void showUi();
    void setParamAddr(uint16_t* srvAddr, uint16_t* lclAddr);
    void setDeviceUiCfg(PageCfgList* cfgList) { devUiCfgList = cfgList; }

protected:
    void initMenu();
    void onResize(int width, int height);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

protected slots:
    void slot_menu2_clicked(CStateButton* pBtn);

private:
    Ui::CBakeupRestoreWid *ui;
    vector<CKeyWidgetButton*> menu2List;
    CStateButtonMgr     menu2Mgr;
    QLabel             *bkLbl;
    int                 menuWidth;
    int                 menuHeight;
    PageCfgList        *devUiCfgList;
    uint16_t           *paramSrvAddr;
    uint16_t           *paramLclAddr;
};

#endif // CBAKEUPRESTOREWID_H
