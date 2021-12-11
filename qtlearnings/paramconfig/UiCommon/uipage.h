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

    void setTitle(QString strTitle);
    void setTitleHeight(int h) { titleHeight = h; }
    void setInitWidthHeight(int w, int h) { initWidth = w; initHeight = h; }
    void fillColList();
    void initTabOrder();

signals:
    void sig_configFinished();

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void focusInEvent(QFocusEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

protected slots:

private:
    void fillColList(QWidget* w);
    void deleteAll();
    QLabel            *title;
    list<QWidget*>::iterator m_it;
    list<QWidget*>     colList[UIPAGE_COL_NUM];
    CStateCtlMgr       ctlMgr;//manage focus control in page
    int                titleHeight;
    int                initWidth, initHeight;
};

#endif // PAGECONTAINER_H
