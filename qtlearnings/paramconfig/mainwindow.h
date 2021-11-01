#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Util/devcfg.h"
#include "UiCommon/cwidgetbutton.h"
#include <list>
#include <vector>
#include <map>
using namespace std;

class UiPage;
class PageContainer;
class QComboBox;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void initMenu();
    void initPage();
    void deleteAll();
    void selectMenu(int menuIdx);
    bool saveParam();
    bool loadParam();
    virtual void resizeEvent(QResizeEvent *event);

protected slots:
    void slot_systemClicked(bool);
    void slot_deviceChanged(int newIdx);
    void slot_menu2Clicked(CStateButton* btn);

private:
    Ui::MainWindow *ui;
    QComboBox      *cmbDevices;
    DevCfgList     devCfg;
    vector<list<CStateButton*>> menu2Lists;
    int                         currMenuIdx;
    vector<CStateButtonMgr*>    menu2MgrList;
    list<UiPage*>               pageList;
    map<CStateButton*, UiPage*> menu2ToPageMap;
    PageContainer              *pageMgr;
    unsigned short             *paramAddr;
    int                         paramCount;
    QString                     appDir;
};
#endif // MAINWINDOW_H
