#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Util/devcfg.h"
#include "Util/PageCfg.h"
#include "Util/logger.h"
#include "UiCommon/cwidgetbutton.h"
#include <list>
#include <vector>
#include <map>
using namespace std;

class UiPage;
class QLabel;
class CKeyLabel;
class CDeviceConfig;
class CDeviceIconWidget;
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
    void initTitle();
    void initMenu();
    void initPage();
    void deleteAll();
    bool saveParam();
    bool loadParam();
    QWidget* getCloseWidget(QWidget* wid, int direction);
    int getDeviceCols();
    bool initWorkDir();
    void initDeviceUiCfg(GroupCfgItem* devItem);
    void getRowsAndCols(int* pRows, int *pCols);
    void onResize(int width, int height);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual bool eventFilter(QObject * watched, QEvent * event);//for arm 4.8.6

    enum {
      DIRECTION_UP = 0,
      DIRECTION_DOWN,
      DIRECTION_LEFT,
      DIRECTION_RIGHT,
      DIRECTION_CNT,
    };
protected slots:
    //void slot_deviceClicked(QLabel* lbl);
    void slot_systemClicked(QWidget* w);
    void slot_deviceClicked(QWidget* w);
    void slot_uploadQueryClicked(QWidget* w);
    void slot_bakeupRestoreClicked(QWidget* w);
    void slot_commClicked(QWidget* w);
    void slot_helpClicked(QWidget* w);
    void slot_rollBack_paramAddrList(list<uint16_t*> *pMparamAddrList);
    void slot_emitTimer();
    void slot_closeClicked(QLabel* lbl);

private:
    Ui::MainWindow *ui;
    DevCfgList     devCfg;
    PageCfgList    devUiCfgList;
    vector<CDeviceIconWidget*> devList;
    CDeviceConfig              *deviceUi;
    unsigned short             *paramLocalAddr;
    unsigned short             *paramServerAddr;
    int                         paramCount;
    QString                     workDir;
    list<uint32_t>              mparamIdxList;
    QLabel                     *topLbl, *titleLbl, *logoLbl, *verLbl, *timeLbl;
    QLabel                     *bkLbl;
    CKeyLabel                  *closeLbl;
    QTimer               	   *emitTimer;
    int                         timerInterval;
    int                         rowMax;
};
#endif // MAINWINDOW_H
