#ifndef PAGE_H
#define PAGE_H
#include "UiCfgItem.h"
#include <list>
using namespace std;

class UiPage;
class CKeyDnComboBox;

class GroupCfgItem : public UiCfgItem
{
public:
    HNDZ_DECLARE_DYNCREATE(GroupCfgItem)
    GroupCfgItem() :m_titleDepth(1),m_cols(1),m_ctlWidth(100),m_ctlHeight(40),m_colMode(true),m_ctlUnitSize(10){
        m_type = UiCfgItem::strTypeGroup; m_devLevel = 0; m_inited = false;
    }
    virtual ~GroupCfgItem();

    UiCfgItem* getHead();
    UiCfgItem* getNext();
    int      getChildrenCount() {	return m_children.size();	}
    int      getDataCount();
    void     addChild(UiCfgItem* item) { item->setParent(this); m_children.push_back(item); }
    void     clear()             { m_children.clear(); }
    uint8_t  titleDepth()        { return m_titleDepth; }
    bool     isInited()          { return m_inited; }
    void     setInited(bool init){ m_inited = init; }

    virtual bool initFromDomElement(QDomElement element);
    virtual bool initChildrenFromDomElement(QDomNodeList list);
    virtual void dump();

    virtual UiCfgItem* createMyself();
    void    copyChildren(GroupCfgItem* pDst);

    //virtual void setDataidx(int idx);
    virtual void create(QWidget* parent);
    virtual bool initData(int idx, bool useDef);
    virtual bool initUi(unsigned short* pStAddr, int w=0, int h=0);//init data
    bool         chkDataConflict(); //test function

    void createPage(list<UiPage*> &pageList);
    //UiPage* getPage() {	return m_uiPage;}

    bool readXmlFile(QString strFile);
    UiCfgItem* findItemById(int id);
    UiCfgItem* findItemById(const QString& strId);
    UiCfgItem* findItemByName(const QString& strName);
    virtual UiCfgItem* findItemByDataIdx(int dataidx);
    void findPortInfoByPortType(int line, int portType, list<pair<uint32_t,QString>> *infoList, bool chkVal=false);

    virtual uint16_t *paramAddress() { return nullptr; }
    uint16_t *firstParamAddress();

    virtual QString previewInfo();
    QString previewInfo(int col);
    virtual void setDefaultVal();

    //load/save json file
    bool readJsonFile(QString strFile);
    bool saveJsonFile(QString strFile);
    virtual bool loadFromJsonObject(QJsonObject* obj, QJsonObject* root=nullptr);
    virtual bool saveToJsonObject(QJsonObject* obj, QJsonObject* root=nullptr);
    void         setDeviceLevel(uint8_t lvl) { m_devLevel = lvl; }
protected:
    void deleteAll();
    list<UiCfgItem*> m_children;
    list<UiCfgItem*>::iterator m_it;
    uint8_t          m_titleDepth;
    uint8_t          m_cols;
    uint8_t          m_devLevel;//default 0; 1: device 2
    int              m_ctlWidth;
    int              m_ctlHeight;
    bool             m_colMode;
    bool             m_inited;
    int              m_ctlUnitSize;
};

class GroupCfgList : public GroupCfgItem
{
public:
    HNDZ_DECLARE_DYNCREATE(GroupCfgList)
    GroupCfgList() {}
    virtual ~GroupCfgList();

    virtual void create(QWidget* parent);
    virtual bool initUi(unsigned short* pStAddr, int w=0, int h=0);//init data
    virtual UiCfgItem* createMyself();
    GroupCfgItem* findGroupByName(const QString& strName);
    virtual UiCfgItem* findItemByDataIdx(int dataidx);
    void addEnableSource(CKeyDnComboBox* pCmb, uint16_t val);

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

