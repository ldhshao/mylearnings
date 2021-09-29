#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "pagecontainer.h"

#define MENU2_COUNTMAX   8

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
    void initMenu2(QStringList *pMenuList);

private slots:
    void slot_btnClicked(CStateButton* pBtn);
    void on_btnSystem_clicked();

    void on_btnWork_clicked();

    void on_btnBelt_clicked();

private:
    Ui::MainWindow *ui;
    CStateButton   *pBtnMenus[MENU2_COUNTMAX];
    CStateButtonMgr fstButtonMgr;
    CStateButtonMgr scdButtonMgr;
    PageContainer   *pageContainer;
};
#endif // MAINWINDOW_H
