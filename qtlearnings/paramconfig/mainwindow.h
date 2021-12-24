#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Util/devcfg.h"
#include "Util/PageCfg.h"
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
    QWidget* getCloseWidget(QWidget* wid, bool up);
    int getDeviceCols();
    bool initWorkDir();
    void addModifiedParamIndex(uint32_t idx);
    void connectPages();
    void onResize(int width, int height);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

protected slots:
    //void slot_deviceClicked(QLabel* lbl);
    void slot_deviceClicked(QWidget* w);
    void slot_modifiedParamAddrList(list<uint16_t*> *pMparamAddrList);
    void slot_rollBack_paramAddrList(list<uint16_t*> *pMparamAddrList);
    void slot_emitTimer();
    void slot_closeClicked(QLabel* lbl);

private slots:
    void on_pushButton_load_clicked();

    void on_pushButton_send_clicked();

    void on_pushButton_preview_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_queryrecord_clicked();

private:
    Ui::MainWindow *ui;
    DevCfgList     devCfg;
    PageCfgList    devUiCfgList;
    //vector<QLabel*> devList;
    vector<CDeviceIconWidget*> devList;
    list<UiPage*>               pageList;
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
};
#endif // MAINWINDOW_H
