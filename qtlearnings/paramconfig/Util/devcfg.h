#ifndef DEVCFG_H
#define DEVCFG_H
#include "ItemBase.h"

class DevCfgItem : public ItemBase
{
public:
    HNDZ_DECLARE_DYNCREATE(DevCfgItem)
    DevCfgItem() {}
    virtual ~DevCfgItem() {}

    virtual bool initFromDomElement(QDomElement element);
    virtual DevCfgItem* createMyself();

    virtual void dump();
    QString getType() { return m_type; }

static const QString DevTypeSystem;
static const QString DevTypeWorkFace;
static const QString DevTypeBelt;
static const QString DevTypePump;
static const QString DevTypeBunker;
static const QString DevTypeMotorGroup;
static const QString DevTypeMotor;
static const QString DevTypeWorkVoithGroup;
static const QString DevTypeWorkVoith;
static const QString DevTypeAnalogProtect;

protected:
    QString m_type;
};
class DevCfgList : public DevCfgItem
{
public:
    HNDZ_DECLARE_DYNCREATE(DevCfgList)
    DevCfgList() {}
    virtual ~DevCfgList();

    static ReflectFactory m_reflectFactory;

    bool readDevCfgFile(QString strFile);
    DevCfgItem* getHead();
    DevCfgItem* getNext();
    int      getChildrenCount() {	return m_children.count();	}
    DevCfgItem* getItemById(int id);

    virtual bool initFromDomElement(QDomElement element);
    virtual bool initChildrenFromDomElement(QDomNodeList list);
    virtual DevCfgItem* createMyself();
    void    copyChildren(DevCfgList* pDst);

    virtual void dump();
protected:
    void deleteAll();
    QList<DevCfgItem*> m_children;
    QList<DevCfgItem*>::iterator m_it;
};
#endif
