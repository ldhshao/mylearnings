#ifndef ITEMBASE_H
#define ITEMBASE_H
#include "DynamicLib.h"
#include <QString>
#include <QList>
#include <qdom.h>

#define setIntValue(var, element, attr) {\
        QString strVal = element.attribute(attr);\
        if (!strVal.isEmpty())                   \
            var = strVal.toInt();                \
    }

#define setStrValue(var, element, attr) var = element.attribute(attr);

class ItemBase : public BaseObject
{
public:
    ItemBase () { m_id = 0;}
    virtual ~ItemBase() {}
    int getId() {return m_id;}
    void setId(int id) { m_id = id; }
    QString getName() {return m_name;}
    void setName(const QString& name) { m_name = name; }

protected:
    int m_id;
    QString m_name;
};

class XmlItem : public ItemBase
{
public:
    HNDZ_DECLARE_DYNCREATE(XmlItem)
    XmlItem() {}
    virtual ~XmlItem() {}

    virtual bool initFromDomElement(QDomElement element);

    virtual void dump();
protected:
};
class XmlList : public XmlItem
{
public:
    HNDZ_DECLARE_DYNCREATE(XmlList)
    XmlList() {}
    virtual ~XmlList();

    static ReflectFactory m_reflectFactory;

    bool readXmlFile(QString strFile);
    XmlItem* getHead();
    XmlItem* getNext();
    int      getChildrenCount() {	return m_children.count();	}
    XmlItem* getItemById(int id);

    virtual bool initFromDomElement(QDomElement element);
    virtual bool initChildrenFromDomElement(QDomNodeList list);

    virtual void dump();
protected:
    void deleteAll();
    QList<XmlItem*> m_children;
    QList<XmlItem*>::iterator m_it;
};
#endif
