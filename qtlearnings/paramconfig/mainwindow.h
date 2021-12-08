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
class CDeviceConfig;
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
    bool saveParam();
    bool loadParam();
    void onResize(int width, int height);
    virtual void resizeEvent(QResizeEvent *event);

protected slots:
    void slot_deviceClicked(QLabel* lbl);

private slots:
    void on_pushButton_load_clicked();

    void on_pushButton_send_clicked();

    void on_pushButton_preview_clicked();

    void on_pushButton_save_clicked();

private:
    Ui::MainWindow *ui;
    DevCfgList     devCfg;
    PageCfgList    devUiCfgList;
    vector<QLabel*> devList;
    list<UiPage*>               pageList;
    CDeviceConfig              *deviceUi;
    unsigned short             *paramAddr;
    int                         paramCount;
    QString                     appDir;
};
#endif // MAINWINDOW_H
