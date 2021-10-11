#include "uipage.h"
#include <QDebug>

UiPage::UiPage(QWidget *parent) :
    QWidget(parent)
{
}

UiPage::~UiPage()
{
    deleteAll();
}

void UiPage::deleteAll()
{
    list<QWidget*>::iterator it = ctlList.begin();
    for (; it != ctlList.end(); it++) {
        delete *it;
    }

    ctlList.clear();
}

void UiPage::addWidget(QWidget *w)
{
    ctlList.push_back(w);
    CStateCtl *pCtl = dynamic_cast<CStateCtl*>(w);
    if (nullptr != pCtl)
        pCtl->setManager(&ctlMgr);
}

QWidget* UiPage::getHead()
{
    m_it = ctlList.begin();
    if (ctlList.end() != m_it)
        return *m_it;

    return nullptr;
}
QWidget* UiPage::getNext()
{
    if (ctlList.end() != m_it){
        m_it++;
        if (ctlList.end() != m_it){
            return *m_it;
        }
    }

    return nullptr;
}
