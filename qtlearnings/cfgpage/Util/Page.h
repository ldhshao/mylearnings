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
    GroupCfgItem() {
        m_type = UiCfgItem::strTypeGroup;
    }
    virtual ~GroupCfgItem();

    UiCfgItem* getHead();
    UiCfgItem* getNext();
    int      getChildrenCount() {	return m_children.size();	}

    virtual bool initFromDomElement(QDomElement element);
    virtual bool initChildrenFromDomElement(QDomNodeList list);

    virtual void create(QWidget* parent);
    virtual bool init();
    virtual bool initData(unsigned short* pStAddr);//init data
    void         addToPage(UiPage* page);

protected:
    void deleteAll();
    list<UiCfgItem*> m_children;
    list<UiCfgItem*>::iterator m_it;

};

class PageCfg : public GroupCfgItem
{
public:
    HNDZ_DECLARE_DYNCREATE(PageCfg)
    PageCfg() :m_uiPage(nullptr) {}
    virtual ~PageCfg();

    UiPage* createPage();
    UiPage* getPage() {	return m_uiPage;}

    virtual bool initFromDomElement(QDomElement element);

    virtual bool init();

protected:

    virtual void dump();
    UiPage                 *m_uiPage;
};

class PageCfgList : public PageCfg
{
public:
    PageCfgList(unsigned short* paramTbl) : m_pParamTbl(paramTbl) {}
    virtual ~PageCfgList();

    bool createAllPage(list<UiPage*> &pageList);

    bool readXmlFile(QString strFile);
protected:
    unsigned short* m_pParamTbl;
};
#endif

