#include "Page.h"
#include "UiCommon/uipage.h"
#include <QDebug>

HNDZ_IMPLEMENT_DYNCREATE(Group, UiItem)

Group::~Group()
{
    deleteAll();
}

QWidget* Group::create()
{
    return nullptr;
}
bool Group::init(QWidget* w)
{
    return false;
}

bool Group::readXmlFile(QString strFile)
{

    return false;
}
UiItem* Group::getHead()
{

}
UiItem* Group::getNext()
{

}

bool Group::initFromDomElement(QDomElement element)
{

    return false;
}
bool Group::initChildrenFromDomElement(QDomNodeList list)
{

    return false;
}

void Group::dump()
{

}
void Group::deleteAll()
{

}

//Page
HNDZ_IMPLEMENT_DYNCREATE(Page, UiItem)
Page::~Page()
{
    deleteAll();
}

UiPage* Page::createPage()
{
    if (nullptr == m_uiPage){
        m_uiPage = new UiPage();
        init(m_uiPage);
        //first create group
        for (list<UiItem*>::iterator it = m_children.begin(); it !=m_children.end(); it++){
            if ((*it)->isType(UiItem::strTypeGroup)){
                QWidget* w = (*it)->create();
                if (nullptr != w){
                    w->setParent(m_uiPage);
                    m_uiPage->addWidget(w);
                    (*it)->init(w);
                }
            }
        }
        //then create other control
        for (list<UiItem*>::iterator it = m_children.begin(); it !=m_children.end(); it++){
            if (!(*it)->isType(UiItem::strTypeGroup)){
                QWidget* w = (*it)->create();
                if (nullptr != w){
                    w->setParent(m_uiPage);
                    m_uiPage->addWidget(w);
                    (*it)->init(w);
                }
            }
        }
    }
    return m_uiPage;
}

UiItem* Page::getHead()
{
    m_it = m_children.begin();
    if (m_children.end() != m_it)
        return *m_it;

    return nullptr;
}
UiItem* Page::getNext()
{
    if (m_children.end() != m_it){
        m_it++;
        if (m_children.end() != m_it){
            return *m_it;
        }
    }

    return nullptr;
}

bool Page::initFromDomElement(QDomElement element)
{
    if (UiItem::initFromDomElement(element))
        return initChildrenFromDomElement(element.childNodes());
    return false;
}
bool Page::initChildrenFromDomElement(QDomNodeList list)
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
            UiItem* pItem = dynamic_cast<UiItem*>(XmlList::m_reflectFactory.CreateObject(ele.tagName().toUtf8().constData()));
            if (nullptr == pItem)
                return false;

            //init from Dom
            pItem->initFromDomElement(ele);
            //add list
            m_children.push_back(pItem);
        }
    }

    return false;
}

void Page::dump()
{

}
void Page::deleteAll()
{
    m_it = m_children.begin();
    for (; m_it!= m_children.end(); m_it++){
        delete *m_it;
    }

    m_children.clear();
}

//PageList
PageList::~PageList()
{
    deleteAll();
}
bool PageList::createAllPage(list<UiPage*> &pageList)
{
    for(list<UiItem*>::iterator it = m_children.begin(); it != m_children.end(); it++){
        Page *page = dynamic_cast<Page*>(*it);
        UiPage *w = page->createPage();
        w->hide();
        pageList.push_back(w);
    }
    return true;
}

bool PageList::readXmlFile(QString strFile)
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
    //if(!doc.setContent(file.readAll()))
    QString err;
    int iLine, iCol;
    if(!doc.setContent(file.readAll(), false, &err, &iLine, &iCol))
    {
        file.close();
        return false;
    }
    file.close();
    qDebug()<<"load successfully 1";
    qWarning()<<"load successfully 2";

    return initFromDomElement(doc.documentElement());

}
