#ifndef CDEVICECONFIG_H
#define CDEVICECONFIG_H
#include "UiCommon/cwidgetbutton.h"
#include <QMainWindow>
#include <vector>
using namespace std;

class QLabel;
class DevCfgList;
class PageCfgList;
class GroupCfgItem;
class CDevicePreview;

namespace Ui {
class CDeviceConfig;
}

class CDeviceConfig : public QMainWindow
{
    Q_OBJECT

public:
    explicit CDeviceConfig(QWidget *parent = nullptr);
    ~CDeviceConfig();
    void updateUi(DevCfgList* dev, PageCfgList* uiCfg);
    void showUi(int reason);
    CDevicePreview* getPreview() {	return preview; }

protected:
    void initMenu2(GroupCfgItem* grpCfg);
    void showPreview(CStateButton* menu);
    void showPreview(GroupCfgItem* grpCfg);
    void onResize(int width, int height);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);


protected slots:
    void slot_menu2_clicked(CStateButton* pBtn);
    void slot_menu3_clicked(CStateButton* pBtn);
    void slot_menu4_clicked(CStateButton* pBtn);

private:
    Ui::CDeviceConfig *ui;
    DevCfgList*     devCfg;
    PageCfgList*    devUiCfgList;
    vector<CStateButton*> menu2List;
    vector<CStateButton*> menu3List;
    vector<CStateButton*> menu4List;
    CStateButtonMgr     menu2Mgr;
    CStateButtonMgr     menu3Mgr;
    CStateButtonMgr     menu4Mgr;
    int                 menu2Cnt;
    int                 menu3Cnt;
    int                 menu4Cnt;
    CStateButton       *menuExit;
    CDevicePreview     *preview;
    QLabel             *bkLbl;
    int                 menuWidth;
    int                 menuHeight;

};

#endif // CDEVICECONFIG_H
