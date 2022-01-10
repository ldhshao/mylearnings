#ifndef UIPAGE_H
#define UIPAGE_H

#include "uistatectl.h"
#include <QWidget>
#include <list>
using namespace std;

class QLabel;
#define UIPAGE_COL_NUM 2
class UiPage : public QWidget
{
    Q_OBJECT

public:
    explicit UiPage(QWidget *parent = nullptr);
    ~UiPage();

    //void resetModifiedParamAddrList() { mparamAddrList.clear(); }
    void setTitle(QString strTitle);
    void setTitleHeight(int h) { titleHeight = h; }
    void setInitWidthHeight(int w, int h) { initWidth = w; initHeight = h; }
    void fillColList(int col, QWidget* w);
    void initTabOrder();
    void updateUi();
    void showPage();

signals:
    void sig_configFinished();
    void sig_modifiedParamAddrList(list<uint16_t*> *pMparamAddrList);
    void sig_rollBack_paramAddrList(list<uint16_t*> *pMparamAddrList);

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void focusInEvent(QFocusEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

protected slots:
    void slot_valueChanged(uint16_t *pVal, uint32_t valNew);

private:
    void deleteAll();
    void showTipInfo(QPoint pt, const QString& strTip);
    bool checkRequiredData();
    QLabel            *title;
    list<QWidget*>::iterator m_it;
    list<QWidget*>     colList[UIPAGE_COL_NUM];
    list<uint16_t*>    mparamAddrList;//modified params address list
    int                titleHeight;
    int                initWidth, initHeight;
    bool               init;
};

#endif // PAGECONTAINER_H
