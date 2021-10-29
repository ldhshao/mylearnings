#include "devcfg.h"
#include <qtextstream.h>
#include <QtDebug>

const QString DevCfgItem::DevTypeSystem = "system";
const QString DevCfgItem::DevTypeWorkFace = "workface";
const QString DevCfgItem::DevTypeBelt = "belt";
const QString DevCfgItem::DevTypePump = "pump";
const QString DevCfgItem::DevTypeBunker = "bunker";
const QString DevCfgItem::DevTypeMotorGroup = "motorgroup";
const QString DevCfgItem::DevTypeMotor = "motor";
const QString DevCfgItem::DevTypeWorkVoithGroup = "workvoithgroup";
const QString DevCfgItem::DevTypeWorkVoith = "workvoith";
const QString DevCfgItem::DevTypeAnalogProtect = "analogprotect";

HNDZ_IMPLEMENT_DYNCREATE(DevCfgItem, BaseObject)
bool DevCfgItem::initFromDomElement(QDomElement element)
{
    setIntValue(m_id, element, "id");
    setStrValue(m_name, element, "name");
    setStrValue(m_type, element, "type");

    return true;
}
DevCfgItem* DevCfgItem::createMyself()
{
    DevCfgItem *pItem = new DevCfgItem();
    pItem->m_id = m_id;
    pItem->m_name = m_name;
    pItem->m_type = m_type;
}

void DevCfgItem::dump()
{
    qDebug()<<"ID "<<m_id<<" name "<<m_name <<" type "<<m_type;
}

HNDZ_IMPLEMENT_DYNCREATE(DevCfgList, DevCfgItem)
ReflectFactory DevCfgList::m_reflectFactory;
DevCfgList::~DevCfgList()
{
    deleteAll();
}
bool DevCfgList::readDevCfgFile(QString strFile)
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
void DevCfgList::deleteAll()
{
    QList<DevCfgItem*>::ConstIterator it = m_children.cbegin();
    for (; it != m_children.cend(); it++){
        delete *it;
    }
    m_children.clear();
}

DevCfgItem* DevCfgList::getHead()
{
    m_it = m_children.begin();
    if (m_children.end() != m_it)
        return *m_it;

    return nullptr;
}

DevCfgItem* DevCfgList::getNext()
{
    if (m_children.end() != m_it){
        m_it++;
        if (m_children.end() != m_it)
            return *m_it;
    }

    return nullptr;
}

DevCfgItem* DevCfgList::getItemById(int id)
{
    if (id == m_id)
        return this;

    QList<DevCfgItem*>::iterator it = m_children.begin();
    for (; it != m_children.cend(); it++){
        DevCfgList *pList = dynamic_cast<DevCfgList*>(*it);
        if (nullptr != pList){
            DevCfgItem* pItem = pList->getItemById(id);
            if (nullptr != pItem)
                return pItem;
        }else {
            if (id == (*it)->getId())
                return *it;
        }
    }

    return nullptr;
}

bool DevCfgList::initFromDomElement(QDomElement element)
{
    if (DevCfgItem::initFromDomElement(element))
        return initChildrenFromDomElement(element.childNodes());

    return false;
}

bool DevCfgList::initChildrenFromDomElement(QDomNodeList list)
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
            //create DevCfgItem
            DevCfgItem* pItem = dynamic_cast<DevCfgItem*>(m_reflectFactory.CreateObject(ele.tagName().toUtf8().constData()));
            if (nullptr == pItem)
                return false;

            //init from Dom
            pItem->initFromDomElement(ele);
            //add list
            m_children.append(pItem);
        }
    }

}
DevCfgItem* DevCfgList::createMyself()
{
    DevCfgList* pList = new DevCfgList();
    pList->m_id = m_id;
    pList->m_name = m_name;
    pList->m_type = m_type;

    copyChildren(pList);
}
void DevCfgList::copyChildren(DevCfgList *pDst)
{
    QList<DevCfgItem*>::iterator it = m_children.begin();
    for (; it != m_children.end(); it++){
        DevCfgItem* pItem = (*it)->createMyself();
        //pDst->readDevCfgFile();
        //unfinished
    }
}

void DevCfgList::dump()
{
    DevCfgItem::dump();
    QList<DevCfgItem*>::ConstIterator it = m_children.cbegin();
    for (; it != m_children.cend(); it++){
        (*it)->dump();
    }
}
