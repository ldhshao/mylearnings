#ifndef PAGE_H
#define PAGE_H
#include "UiItem.h"
#include <list>
using namespace std;

class UiPage;
class Group : public UiItem
{
public:
    HNDZ_DECLARE_DYNCREATE(Group)
    Group()  {}
    virtual ~Group();

    virtual QWidget* create();
    virtual bool init(QWidget* w);

    bool readXmlFile(QString strFile);
    UiItem* getHead();
    UiItem* getNext();
    int      getChildrenCount() {	return m_children.size();	}

protected:
    virtual bool initFromDomElement(QDomElement element);
    virtual bool initChildrenFromDomElement(QDomNodeList list);

    virtual void dump();
protected:
    void deleteAll();
    list<UiItem*> m_children;
    list<UiItem*>::iterator m_it;
    static ReflectFactory m_reflectFactory;
};

class Page : public UiItem
{
public:
    HNDZ_DECLARE_DYNCREATE(Page)
    Page() :m_uiPage(nullptr) {}
    virtual ~Page();

    UiPage* createPage();
    UiPage* getPage() {	return m_uiPage;}

    UiItem* getHead();
    UiItem* getNext();
    int      getChildrenCount() {	return m_children.size();	}

protected:
    virtual bool initFromDomElement(QDomElement element);
    virtual bool initChildrenFromDomElement(QDomNodeList list);

    virtual void dump();
protected:
    void deleteAll();
    list<UiItem*> m_children;
    list<UiItem*>::iterator m_it;
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

