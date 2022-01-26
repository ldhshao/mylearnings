#ifndef PAGECONTAINER_H
#define PAGECONTAINER_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include "cwidgetbutton.h"
#include <list>
using namespace std;

class UiPage;
namespace Ui {
class PageContainer;
}

class PageContainer : public QWidget
{
    Q_OBJECT

public:
    explicit PageContainer(QWidget *parent = nullptr);
    ~PageContainer();

    void setContent(UiPage *page);
    void addWidgetLink(QWidget* w, QString strName);
    void clear();

    virtual void resizeEvent(QResizeEvent *event);

signals:
    void sig_selectButton(QWidget* pSubWid);

protected slots:
    void slot_showWidget(QWidget* pSubWid);
    void slot_vertScrollBarValueChanged(int val);

private:
    Ui::PageContainer *ui;
    UiPage*            pContentWidget;
    CWidgetButtonMgr   widButtonMgr;
    QScrollArea       *pScrollArea;
    QVBoxLayout       *pVLayout;
    int                iTagWidWidth;
    int                iTagWidHeigth;
    bool               bInited;
    list<QWidget*>     subWidList;
};

#endif // PAGECONTAINER_H
