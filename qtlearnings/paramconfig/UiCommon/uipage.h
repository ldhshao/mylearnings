#ifndef UIPAGE_H
#define UIPAGE_H

#include "uistatectl.h"
#include <QWidget>
#include <list>
using namespace std;

class UiPage : public QWidget
{
    Q_OBJECT

public:
    explicit UiPage(QWidget *parent = nullptr);
    ~UiPage();

    void addWidget(QWidget* w);
    QWidget* getHead();
    QWidget* getNext();

signals:

protected slots:

private:
    void deleteAll();
    list<QWidget*>     ctlList;
    list<QWidget*>::iterator m_it;
    CStateCtlMgr       ctlMgr;//manage focus control in page
};

#endif // PAGECONTAINER_H
