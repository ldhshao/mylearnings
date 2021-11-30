#include "PageCfg.h"
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

int GroupCfgItem::getDataCount()
{
    int iCnt = 0;
    list<UiCfgItem*>::iterator it = m_children.begin();
    for (; it != m_children.end(); it++){
        GroupCfgItem* pGroup = dynamic_cast<GroupCfgItem*>(*it);
        if (nullptr == pGroup)
            iCnt++;
        else
            iCnt += pGroup->getDataCount();
    }

    return iCnt;
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
            addChild(pItem);
        }
    }

    return false;
}

void GroupCfgItem::dump()
{
    UiCfgItem::dump();

    for(list<UiCfgItem*>::iterator it = m_children.begin(); it != m_children.end(); it++){
        (*it)->dump();
    }
}

UiCfgItem* GroupCfgItem::createMyself()
{
    GroupCfgItem* pList = new GroupCfgItem();
    pList->m_id = m_id;
    pList->m_name = m_name;
    pList->m_left = m_left;
    pList->m_top = m_top;
    //pList->m_width = m_width;
    //pList->m_height = m_height;
    pList->m_dataidx = m_dataidx;
    pList->m_type = m_type;
    //pList->m_description = m_description;
    //pList->m_defaultVal = m_defaultVal;
    pList->m_enableSourceId = m_enableSourceId;
    pList->m_enableSourceVal = m_enableSourceVal;

    copyChildren(pList);

    return pList;
}

void GroupCfgItem::copyChildren(GroupCfgItem *pDst)
{
    list<UiCfgItem*>::iterator it = m_children.begin();
    for(; it != m_children.end(); it++){
        UiCfgItem* pItem = (*it)->createMyself();
        pDst->addChild(pItem);
    }
}

void GroupCfgItem::deleteAll()
{
    m_it = m_children.begin();
    for (; m_it!= m_children.end(); m_it++){
        delete *m_it;
    }

    m_children.clear();
}

bool GroupCfgItem::initData(int idx)
{
    m_dataidx = idx;

    int cnt = 0;
    list<UiCfgItem*>::iterator it = m_children.begin();
    for(; it != m_children.end(); it++){
        (*it)->initData(idx);
        cnt += (*it)->datacount();
        idx += (*it)->datacount();
    }
    m_datacnt = cnt;
    //check data conflict
    chkDataConflict();

    return true;
}

bool isInRange(int b, int e, int min, int max)
{
    return ((min <= b) && (e <= max));
}
struct SRange{
    int min;
    int max;
};
bool isConflict(int min, int max, vector<struct SRange>& rangeList)
{
    for(auto it = rangeList.begin(); it != rangeList.end(); it++){
        if (isInRange(min, max, it->min, it->max))
            return true;
    }

    return false;
}
void addRangeList(int min, int max, vector<struct SRange>& rangeList)
{
    for(auto it = rangeList.begin(); it != rangeList.end(); it++){
        if (min == it->max){
            it->max = max;
            return ;
        }else if (max == it->min){
            it->min = min;
            return ;
        }
    }

    struct SRange range = {min, max};
    rangeList.push_back(range);
}
bool GroupCfgItem::chkDataConflict()
{
    bool bRet = false;
    vector<struct SRange> rangeList;
    list<UiCfgItem*>::iterator it = m_children.begin();
    for(; it != m_children.end(); it++){
        int min = (*it)->dataidx(), max = (*it)->dataidx()+(*it)->datacount();
        if (!isConflict(min, max, rangeList)){
            addRangeList(min, max, rangeList);
        }else {
            bRet = true;
            qDebug()<<"error: data conflict "<<(*it)->getName()<<" dataidx "<<min;
        }
    }

    return bRet;
}

void GroupCfgItem::create(QWidget *parent)
{
    QGroupBox* pBox = new QGroupBox(parent);
    pBox->setTitle(getName());
    m_pWidget = pBox;
    //UiPage* page = dynamic_cast<UiPage*>(parent);
    list<UiCfgItem*>::iterator it = m_children.begin();
    for (; it != m_children.end(); it++){
        (*it)->create(m_pWidget);
        //if (nullptr != page){
        //    if (nullptr != (*it)->getWidget())
        //        page->addWidget((*it)->getWidget());
        //    if (nullptr != (*it)->getWidName())
        //        page->addWidget((*it)->getWidName());
        //    if (nullptr != (*it)->getWidDes())
        //        page->addWidget((*it)->getWidDes());
        //}
    }
    //if (nullptr != page){
    //    page->addWidget(m_pWidget);
    //}
}

//version 002
bool GroupCfgItem::initUi(unsigned short* pStAddr)
{
    //only for group
    int iSpanName = 4;
    int iSpanDes = 2;
    int iSpanItem = 10;
    int iSpanMargin = 4;
    int iTitleHeight = 36;
    //find cols and rows
    //count max width and max height
    struct SRowItemInfo{
        int iName;
        int iWidget;
    };

    map<int, struct SRowItemInfo> colWidths, colInfos;
    map<int, int> rowHeights, rowInfos;
    list<UiCfgItem*>::iterator it = m_children.begin();
    for (; it != m_children.end(); it++){
        (*it)->initUi(pStAddr);

        //width/height
        int wName = 0, wOther = 0, h = 0;
        QWidget *w0 = (*it)->getWidget();
        QWidget *w1 = (*it)->getWidName();
        QWidget *w2 = (*it)->getWidDes();
        if (nullptr != w0){
            wOther += w0->size().width();
            if (w0->size().height() > h) h = w0->size().height();
        }
        if (nullptr != w1){
            wName += w1->size().width();
            if (w1->size().height() > h) h = w1->size().height();
        }
        if (nullptr != w2){
            wOther += w2->size().width();
            wOther += iSpanDes;
            if (w2->size().height() > h) h = w2->size().height();
        }

        int l = (*it)->left(), t = (*it)->top();
        map<int, struct SRowItemInfo>::iterator itCol = colWidths.find(l);
        if (itCol != colWidths.end()){
            if (itCol->second.iName < wName) itCol->second.iName = wName;
            if (itCol->second.iWidget < wOther) itCol->second.iWidget = wOther;
        }else {
            colWidths[l] = {wName, wOther};
        }
        map<int, int>::iterator itRow = rowHeights.find(t);
        if (itRow != rowHeights.end()){
            if (itRow->second < h) itRow->second = h;
        }else {
            rowHeights[t] = h;
        }

    }
    int iWidth = iSpanMargin, iHeight = iTitleHeight;
    //assign colInfos and rowInfos
    int n = iSpanMargin;
    map<int, struct SRowItemInfo>::iterator itCol = colWidths.begin();
    for ( ; itCol != colWidths.end(); itCol++){
        int w = n + itCol->second.iName + iSpanName;
        colInfos[itCol->first] = {n, w};
        n = w + itCol->second.iWidget + iSpanItem;
    }
    if (!colWidths.empty()) iWidth = n - iSpanItem + iSpanMargin;
    n = iTitleHeight;
    map<int, int>::iterator itRow = rowHeights.begin();
    for ( ; itRow != rowHeights.end(); itRow++){
        rowInfos[itRow->first] = n;
        n += (itRow->second + iSpanItem);
    }
    if(!rowHeights.empty()) iHeight = n - iSpanItem + iSpanMargin;
    //init every children
    it = m_children.begin();
    for (; it != m_children.end(); it++){
        QWidget *w0 = (*it)->getWidget();
        QWidget *w1 = (*it)->getWidName();
        QWidget *w2 = (*it)->getWidDes();
        int top = rowInfos[(*it)->top()];
        int namePos = colInfos[(*it)->left()].iName;
        int widPos = colInfos[(*it)->left()].iWidget;
        qDebug()<<(*it)->getName()<<": top "<<top;
        if (nullptr != w0){
            w0->move(widPos, top);
            qDebug()<<w0->width()<<","<<w0->height();
            qDebug()<<"l0: "<<widPos;
        }
        if (nullptr != w1){
            w1->move(namePos, top);
            qDebug()<<w1->width()<<","<<w1->height();
            qDebug()<<"l1: "<<namePos;
        }
        if (nullptr != w2){
            w2->move(widPos + w0->width() + iSpanDes, top);
            qDebug()<<w2->width()<<","<<w2->height();
            qDebug()<<"l2: "<<widPos + w0->width() + iSpanDes;
        }
    }
    //init myself
    int iWidthMin = QFontMetrics(m_pWidget->font()).width(m_name);
    if (iWidth < iWidthMin) iWidth = iWidthMin;
    m_pWidget->resize(iWidth, iHeight);
    qDebug()<<"group "<<getName()<<": "<<iWidth<<","<<iHeight;

    return true;
}

void GroupCfgItem::addToPage(UiPage* page)
{
    list<UiCfgItem*>::iterator it = m_children.begin();
    for (; it != m_children.end(); it++){
        GroupCfgItem* pGroup = dynamic_cast<GroupCfgItem*>(*it);
        if (nullptr != pGroup){
            pGroup->addToPage(page);
        }
        QWidget *w0 = (*it)->getWidget();
        QWidget *w1 = (*it)->getWidName();
        QWidget *w2 = (*it)->getWidDes();
        if (nullptr != w0){
            page->addWidget(w0);
        }
        if (nullptr != w1){
            page->addWidget(w1);
        }
        if (nullptr != w2){
            page->addWidget(w2);
        }
    }
    page->addWidget(m_pWidget);
}

bool GroupCfgItem::readXmlFile(QString strFile)
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

UiCfgItem* GroupCfgItem::findItemById(int id)
{
    UiCfgItem *pFind = nullptr;
    list<UiCfgItem*>::iterator it = m_children.begin();
    for (; it != m_children.end(); it++){
        if (id == (*it)->getId()){
            pFind = (*it);
            break;
        }
    }
    return pFind;
}

UiCfgItem* GroupCfgItem::findItemById(const QString& strId)
{
    UiCfgItem *pFind = nullptr;
    if (!strId.isEmpty()){
        QStringList idList = strId.split('/');
        UiCfgItem *item = nullptr;
        GroupCfgItem* grp = this;
        int i = 0;
        int iCnt = idList.count();
        for (; i < iCnt && nullptr!= grp; i++){
            item = grp->findItemById(idList[i].toInt());
            grp = dynamic_cast<GroupCfgItem*>(item);
        }
        if (i == iCnt)
            pFind = item;
    }
    return pFind;
}

//common function
static int getValueIndex(vector<int> &vec, int v)
{
    int iRet = -1;
    int iIdx = 0;
    vector<int>::iterator it = vec.begin();
    for (; it!= vec.end(); it++,iIdx++){
        if (*it == v){
            iRet = iIdx;
            break;
        }
    }

    return iRet;
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
        }
        for (list<UiCfgItem*>::iterator it = m_children.begin(); it !=m_children.end(); it++){
            GroupCfgItem *pGroup = dynamic_cast<GroupCfgItem*>(*it);
            if (nullptr != pGroup)
                pGroup->addToPage(m_uiPage);
        }

        m_uiPage->initTabOrder();
    }
    return m_uiPage;
}

bool PageCfg::initFromDomElement(QDomElement element)
{
    if (UiCfgItem::initFromDomElement(element)){
        if (m_type.isEmpty())
            m_type = UiCfgItem::strTypePage;
        return initChildrenFromDomElement(element.childNodes());
    }
    return false;
}
bool PageCfg::initUi(unsigned short *pStAddr)
{
    for(list<UiCfgItem*>::iterator it = m_children.begin(); it != m_children.end(); it++){
        (*it)->initUi(pStAddr);
    }

    int iSpanItem = 10;
    int iSpanMargin = 4;
    vector<int> vecLeft, vecTop;
    vector<int> vecWidth, vecHeight;
    //layout Group
    if (!m_children.empty()){
        list<UiCfgItem*>::iterator it = m_children.begin();
        for (; it != m_children.end(); it++){
            int l = (*it)->left();
            int t = (*it)->top();
            int w = (*it)->getWidget()->width();
            int h = (*it)->getWidget()->height();
            int iIdx = getValueIndex(vecLeft, l);
            if (-1 != iIdx){
                if (w > vecWidth[iIdx]) vecWidth[iIdx] = w;
            }else {
                vecLeft.push_back(l);
                vecWidth.push_back(w);
            }
            iIdx = getValueIndex(vecTop, t);
            if (-1 != iIdx){
                if (h > vecHeight[iIdx]) vecHeight[iIdx] = h;
            }else {
                vecTop.push_back(t);
                vecHeight.push_back(h);
            }
        }

        it = m_children.begin();
        for (; it != m_children.end(); it++){
            QWidget* w = (*it)->getWidget();
            int l = iSpanMargin, t = iSpanMargin;
            int iIdxCol = getValueIndex(vecLeft, (*it)->left());
            int iIdxRow = getValueIndex(vecTop, (*it)->top());
            w->resize(vecWidth[iIdxCol], vecHeight[iIdxRow]);
            for (int i = 0; i < iIdxCol; i++){
                l += vecWidth[i];
                l += iSpanItem;
            }
            for (int i = 0; i < iIdxRow; i++){
                t += vecHeight[i];
                t += iSpanItem;
            }
            w->move(l, t);
            w->show();
            qDebug()<<(*it)->getName()<<":"<<l<<","<<t<<","<<vecWidth[iIdxCol]<<","<<vecHeight[iIdxRow];
        }
    }

    //set size
    int w = iSpanMargin, h = iSpanMargin;
    for (vector<int>::iterator it = vecWidth.begin(); it != vecWidth.end(); it++){
        w += *it;
    }
    if (!vecWidth.empty())
        w = w + (vecWidth.size() - 1) * iSpanItem;
    w += iSpanMargin;
    for (vector<int>::iterator it = vecHeight.begin(); it != vecHeight.end(); it++){
        h += *it;
    }
    if (!vecHeight.empty())
        h = h + (vecHeight.size() - 1) * iSpanItem;
    h += iSpanMargin;
    m_uiPage->resize(w, h);
    m_uiPage->move(iSpanMargin, iSpanMargin);
    qDebug()<<getName()<<":"<<w<<","<<h;
}

GroupCfgItem* PageCfg::findGroupByName(const QString &strName)
{
    for(list<UiCfgItem*>::iterator it = m_children.begin(); it != m_children.end(); it++){
        GroupCfgItem* pGroup = dynamic_cast<GroupCfgItem*>(*it);
        if (nullptr != pGroup && (strName == pGroup->getName())){
            return pGroup;
        }
    }

    return nullptr;
}

//PageCfgList
PageCfgList::~PageCfgList()
{
    deleteAll();
}
bool PageCfgList::createAllPage(list<UiPage*> &pageList)
{
    initData(0);
    for(list<UiCfgItem*>::iterator it = m_children.begin(); it != m_children.end(); it++){
        PageCfg *page = dynamic_cast<PageCfg*>(*it);
        UiPage *w = page->createPage();
        page->initUi(m_pParamTbl);
        pageList.push_back(w);
    }
    dump();
    return true;
}
