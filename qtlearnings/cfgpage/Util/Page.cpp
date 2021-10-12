#include "Page.h"
#include "UiCommon/uipage.h"
#include <QGroupBox>
#include <set>
#include <vector>
#include <QDebug>

//GroupCfgItem
HNDZ_IMPLEMENT_DYNCREATE(GroupCfgItem, UiCfgItem)
GroupCfgItem::~GroupCfgItem()
{
    deleteAll();
}

UiCfgItem* GroupCfgItem::getHead()
{
    m_it = m_children.begin();
    if (m_children.end() != m_it)
        return *m_it;

    return nullptr;
}
UiCfgItem* GroupCfgItem::getNext()
{
    if (m_children.end() != m_it){
        m_it++;
        if (m_children.end() != m_it){
            return *m_it;
        }
    }

    return nullptr;
}

bool GroupCfgItem::initFromDomElement(QDomElement element)
{
    if (UiCfgItem::initFromDomElement(element)){
        if (m_type.isEmpty())
            m_type = UiCfgItem::strTypeGroup;
        return initChildrenFromDomElement(element.childNodes());
    }
    return false;
}
bool GroupCfgItem::initChildrenFromDomElement(QDomNodeList list)
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
            UiCfgItem* pItem = dynamic_cast<UiCfgItem*>(XmlList::m_reflectFactory.CreateObject(ele.tagName().toUtf8().constData()));
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

void GroupCfgItem::deleteAll()
{
    m_it = m_children.begin();
    for (; m_it!= m_children.end(); m_it++){
        delete *m_it;
    }

    m_children.clear();
}

void GroupCfgItem::create(QWidget *parent)
{
    QGroupBox* pBox = new QGroupBox(parent);
    pBox->setTitle(getName());
    m_pWidget = pBox;
    UiPage* page = dynamic_cast<UiPage*>(parent);
    list<UiCfgItem*>::iterator it = m_children.begin();
    for (; it != m_children.end(); it++){
        (*it)->create(m_pWidget);
        if (nullptr != page){
            if (nullptr != (*it)->getWidget())
                page->addWidget((*it)->getWidget());
            if (nullptr != (*it)->getWidName())
                page->addWidget((*it)->getWidName());
            if (nullptr != (*it)->getWidDes())
                page->addWidget((*it)->getWidDes());
        }
    }
    if (nullptr != page){
        page->addWidget(m_pWidget);
    }
}

bool GroupCfgItem::init()
{
    //only for group
    int iSpanName = 4;
    int iSpanDes = 2;
    int iSpanItem = 10;
    int iSpanMargin = 4;
    //find cols and rows
    //count max width and max height
    set<int> cols, rows;
    vector<int> colInfos;
    int colCnt = 0, rowCnt = 0;
    int iMaxWidth=0, iMaxHeight = 0;
    list<UiCfgItem*>::iterator it = m_children.begin();
    for (; it != m_children.end(); it++){
        int l = (*it)->left(), t = (*it)->top();
        if (cols.find(l) == cols.end())
            cols.insert(l);
        if (rows.find(t) == rows.end())
            rows.insert(t);

        //width/height
        int w = 0, h = 0;
        QWidget *w0 = (*it)->getWidget();
        QWidget *w1 = (*it)->getWidName();
        QWidget *w2 = (*it)->getWidDes();
        if (nullptr != w0){
            w += w0->size().width();
            if (w0->size().height() > h) h = w0->size().height();
        }
        if (nullptr != w1){
            w += w1->size().width();
            w += iSpanName;
            if (w1->size().height() > h) h = w1->size().height();
        }
        if (nullptr != w2){
            w += w2->size().width();
            w += iSpanDes;
            if (w2->size().height() > h) h = w2->size().height();
        }
        iMaxWidth = ((iMaxWidth > w) ? iMaxWidth : w);
        iMaxHeight = ((iMaxHeight > h) ? iMaxHeight : h);
    }
    if (!cols.empty()){
        int s0 = iSpanMargin, s1 = *cols.begin();
        set<int>::iterator its = cols.begin();
        do {
            colInfos.push_back(s0);
            colInfos.push_back(s1);
            s0 += iMaxWidth;
            s1 += iMaxWidth;
        }while ( its != cols.end());
        colCnt = cols.size();
        rowCnt = rows.size();
    }
    //init every children
    int row = 0, col = -1;
    iMaxHeight += iSpanMargin;
    it = m_children.begin();
    for (; it != m_children.end(); it++){
        QWidget *w0 = (*it)->getWidget();
        QWidget *w1 = (*it)->getWidName();
        QWidget *w2 = (*it)->getWidDes();
        if (nullptr != w0){
            w0->move(colInfos[2*col + 1], row * iMaxHeight);
            w0->show();
        }
        if (nullptr != w1){
            w1->move(colInfos[2*col + 0], row * iMaxHeight);
            w1->show();
        }
        if (nullptr != w2){
            w2->move(colInfos[2*col + 1] + w0->size().width() + iSpanDes, row * iMaxHeight);
            w2->show();
        }
        col++;
        if (col >= colCnt){
            row++; col = 0;
        }
    }
    //init myself
    m_pWidget->resize(iSpanMargin + colCnt * iMaxWidth + (colCnt - 1) * iSpanItem + iSpanMargin,
                      iSpanMargin + rowCnt * iMaxWidth + (rowCnt - 1) * iSpanItem + iSpanMargin);
}

//PageCfg
HNDZ_IMPLEMENT_DYNCREATE(PageCfg, GroupCfgItem)
PageCfg::~PageCfg()
{
    GroupCfgItem::deleteAll();
}

UiPage* PageCfg::createPage()
{
    if (nullptr == m_uiPage){
        m_uiPage = new UiPage();
        //first create group
        for (list<UiCfgItem*>::iterator it = m_children.begin(); it !=m_children.end(); it++){
            (*it)->create(m_uiPage);
            (*it)->init();
        }
    }
    return m_uiPage;
}

void PageCfg::dump()
{

}

//PageCfgList
PageCfgList::~PageCfgList()
{
    deleteAll();
}
bool PageCfgList::createAllPage(list<UiPage*> &pageList)
{
    for(list<UiCfgItem*>::iterator it = m_children.begin(); it != m_children.end(); it++){
        PageCfg *page = dynamic_cast<PageCfg*>(*it);
        UiPage *w = page->createPage();
        w->hide();
        pageList.push_back(w);
    }
    return true;
}

bool PageCfgList::readXmlFile(QString strFile)
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
