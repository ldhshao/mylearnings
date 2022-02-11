#ifndef CHELPWID_H
#define CHELPWID_H

#include "UiCommon/cwidgetbutton.h"
#include "cmodparampreview.h"
#include <QMainWindow>
#include <vector>
using namespace std;

class QLabel;
class PageCfgList;

namespace Ui {
class CHelpWid;
}

class CHelpWid : public QMainWindow
{
    Q_OBJECT

public:
    explicit CHelpWid(QWidget *parent = nullptr);
    ~CHelpWid();

    void showUi();
    void setDeviceUiCfg(PageCfgList* cfgList) { devUiCfgList = cfgList; }

protected:
    void initMenu();
    void onResize(int width, int height);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual bool eventFilter(QObject * watched, QEvent * event);//for arm 4.8.6

protected slots:
    void slot_menu2_clicked(CStateButton* pBtn);

private:
    Ui::CHelpWid *ui;
    vector<CKeyWidgetButton*> menu2List;
    CStateButtonMgr     menu2Mgr;
    QLabel             *bkLbl;
    int                 menuWidth;
    int                 menuHeight;
    PageCfgList        *devUiCfgList;
    list<SModParamInfoItem>    itemList;
};

#endif // CHELPWID_H
