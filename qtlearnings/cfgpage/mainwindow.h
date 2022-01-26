#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "UiCommon/cwidgetbutton.h"
#include "Util/ItemBase.h"
#include "UiCommon/pagecontainer.h"
#include <map>
#include <list>
using namespace std;
class UiPage;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void slot_button_clicked(CStateButton* pBtn);

protected:
    void initMenu();
    void initPage();
    void onResize(int width, int height);
    virtual void resizeEvent(QResizeEvent *event);

private:
    Ui::MainWindow *ui;
    XmlList                      menuList;
    list<CStateButton*>          menu1List;
    list<CStateButton*>          menu2List;
    map<CStateButton*, XmlItem*> mapBtnToItem;
    CStateButtonMgr              menu1Mgr;
    CStateButtonMgr              menu2Mgr;
    PageContainer                pageContainer;
    list<UiPage*>                pageList;
    map<XmlItem*, UiPage*>       mapMenuToPage;
    unsigned short               paramTable[512];
};
#endif // MAINWINDOW_H
