#ifndef PAGE_H
#define PAGE_H
#include "UiCfgItem.h"
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
    int      getDataCount();
    void     addChild(UiCfgItem* item) { item->setParent(this); m_children.push_back(item); }
    void     clear()             { m_children.clear(); }

    virtual bool initFromDomElement(QDomElement element);
    virtual bool initChildrenFromDomElement(QDomNodeList list);
    virtual void dump();

    virtual UiCfgItem* createMyself();
    void    copyChildren(GroupCfgItem* pDst);

    //virtual void setDataidx(int idx);
    virtual void create(QWidget* parent);
    virtual bool initData(int idx);
    virtual bool initUi(unsigned short* pStAddr);//init data
    void         addToPage(UiPage* page);
    bool         chkDataConflict(); //test function

    void createPage(list<UiPage*> &pageList);
    //UiPage* getPage() {	return m_uiPage;}

    bool readXmlFile(QString strFile);
    UiCfgItem* findItemById(int id);
    UiCfgItem* findItemById(const QString& strId);

    virtual uint16_t *paramAddress() { return nullptr; }
protected:
    void deleteAll();
    list<UiCfgItem*> m_children;
    list<UiCfgItem*>::iterator m_it;

};

class GroupCfgList : public GroupCfgItem
{
public:
    HNDZ_DECLARE_DYNCREATE(GroupCfgList)
    GroupCfgList() {}
    virtual ~GroupCfgList();

    virtual void create(QWidget* parent);
    virtual bool initUi(unsigned short* pStAddr);//init data
    virtual UiCfgItem* createMyself();
    GroupCfgItem* findGroupByName(const QString& strName);

protected:
};

class PageCfgList : public GroupCfgList
{
public:
    PageCfgList(unsigned short* paramTbl) : m_pParamTbl(paramTbl) {}
    virtual ~PageCfgList();

    bool createAllPage(list<UiPage*> &pageList);
    void setParamTbl(unsigned short* addr) { m_pParamTbl = addr; }

    virtual uint16_t *paramAddress() { return m_pParamTbl; }
protected:
    unsigned short* m_pParamTbl;
};
#endif

