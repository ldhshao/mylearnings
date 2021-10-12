#ifndef PAGE_H
#define PAGE_H
#include "UiItem.h"
#include <list>
using namespace std;

class UiPage;

class GroupCfgItem : public UiCfgItem
{
public:
    HNDZ_DECLARE_DYNCREATE(GroupCfgItem)
    GroupCfgItem() {}
    virtual ~GroupCfgItem() {}

    UiCfgItem* getHead();
    UiCfgItem* getNext();
    int      getChildrenCount() {	return m_children.size();	}

    virtual bool initFromDomElement(QDomElement element);
    virtual bool initChildrenFromDomElement(QDomNodeList list);

protected:
    void deleteAll();
    list<UiCfgItem*> m_children;
    list<UiCfgItem*>::iterator m_it;

};

class Page : public UiCfgItem
{
public:
    HNDZ_DECLARE_DYNCREATE(Page)
    Page() :m_uiPage(nullptr) {}
    virtual ~Page();

    UiPage* createPage();
    UiPage* getPage() {	return m_uiPage;}

protected:

    virtual void dump();
    UiPage                 *m_uiPage;
};

class PageList : public Page
{
public:
    PageList()  {}
    virtual ~PageList();

    bool createAllPage(list<UiPage*> &pageList);

    bool readXmlFile(QString strFile);
};
#endif

