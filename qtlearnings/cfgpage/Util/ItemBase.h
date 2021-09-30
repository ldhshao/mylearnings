#ifndef ITEMBASE_H
#define ITEMBASE_H
#include "DynamicLib.h"
#include <QString>
#include <QList>
#include <qdom.h>

class ItemBase : public BaseObject
{
public:
    ItemBase () { m_id = 0;}
    virtual ~ItemBase() {}
    int getId() {return m_id;}
    QString getName() {return m_name;}

protected:
    int m_id;
    QString m_name;
};

class XmlItem : public ItemBase
{
public:
    XmlItem() {}
    virtual ~XmlItem() {}

    virtual bool initFromDomElement(QDomElement element);

    virtual void dump();
protected:
};
class XmlList : public XmlItem
{
public:
    XmlList() {}
    virtual ~XmlList();

    bool readXmlFile(QString strFile);

    virtual bool initFromDomElement(QDomElement element);
    virtual bool initChildrenFromDomElement(QDomNodeList list);

    virtual void dump();
protected:
    void deleteAll();
    QList<XmlItem*> m_children;
    ReflectFactory m_reflectFactory;
};
#endif
