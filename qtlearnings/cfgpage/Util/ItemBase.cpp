#include "ItemBase.h"
#include <qtextstream.h>
#include <QtDebug>

HNDZ_IMPLEMENT_DYNCREATE(XmlItem, BaseObject)
bool XmlItem::initFromDomElement(QDomElement element)
{
    QString strId, strName;
    strId = element.attribute("id");
    strName = element.attribute("name");
    if (!strId.isEmpty())
        m_id = strId.toInt();
    if (!strName.isEmpty())
        m_name = strName;

    return true;
}
void XmlItem::dump()
{
    qDebug()<<"ID "<<m_id<<" name "<<m_name;
}

HNDZ_IMPLEMENT_DYNCREATE(XmlList, XmlItem)
ReflectFactory XmlList::m_reflectFactory;
XmlList::~XmlList()
{
    deleteAll();
}
bool XmlList::readXmlFile(QString strFile)
{
    QFile file(strFile);
    if(!file.exists(strFile))
    {
        return false;
    }
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QDomDocument doc;
    QTextStream stream(file.readAll());
    if(!doc.setContent(stream.readAll()))
    //QString err;
    //int iLine, iCol;
    //if(!doc.setContent(file.readAll(), false, &err, &iLine, &iCol))
    {
        file.close();
        return false;
    }
    file.close();
    qDebug()<<"load successfully 1";
    qWarning()<<"load successfully 2";

    return initFromDomElement(doc.documentElement());
}
void XmlList::deleteAll()
{
    QList<XmlItem*>::ConstIterator it = m_children.cbegin();
    for (; it != m_children.cend(); it++){
        delete *it;
    }
    m_children.clear();
}

bool XmlList::initFromDomElement(QDomElement element)
{
    if (XmlItem::initFromDomElement(element))
        return initChildrenFromDomElement(element.childNodes());

    return false;
}

bool XmlList::initChildrenFromDomElement(QDomNodeList list)
{
    int count = list.count();
    for(int i = 0; i < count; i++)
    {
        QDomNode dn = list.at(i);
        if(dn.isElement())
        {
            QDomElement ele = dn.toElement();
            QString tag = ele.tagName();
            const char *strName = tag.toUtf8().constData();
            //create XmlItem
            XmlItem* pItem = dynamic_cast<XmlItem*>(m_reflectFactory.CreateObject(ele.tagName().toUtf8().constData()));
            if (nullptr == pItem)
                return false;

            //init from Dom
            pItem->initFromDomElement(ele);
            //add list
            m_children.append(pItem);
        }
    }

}
void XmlList::dump()
{
    XmlItem::dump();
    QList<XmlItem*>::ConstIterator it = m_children.cbegin();
    for (; it != m_children.cend(); it++){
        (*it)->dump();
    }
}
