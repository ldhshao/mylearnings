#include "datacfgitem.h"

HNDZ_IMPLEMENT_DYNCREATE(DataCfgItem, XmlItem)
QString DataCfgItem::getNamePath(int ancestorDepth)
{
    QString strName;
    //
    return strName;
}

QString DataCfgItem::getFullName()
{
    QString strName;
    //
    return strName;
}

QString DataCfgItem::getFullName(int idx)
{
    QString strName;
    //
    return strName;
}

bool DataCfgItem::initFromDomElement(QDomElement element)
{

}

void DataCfgItem::dump()
{

}

DataCfgItem* DataCfgItem::createMyself()
{
    return nullptr;

}
void DataCfgItem::copyTo(DataCfgItem* destItem)
{

}

uint16_t *DataCfgItem::paramAddress()
{
    return nullptr;
}

bool DataCfgItem::loadFromJsonObject(QJsonObject* obj)
{
    return false;
}
bool DataCfgItem::saveToJsonObject(QJsonObject* obj)
{
    return false;
}
