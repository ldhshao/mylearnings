#ifndef DEVCFG_H
#define DEVCFG_H
#include "ItemBase.h"
#include "json.h"

class DevCfgItem : public ItemBase
{
public:
    HNDZ_DECLARE_DYNCREATE(DevCfgItem)
    DevCfgItem() {}
    virtual ~DevCfgItem() {}

    virtual bool initFromDomElement(QDomElement element);
#ifdef USE_JSON_SRC
    virtual bool initFromJsonObject(QJsonObject* obj);
    virtual bool initChildrenFromJsonObject(QJsonObject* obj);
#else
    virtual bool initFromJsonObject(QJsonObject obj);
    virtual bool initChildrenFromJsonObject(QJsonObject obj);
#endif
    virtual DevCfgItem* createMyself();

    virtual void dump();
    QString getType() { return m_type; }
    QString paramName() { return m_paName; }
    void    setParamName(QString paName) { m_paName = paName; }

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

static const QString Dev2TypePrewarning;
static const QString Dev2TypeMotor;
static const QString Dev2TypeBrake;
static const QString Dev2TypeAutoTension;
static const QString Dev2TypeSensor;

protected:
    static QString translateType(DevCfgItem* item, int iType);
#ifdef USE_JSON_SRC
    static QString translateType2UiCfgType(bool bList, QJsonObject *obj, int iType);
#else
    static QString translateType2UiCfgType(bool bList, QJsonObject obj, int iType);
#endif
    QString m_type;
    QString m_paName;
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
#ifdef USE_JSON_SRC
    virtual bool initChildrenFromJsonObject(QJsonObject* obj);
#else
    virtual bool initChildrenFromJsonObject(QJsonObject obj);
#endif
    virtual DevCfgItem* createMyself();
    void    copyChildren(DevCfgList* pDst);

    virtual void dump();
protected:
    void deleteAll();
    bool readDevCfgJsonFile(QString strFile);
    QList<DevCfgItem*> m_children;
    QList<DevCfgItem*>::iterator m_it;
};
#endif
