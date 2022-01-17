#include "PageCfg.h"
#include "UiCommon/uipage.h"
#include "UiCommon/ckeydncombobox.h"
#include "UiCommon/cenablemngr.h"
#include <QGroupBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
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
            iCnt += (*it)->datacount();
        else
            iCnt += pGroup->getDataCount();
    }

    return iCnt;
}

bool GroupCfgItem::initFromDomElement(QDomElement element)
{
    if (UiCfgItem::initFromDomElement(element)){
        setIntValue(m_titleDepth, element, "titledepth");
        setIntValue(m_ctlWidth, element, "controlwidth");
        setIntValue(m_ctlHeight, element, "controlheight");
        setIntValue(m_ctlUnitSize, element, "unitsize");
        int displayMode = 1;
        setIntValue(displayMode, element, "columnmode");
        m_colMode = (1==displayMode);
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
    copyTo(pList);
    pList->m_titleDepth = m_titleDepth;
    pList->m_ctlWidth = m_ctlWidth;
    pList->m_ctlHeight = m_ctlHeight;
    pList->m_colMode = m_colMode;
    pList->m_ctlUnitSize = m_ctlUnitSize;

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

bool GroupCfgItem::initData(int idx, bool useDef)
{
    m_dataidx = idx;

    int cnt = 0;
    list<UiCfgItem*>::iterator it = m_children.begin();
    for(; it != m_children.end(); it++){
        (*it)->initData(idx, useDef);
        cnt += (*it)->datacount();
        idx += (*it)->datacount();
    }
    m_datacnt = cnt;
    //check data conflict
    //chkDataConflict();

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
    UiPage* page = new UiPage(parent);
    QVariant var;
    var.setValue<void*>(this);
    page->setProperty("device", var);
    //page->setTitle(getName());
    m_pWidget = page;
    list<UiCfgItem*>::iterator it = m_children.begin();
    for (; it != m_children.end(); it++){
        (*it)->create(m_pWidget);
    }
    page->initTabOrder();
}

//version 003
bool GroupCfgItem::initUi(unsigned short* pStAddr, int w, int h)
{
    //deal enable souce for page
    GroupCfgItem *pGroup = dynamic_cast<GroupCfgItem*>(m_parent);
    if (nullptr!= pGroup){
        UiCfgItem* pEnSource = nullptr;
        if (nullptr != (pEnSource = pGroup->findItemById(m_enableSourceId))){
            CKeyDnComboBox* pCmb = dynamic_cast<CKeyDnComboBox*>(pEnSource->getWidget());
            if(nullptr != pCmb){
                CEnableMngr::instance()->registerEableUi(pCmb, pCmb->valuePtr(), m_enableSourceVal, this);
            }
            m_enableSource = pEnSource;
        }
    }

    //only for group
    int iSpanName = 4;
    int iSpanDes = 2;
    int iSpanItem = 10;
    int iSpanMargin = 4;
    int iTitleHeight = 80;
    //find cols and rows
    //count max width and max height
    struct SRowItemInfo{
        int iName;
        int iWidget;
    };

    map<int, struct SRowItemInfo> colWidths;
    int i = 0;
    list<UiCfgItem*>::iterator it = m_children.begin();
    for (i=0; it != m_children.end(); it++,i++){
        (*it)->initUi(pStAddr, m_ctlWidth, m_ctlHeight);

        //width/height
        int wName = 0, wOther = 0, h = 0;
        QWidget *w0 = (*it)->getWidget();
        QWidget *w1 = (*it)->getWidName();
        QWidget *w2 = (*it)->getWidDes();
        if (nullptr != w0){
            wOther += w0->size().width();
        }
        if (nullptr != w1){
        qDebug()<<w1<<w1->width()<<w1->size().width();
            wName += w1->size().width();
        }
        if (nullptr != w2){
            wOther += w2->size().width();
            wOther += iSpanDes;
        }

        int grp = i / m_ctlUnitSize;
        if (0 == (i % m_ctlUnitSize)){
            colWidths[grp] = {wName, wOther};
        }else {
            if (wName > colWidths[grp].iName) colWidths[grp].iName = wName;
            if (wOther > colWidths[grp].iWidget) colWidths[grp].iWidget = wOther;
        }
    }
    int iWidth = iSpanMargin, iHeight = iTitleHeight;
    //assign colInfos and rowInfos
    int left = iSpanMargin;
    //init every children
    for (it = m_children.begin(), i= 0;it != m_children.end(); it++, i++){
        QWidget *w0 = (*it)->getWidget();
        QWidget *w1 = (*it)->getWidName();
        QWidget *w2 = (*it)->getWidDes();
        int top = iTitleHeight + (i % m_ctlUnitSize) * (iSpanMargin + m_ctlHeight);
        int grp = i /m_ctlUnitSize;
        qDebug()<<(*it)->getName()<<": top "<<top;
        if (nullptr != w0){
            w0->move(left + colWidths[grp].iName + iSpanName, top);
            qDebug()<<w0->width()<<","<<w0->height();
            qDebug()<<"l0: "<<w0->pos();
        }
        if (nullptr != w1){
            w1->move(left, top);
            qDebug()<<w1->width()<<","<<w1->height();
            qDebug()<<"l1: "<<w1->pos();
        }
        if (nullptr != w2){
            w2->move(w0->pos().x() + w0->width() + iSpanDes, top);
            qDebug()<<w2->width()<<","<<w2->height();
            qDebug()<<"l2: "<<w2->pos();
        }

        if (m_ctlUnitSize -1 == i % m_ctlUnitSize){
            left += (colWidths[grp].iName + iSpanName + colWidths[grp].iWidget + iSpanItem);
        }
    }
    //init myself
    int iWidthMin = QFontMetrics(m_pWidget->font()).width(m_name);
    for(auto itW = colWidths.begin(); itW != colWidths.end(); itW++){
        iWidth += (itW->second.iName + iSpanName + itW->second.iWidget);
        if (itW != colWidths.begin()) iWidth += iSpanItem;
    }
    iWidth += iSpanMargin;
    if (iWidth < iWidthMin) iWidth = iWidthMin;
    iHeight += (i > m_ctlUnitSize)? (m_ctlUnitSize*(iSpanMargin + m_ctlHeight)) : (i * (iSpanMargin + m_ctlHeight));
    qDebug()<<"group "<<getName()<<": "<<iWidth<<","<<iHeight;
    UiPage* page = dynamic_cast<UiPage*>(m_pWidget);
    if (nullptr != page){
        page->setTitle(getNamePath(m_titleDepth-1));
        page->setTitleHeight(iTitleHeight);
        page->setInitWidthHeight(iWidth, iHeight);
        for (it = m_children.begin(), i= 0;it != m_children.end(); it++, i++){
            QWidget *w0 = (*it)->getWidget();
            QWidget *w1 = (*it)->getWidName();
            QWidget *w2 = (*it)->getWidDes();
            int grp = i / m_ctlUnitSize;
            if (nullptr != w0) page->fillColList(grp, w0);
            if (nullptr != w1) page->fillColList(grp, w1);
            if (nullptr != w2) page->fillColList(grp, w2);
        }
    }

    return true;
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
            if (".." == idList[i]){
                grp = dynamic_cast<GroupCfgItem*>(grp->parent());
            }else {
                item = grp->findItemById(idList[i].toInt());
                grp = dynamic_cast<GroupCfgItem*>(item);
            }
        }
        if (i == iCnt)
            pFind = item;
        else{
            qDebug()<<"error: wrong enable source id "<<strId;
        }
    }
    return pFind;
}

UiCfgItem* GroupCfgItem::findItemByName(const QString& strName)
{
    UiCfgItem* pFind = nullptr;
    if (m_name == strName) return this;
    for(auto it = m_children.begin(); it != m_children.end(); it++){
        GroupCfgItem* grp = dynamic_cast<GroupCfgItem*>(*it);
        if (nullptr != grp){
            pFind = grp->findItemByName(strName);
        }else if ((*it)->getName() == strName){
            pFind = *it;
        }
        if (nullptr != pFind)
            break;
    }

    return pFind;
}

UiCfgItem* GroupCfgItem::findItemByDataIdx(int dataidx)
{
    UiCfgItem *pFind = nullptr;
    list<UiCfgItem*>::iterator it = m_children.begin();
    for (; it != m_children.end(); it++){
        //if (dataidx == m_dataidx + (*it)->dataidx()){
        if (m_dataidx + (*it)->dataidx() <= dataidx && dataidx < m_dataidx + (*it)->dataidx() + (*it)->datacount()){
            pFind = (*it);
            break;
        }
    }
    return pFind;
}

uint16_t* GroupCfgItem::firstParamAddress()
{
    uint16_t *pAddr = nullptr;
    GroupCfgItem* parent = this;
    while(nullptr != parent){
        if (nullptr != (pAddr = parent->paramAddress())){
            break;
        }
        parent = dynamic_cast<GroupCfgItem*>(parent->parent());
    }
    if (nullptr == parent)
        return nullptr;

    return pAddr + m_dataidx;
}

void GroupCfgItem::createPage(list<UiPage*> &pageList)
{
    //create(nullptr);
    UiPage* page = dynamic_cast<UiPage*>(m_pWidget);
    if (nullptr != page)
        pageList.push_back(page);
    for (list<UiCfgItem*>::iterator it = m_children.begin(); it !=m_children.end(); it++){
        GroupCfgItem* grp = dynamic_cast<GroupCfgItem*>(*it);
        if (nullptr != grp)
            grp->createPage(pageList);
    }
}

QString GroupCfgItem::previewInfo()
{
    QString strInfo;
    for (list<UiCfgItem*>::iterator it = m_children.begin(); it !=m_children.end(); it++){
        strInfo.append((*it)->previewInfo() + "\n");
    }
    return strInfo;
}

QString GroupCfgItem::previewInfo(int col)
{
    QString strInfo;
    int i = 0, max = 0, tlen = 8;
    for (list<UiCfgItem*>::iterator it = m_children.begin(); it !=m_children.end(); it++, i++){
        int len = getQstringShowLen((*it)->getName())+1;
        qWarning()<<i<<len;
        if ((i / m_ctlUnitSize) == col && max < len)
            max = len;
    }
    max = (max + tlen -1) / tlen * tlen;
    i = 0;
    for (list<UiCfgItem*>::iterator it = m_children.begin(); it !=m_children.end(); it++, i++){
        if ((i / m_ctlUnitSize) == col)
            strInfo.append((*it)->previewInfoEx(max) + "\n");
    }
    return strInfo;
}

bool GroupCfgItem::readJsonFile(QString strFile)
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

    QJsonParseError parseError;
    QJsonDocument doc=QJsonDocument::fromJson(file.readAll(),&parseError);
    file.close();
    if(parseError.error!=QJsonParseError::NoError){
        return false;
    }

    QJsonObject obj=doc.object().begin()->toObject();
    loadFromJsonObject(&obj);
    qDebug()<<"load successfully 1";
    qWarning()<<"load successfully 2";

    return true;
}
bool GroupCfgItem::loadFromJsonObject(QJsonObject *obj)
{
    for(auto it = m_children.begin(); it != m_children.end(); it++){
        GroupCfgItem* grp = dynamic_cast<GroupCfgItem*>(*it);
        if (nullptr != grp){
            QString childName = (*it)->paramName();
            if (!childName.isEmpty() && obj->contains(childName)){
                QJsonObject childObj = obj->value(childName).toObject();
                (*it)->loadFromJsonObject(&childObj);
            }else {
                (*it)->loadFromJsonObject(obj);
            }
        }else{
            (*it)->loadFromJsonObject(obj);
        }
    }
    return true;
}
bool GroupCfgItem::saveJsonFile(QString strFile)
{
    QFile file(strFile);
    if(!file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    QJsonObject root;
    QJsonObject obj;
    saveToJsonObject(&obj);
    root.insert(m_paName, obj);
    QJsonDocument doc(root);
    file.write(doc.toJson());
    file.close();
    qDebug()<<"save successfully 1";
    qWarning()<<"save successfully 2";

    return true;
}
bool GroupCfgItem::saveToJsonObject(QJsonObject *obj)
{
    for(auto it = m_children.begin(); it != m_children.end(); it++){
        GroupCfgItem* grp = dynamic_cast<GroupCfgItem*>(*it);
        if (nullptr != grp){
            QJsonObject childObj;
            QString childName = (*it)->paramName();
            if (!childName.isEmpty()){
                (*it)->saveToJsonObject(&childObj);
                obj->insert(childName, childObj);
            }else {
                (*it)->saveToJsonObject(obj);
            }
        }else{
            (*it)->saveToJsonObject(obj);
        }
    }
    return true;
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
HNDZ_IMPLEMENT_DYNCREATE(GroupCfgList, GroupCfgItem)
GroupCfgList::~GroupCfgList()
{
    GroupCfgItem::deleteAll();
}

void GroupCfgList::create(QWidget* parent)
{
    Q_UNUSED(parent)

    for(list<UiCfgItem*>::iterator it = m_children.begin(); it != m_children.end(); it++){
        (*it)->create(0);
    }
}
void GroupCfgList::addEnableSource(CKeyDnComboBox* pCmb, uint16_t val)
{
    for (auto it = m_children.begin(); it != m_children.end(); it++){
        //CEnableMngr::instance()->registerEableUi(pCmb, pCmb->valuePtr(), val, *it);
    }
}

bool GroupCfgList::initUi(unsigned short* pStAddr, int w, int h)
{
    //deal enable souce for page
    GroupCfgItem *pGroup = dynamic_cast<GroupCfgItem*>(m_parent);
    if (nullptr!= pGroup){
        UiCfgItem* pEnSource = nullptr;
        if (nullptr != (pEnSource = pGroup->findItemById(m_enableSourceId))){
            CKeyDnComboBox* pCmb = dynamic_cast<CKeyDnComboBox*>(pEnSource->getWidget());
            if(nullptr != pCmb){
                addEnableSource(pCmb, m_enableSourceVal);
            }
            m_enableSource = pEnSource;
        }
    }

    for(list<UiCfgItem*>::iterator it = m_children.begin(); it != m_children.end(); it++){
        (*it)->initUi(pStAddr);
    }

    return true;
}

UiCfgItem* GroupCfgList::createMyself()
{
    GroupCfgList* pList = new GroupCfgList();
    copyTo(pList);

    copyChildren(pList);

    return pList;
}
GroupCfgItem* GroupCfgList::findGroupByName(const QString &strName)
{
    for(list<UiCfgItem*>::iterator it = m_children.begin(); it != m_children.end(); it++){
        GroupCfgItem* pGroup = dynamic_cast<GroupCfgItem*>(*it);
        if (nullptr != pGroup && (strName == pGroup->getName())){
            return pGroup;
        }
    }

    return nullptr;
}

UiCfgItem* GroupCfgList::findItemByDataIdx(int dataidx)
{
    UiCfgItem *pFind = nullptr;
    list<UiCfgItem*>::iterator it = m_children.begin();
    for (; it != m_children.end(); it++){
        GroupCfgItem* grp = dynamic_cast<GroupCfgItem*>(*it);
        if (nullptr != grp){
            UiCfgItem* item = grp->findItemByDataIdx(dataidx);
            if (nullptr != item){
                pFind = item;
                break;
            }
        }
    }
    return pFind;
}
//PageCfgList
PageCfgList::~PageCfgList()
{
    deleteAll();
}
bool PageCfgList::createAllPage(list<UiPage*> &pageList)
{
    create(nullptr);
    for(list<UiCfgItem*>::iterator it = m_children.begin(); it != m_children.end(); it++){
        GroupCfgItem *page = dynamic_cast<GroupCfgItem*>(*it);
        page->createPage(pageList);
        page->initUi(m_pParamTbl);
    }
    dump();
    return true;
}
