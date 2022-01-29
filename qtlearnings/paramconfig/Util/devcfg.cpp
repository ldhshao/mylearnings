#include "devcfg.h"
#include <qtextstream.h>
#include <QByteArray>
#include <QFile>
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
#else
#include <QJsonDocument>
#include <QJsonParseError>
#endif
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
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
bool DevCfgItem::initFromJsonObject(QJsonObject* obj)
{
    qDebug()<<obj;
    m_name = obj->value("Name").toString();
    bool bOK = false;
    int iType = obj->value("Type").toInt(&bOK);
    if (bOK){
        m_type = translateType2UiCfgType(this, iType);
    }

    initChildrenFromJsonObject(obj);

    return true;
}
bool DevCfgItem::initChildrenFromJsonObject(QJsonObject* obj)
{
    return true;
}
#else
bool DevCfgItem::initFromJsonObject(QJsonObject obj)
{
    qDebug()<<obj;
    QJsonValue::Type type = obj.value("Name").type();
    switch (type){
    case QJsonValue::String:
        m_name = obj.value("Name").toString();
        break;
    }
    type = obj.value("Type").type();
    int iType = 0;
    switch (type){
    case QJsonValue::Double:
        iType = obj.value("Type").toInt();
        m_type = translateType2UiCfgType(this, iType);
        break;
    case QJsonValue::String:
        m_type = obj.value("Type").toString();
        break;
    }

    initChildrenFromJsonObject(obj);

    return true;
}
bool DevCfgItem::initChildrenFromJsonObject(QJsonObject obj)
{
    return true;
}
#endif
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

QString DevCfgItem::translateType(DevCfgItem* item, int iType)
{
    DevCfgList* pList = dynamic_cast<DevCfgList*>(item);
    QStringList types;
    if (nullptr != pList){
        types<<"";
        types<<"皮带";
        types<<"破碎机";
        types<<"转载机";
        types<<"运输机";
        types<<"清水泵";
        types<<"乳化泵";
        types<<"单点";
        types<<"煤仓";
    }else{
        types<<"";
        types<<"预警";
        types<<"电机";
        types<<"制动闸";
        types<<"张紧";
        types<<"皮带VOITH";
        types<<"自动张紧";
        types<<"";
        types<<"";
        types<<"";
        types<<"";
        types<<"";
        types<<"";
        types<<"";
        types<<"";
        types<<"";
    }

    if (0 <= iType && iType < types.count())
        return types[iType];
    return "";
}
QString DevCfgItem::translateType2UiCfgType(DevCfgItem* item, int iType)
{
    DevCfgList* pList = dynamic_cast<DevCfgList*>(item);
    QStringList types;
    if (nullptr != pList){
        types<<"";
        types<<"workface";//"皮带";
        types<<"workface";//"破碎机";
        types<<"workface";//"转载机";
        types<<"";//"运输机";
        types<<"";//"清水泵";
        types<<"";//"乳化泵";
        types<<"";//"单点";
        types<<"";//"煤仓";
    }else{
        types<<"";
        types<<"prewarninggroup";//"预警";
        types<<"motorgroup";//"电机";
        types<<"brakegroup";//"制动闸";
        types<<"autotensiongroup";//"张紧";
        types<<"";//"皮带VOITH";
        types<<"generalsensor";//"通用传感器";
        types<<"";//"";
        types<<"";//"";
        types<<"";//"";
        types<<"";//"";
        types<<"";//"";
        types<<"";//"";
        types<<"";//"";
        types<<"";//"";
        types<<"";//"";
    }

    if (0 <= iType && iType < types.count())
        return types[iType];
    return "";
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
    if (-1 < strFile.indexOf(".json"))
        return readDevCfgJsonFile(strFile);

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
bool DevCfgList::readDevCfgJsonFile(QString strFile)
{
    QFile file(strFile);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
    QJsonObject root;
    if (!root.fromString(file.readAll()))
        return false;
    QList<QJsonObject*> devList = root.children()[0]->children();
    for (auto it = devList.begin(); it != devList.end(); it++){
        QString strID = (*it)->key();
        int pos = strID.lastIndexOf('_');
        if (-1 == pos) qDebug()<<"error device1 name "<<strID;
        DevCfgList *pList = new DevCfgList();
        pList->m_paName = strID;
        pList->m_id = strID.right(strID.length() - pos - 1).toInt();
        pList->initFromJsonObject(*it);
        m_children.push_back(pList);
    }
#else
    QJsonParseError parseError;
    QJsonDocument doc=QJsonDocument::fromJson(file.readAll(),&parseError);
    file.close();
    if(parseError.error!=QJsonParseError::NoError){
        return false;
    }
    QJsonObject obj=doc.object().begin()->toObject();
    for (QJsonObject::iterator it = obj.begin(); it != obj.end(); it++){
        QString strID = it.key();
        int pos = strID.lastIndexOf('_');
        if (-1 == pos) qDebug()<<"error device1 name "<<strID;
        DevCfgList *pList = new DevCfgList();
        pList->m_paName = strID;
        pList->m_id = strID.right(strID.length() - pos - 1).toInt();
        pList->initFromJsonObject(it->toObject());
        m_children.push_back(pList);
    }
#endif
}

void DevCfgList::deleteAll()
{
    for (auto it=m_children.begin(); it != m_children.end(); it++){
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

    for (auto it=m_children.begin(); it != m_children.end(); it++){
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
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
bool DevCfgList::initChildrenFromJsonObject(QJsonObject* obj)
{
    QList<QJsonObject*> childList = obj->children();
    for (auto it = childList.begin(); it != childList.end(); it++){
        if (QJsonObject::vtfObject == (*it)->type()){
            QString strID = (*it)->key();
            int pos = strID.lastIndexOf('_');
            if (-1 == pos) qDebug()<<"error device1 name "<<strID;
            DevCfgItem *pItem = new DevCfgItem();
            pItem->setParamName(strID);
            pItem->setId(strID.right(strID.length() - pos - 1).toInt());
            pItem->initFromJsonObject(*it);
            m_children.append(pItem);
        }
    }

    return true;
}
#else
bool DevCfgList::initChildrenFromJsonObject(QJsonObject obj)
{
    for (auto it = obj.begin(); it != obj.end(); it++){
        if (it->isObject()){
            QString strID = it.key();
            int pos = strID.lastIndexOf('_');
            if (-1 == pos) qDebug()<<"error device1 name "<<strID;
            DevCfgItem *pItem = new DevCfgItem();
            pItem->setParamName(strID);
            pItem->setId(strID.right(strID.length() - pos - 1).toInt());
            pItem->initFromJsonObject(it->toObject());
            m_children.append(pItem);
        }
    }

    return true;
}
#endif
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
    for (auto it=m_children.begin(); it != m_children.end(); it++){
        (*it)->dump();
    }
}
