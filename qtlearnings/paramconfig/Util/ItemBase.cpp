#include "ItemBase.h"
#include <qtextstream.h>
#include <QFile>
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
    for (auto it = m_children.begin(); it != m_children.end(); it++){
        delete *it;
    }
    m_children.clear();
}

XmlItem* XmlList::getHead()
{
    m_it = m_children.begin();
    if (m_children.end() != m_it)
        return *m_it;

    return nullptr;
}

XmlItem* XmlList::getNext()
{
    if (m_children.end() != m_it){
        m_it++;
        if (m_children.end() != m_it)
            return *m_it;
    }

    return nullptr;
}

XmlItem* XmlList::getItemById(int id)
{
    if (id == m_id)
        return this;

    for (auto it = m_children.begin(); it != m_children.end(); it++){
        XmlList *pList = dynamic_cast<XmlList*>(*it);
        if (nullptr != pList){
            XmlItem* pItem = pList->getItemById(id);
            if (nullptr != pItem)
                return pItem;
        }else {
            if (id == (*it)->getId())
                return *it;
        }
    }

    return nullptr;
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
    for (auto it = m_children.begin(); it != m_children.end(); it++){
        (*it)->dump();
    }
}
