#ifndef CDEVICEADRTBLWID_H
#define CDEVICEADRTBLWID_H

#include "csingledevadrtblwid.h"
#include <QWidget>

class QLabel;
class PageCfgList;
namespace Ui {
class CDeviceAdrTblWid;
}

class CDeviceAdrTblWid : public QWidget
{
    Q_OBJECT

public:
    explicit CDeviceAdrTblWid(QWidget *parent = nullptr);
    ~CDeviceAdrTblWid();

    void initUi();

protected:
    void updateWidData();
    void updateWidTitle();
    void onResize(int width, int height);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual bool eventFilter(QObject * watched, QEvent * event);//for arm 4.8.6

private:
    Ui::CDeviceAdrTblWid *ui;
    CSingleDevAdrTblWid  *pInputWid;
    CSingleDevAdrTblWid  *pOutputWid;
    QLabel               *bkLbl;
    PageCfgList          *pDevUiCfgList;
    QWidget              *currWid;
    int                   lineIdx;
    QString               currPrefix;
};

#endif // CDEVICEADRTBLWID_H
