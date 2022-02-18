#ifndef DATACFG_ITEM
#define DATACFG_ITEM
#include "ItemBase.h"
#include "json.h"

class DataCfgItem : public XmlItem
{
public:
    HNDZ_DECLARE_DYNCREATE(DataCfgItem)
    DataCfgItem(DataCfgItem *parent = nullptr) : m_dataidx(-1),m_datacnt(1),m_min(0),m_max(0){
        m_parent = parent;
    }
    virtual ~DataCfgItem() {}

    static QString strDataTypeInt;
    static QString strDataTypeFloat;
    static QString strDataTypeEnum;
    static QString strDataTypeAddr;

    QString getNamePath(int ancestorDepth);
    QString getFullName();
    virtual QString getFullName(int idx);
    virtual void setDataidx(int idx) { m_dataidx = idx; }
    int  dataidx() { return m_dataidx; }
    void setParent(DataCfgItem* parent) { m_parent = parent; }
    DataCfgItem* parent() { return m_parent; }
    int  datacount() { return m_datacnt; }
    bool isType(const QString &type) {	return m_type == type; }

    virtual bool initFromDomElement(QDomElement element);
    virtual void dump();

    virtual DataCfgItem* createMyself();
    virtual void copyTo(DataCfgItem* destItem);

    virtual uint16_t *paramAddress();

    QString paramName() { return m_paName; }
    void setParamName(QString strPaName) { m_paName = strPaName; }
    virtual bool loadFromJsonObject(QJsonObject* obj);
    virtual bool saveToJsonObject(QJsonObject* obj);

protected:
    int m_dataidx, m_datacnt;
    QString m_type;
    uint16_t m_min;
    uint16_t m_max;
    QStringList m_valList;
    uint16_t    m_defaultVal;
    QString m_paName;//for load and save for other
    DataCfgItem  *m_parent;
};
#endif
