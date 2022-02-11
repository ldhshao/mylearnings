#include "UiCfgItem.h"
#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QDebug>
#include "UiCommon/uistatectl.h"
#include "UiCommon/ckeydnedit.h"
#include "UiCommon/ckeydncombobox.h"
#include "UiCommon/cbinder.h"
#include "UiCommon/cenablemngr.h"
#include "UiCommon/uipage.h"
#include "../cdevpointedit.h"
#include "../cdevposmgr.h"
#include "Util/PageCfg.h"
//#include <QJsonValue>
#include <QStringList>
#include <math.h>
using namespace std;

QString UiCfgItem::strTypeEdit = "editbox";
QString UiCfgItem::strTypeDevPointEdit = "devpointedit";
QString UiCfgItem::strTypeSetIndexEdit = "setindexeditbox";
QString UiCfgItem::strTypeSetEdit  = "seteditbox";
QString UiCfgItem::strTypeCombobox = "combobox";
QString UiCfgItem::strTypeComboboxSet = "combobox";
QString UiCfgItem::strTypeCheckBox = "checkbox";
QString UiCfgItem::strTypeLabel    = "label";
QString UiCfgItem::strTypeGroup    = "groupbox";
QString UiCfgItem::strTypePage     = "page";

int getQstringShowLen(const QString& str)
{
    int len = 0;
    for (QString::const_iterator it = str.begin(); it != str.end(); it++){
        if (('0' <= *it && '9' >= *it) || ('a' <= *it && 'z' >= *it) || ('A' <= *it && 'Z' >= *it)
                || ':' == *it || '.' == *it)
            len++;
        else
            len += 2;
    }

    return len;
}

HNDZ_IMPLEMENT_DYNCREATE(UiCfgItem, XmlItem)
bool UiCfgItem::initFromDomElement(QDomElement element)
{
    XmlItem::initFromDomElement(element);

    //setIntValue(m_dataidx, element, "dataidx");
    setIntValue(m_datacnt, element, "datacnt");
    //check value

    setStrValue(m_type, element, "type");
    setStrValue(m_range, element, "range");
    setStrValue(m_defaultVal, element, "defaultvalue");
    setStrValue(m_paName, element, "paname");
    QString strEnableSource;
    setStrValue(strEnableSource, element, "enablesource");
    setStrValue(m_visibleSource, element, "visiblesource");
    if (!strEnableSource.isEmpty()){
        int pos = strEnableSource.indexOf(':');
        if (-1 < pos){
            m_enableSourceId = strEnableSource.left(pos);
            m_enableSourceVal = strEnableSource.right(strEnableSource.length() - pos -1).toUShort();
        }
    }else{
        m_enableSourceId = "";
    }
    int required = 0;
    setIntValue(required, element, "required");
    m_required = (0 != required);

    //check value

    return true;
}
QString UiCfgItem::getNamePath(int ancestorDepth)
{
    if (0 >= ancestorDepth)
        return m_name;

    QString fullName;
    list<QString> ancestors;
    UiCfgItem* par = parent();
    while (ancestorDepth > 0 && nullptr != par){
        QString parName = par->getName();
        if (parName.isEmpty())
            break;
        ancestors.push_back(parName);
        par = par->parent();
        ancestorDepth--;
    }
    ancestors.reverse();
    for (auto it = ancestors.begin(); it != ancestors.end(); it++)
        fullName.append(*it);
    fullName.append(m_name);

    return fullName;
}
QString UiCfgItem::getFullName()
{
    QString fullName;
    list<QString> ancestors;
    UiCfgItem* par = parent();
    while (nullptr != par){
        QString parName = par->getName();
        if (parName.isEmpty())
            break;
        ancestors.push_back(parName);
        par = par->parent();
    }
    ancestors.reverse();
    for (auto it = ancestors.begin(); it != ancestors.end(); it++)
        fullName.append(*it).append("-");
    fullName.append(m_name);

    return fullName;
}
QString UiCfgItem::getFullName(int idx)
{
    return getFullName();
}
void UiCfgItem::dump()
{
    qDebug()<<"name "<<m_name<<" dataidx "<<m_dataidx<<" count "<<m_datacnt<<" addr "<<paramAddress();
}
void UiCfgItem::create(QWidget* parent)
{
    if (nullptr == m_pWidget){
        bool bGroup = false;
        UiPage* page = dynamic_cast<UiPage*>(parent);
        if (m_type == UiCfgItem::strTypeCheckBox){
            //QCheckBox* pBox = new QCheckBox();
            CStateCheckBox *pBox = new CStateCheckBox(parent);
            m_pWidget = pBox;
        }else if (m_type == UiCfgItem::strTypeEdit){
            //m_pWidget = new CMyLineEdit(parent);
            //m_pWidget = new CStateLineEdit(parent);
            m_pWidget = new CKeyDnEdit(parent);
            QObject::connect(m_pWidget, SIGNAL(sig_valueChanged(uint16_t *, uint32_t)), page, SLOT(slot_valueChanged(uint16_t *, uint32_t )));
        }else if (m_type == UiCfgItem::strTypeDevPointEdit){
            m_pWidget = new CDevPointEdit(parent);
            QObject::connect(m_pWidget, SIGNAL(sig_valueChanged(uint16_t *, uint32_t)), page, SLOT(slot_valueChanged(uint16_t *, uint32_t )));
        }else if (m_type == UiCfgItem::strTypeSetIndexEdit){
            m_pWidget = new CKeyDnSetIndexEdit(parent);
        }else if (m_type == UiCfgItem::strTypeSetEdit){
            m_pWidget = new CKeyDnSetEdit(parent);
        }else if (m_type == UiCfgItem::strTypeCombobox){
            //m_pWidget = new CMyComboBox(parent);
            m_pWidget = new CKeyDnComboBox(parent);
            QObject::connect(m_pWidget, SIGNAL(sig_valueChanged(uint16_t *, uint32_t)), page, SLOT(slot_valueChanged(uint16_t *, uint32_t )));
        }else if (m_type == UiCfgItem::strTypeGroup){
            QGroupBox* pBox = new QGroupBox(parent);
            pBox->setTitle(getName());
            m_pWidget = pBox;
            bGroup = true;
        }
        if (!bGroup){
            if (!m_name.isEmpty()){
                QLabel* pBoxName = new QLabel(parent);
                pBoxName->setAlignment(Qt::AlignLeft);
                pBoxName->setText(getName());
                m_pWidName = pBoxName;
            }
            if (!m_range.isEmpty() && 0 > m_range.indexOf('#')){
                QLabel* pBoxDes = new QLabel(parent);
                pBoxDes->setAlignment(Qt::AlignLeft);
                pBoxDes->setText(m_range);
                m_pWidDes = pBoxDes;
            }
        }
        qDebug()<<m_name<<" wid "<<m_pWidget<<" des "<<m_pWidDes<<" name "<<m_pWidName<<" parent "<<parent;
    }
}
bool UiCfgItem::initData(int idx, bool useDef)
{
    m_dataidx = idx;
    if (!m_defaultVal.isEmpty() && useDef){
        uint16_t usDef = static_cast<unsigned short>(m_defaultVal.toInt());
        uint16_t *pAddr = paramAddress();
        if (nullptr != pAddr){
            for (int i = 0; i < m_datacnt; i++)
                *(pAddr+i) = usDef;
        }
        qWarning()<<"name "<<m_name<<" address "<<pAddr<<" default "<<usDef;
        qWarning()<<"initData name "<<m_name<<" index count "<<m_dataidx<<datacount();
    }
    return true;
}
bool UiCfgItem::initUi(unsigned short* pStAddr, int w, int h)
{
    if (nullptr != m_pWidget){
        m_pWidget->resize(w, h);
        m_pWidget->show();
    }
    if (nullptr != m_pWidName){
        int width = QFontMetrics(m_pWidName->font()).width(m_name);
        m_pWidName->resize(width, h);
        m_pWidName->show();
    }
    if (nullptr != m_pWidDes){
        QLabel *lbl = dynamic_cast<QLabel*>(m_pWidDes);
        int width = QFontMetrics(m_pWidDes->font()).width(lbl->text());
        m_pWidDes->resize(width, h);
        m_pWidDes->show();
    }

    bool badFormat = false;
    unsigned short usMin = 0, usMax = 0, usDef = 0;
    int iPos0 =  m_range.indexOf("(");
    int iPos1 =  m_range.indexOf(")");
    if (-1 < iPos0 && iPos0 < iPos1){
        QString strRange = m_range.mid(iPos0+1, iPos1 - iPos0 - 1).remove(' ');
        int pos = strRange.indexOf('-');
        if (-1 < pos){
            QString strMin = strRange.left(pos).trimmed();
            QString strMax = strRange.right(strRange.length() - pos - 1).trimmed();
            UiCfgItem *pMinItem = nullptr, *pMaxItem = nullptr;
            uint16_t   usMin = 0xFFFF, usMax = 0xFFFF, usDef;
            uint16_t  *pMin = nullptr, *pMax = nullptr;
            GroupCfgItem *pGroup = dynamic_cast<GroupCfgItem*>(m_parent);
            if (strMin[0] == '#' && nullptr != pGroup){
                pMinItem = pGroup->findItemById(strMin.right(strMin.length() - 1));
                pMin = pMinItem->paramAddress();
                CMinValMngr::instance()->registerMinValUi(dynamic_cast<CKeyDnEdit*>(pMinItem->getWidget()), pMin, this);
            }else {
                bool bOk = false;
                usMin = strMin.toUShort(&bOk);
                pMin = Uint16PtrMngr::instance()->registerValue(usMin);
            }
            if (strMax[0] == '#' && nullptr != pGroup){
                pMaxItem = pGroup->findItemById(strMax.right(strMax.length() - 1));
                pMax = pMaxItem->paramAddress();
                CMinValMngr::instance()->registerMinValUi(dynamic_cast<CKeyDnEdit*>(pMaxItem->getWidget()), pMax, this);
            }else {
                bool bOk = false;
                usMax = strMax.toUShort(&bOk);
                pMax = Uint16PtrMngr::instance()->registerValue(usMax);
            }
            usDef = m_defaultVal.toUShort();
            unsigned short* pAddr = pStAddr + parent()->dataidx();
            //qDebug()<<"name "<<m_name<<" address "<<pAddr+m_dataidx;
            CKeyDnEdit* pEdit = dynamic_cast<CKeyDnEdit*>(m_pWidget);
            if (nullptr != pEdit){
                CKeyDnSetIndexEdit* pSetIdxEdit = dynamic_cast<CKeyDnSetIndexEdit*>(m_pWidget);
                CKeyDnSetEdit* pSetEdit = dynamic_cast<CKeyDnSetEdit*>(m_pWidget);
                if (nullptr != pSetIdxEdit){
                    pSetIdxEdit->initIndexRange(pMin, pMax);
                }else if (nullptr != pSetEdit){
                    pSetEdit->initIndexRange(pMin, pMax);
                }else {
                    if (0 == m_datacnt) CBinder::BindEdit(pEdit, nullptr, pMin, pMax, usDef);
                    else if (-1 < m_dataidx) CBinder::BindEdit(pEdit, pAddr+m_dataidx, pMin, pMax, usDef);
                    else qDebug()<<"error addr:"<<pAddr<<dataidx()<<pAddr+m_dataidx;
                }
            }else {
            }
        }else{
            badFormat = true;
        }
    }else{
        badFormat = true;
    }
    if (badFormat){
        unsigned short* pAddr = pStAddr + parent()->dataidx();
        CKeyDnEdit* pEdit = dynamic_cast<CKeyDnEdit*>(m_pWidget);
        if (nullptr != pEdit){
            if (0 < m_datacnt) CBinder::BindEdit(pEdit, pAddr + m_dataidx, nullptr, nullptr, 0);
        }
    }

    if (UiCfgItem::strTypeDevPointEdit == m_type){
        unsigned short* pAddr = pStAddr + parent()->dataidx();
        CDevPointEdit* pEdit = dynamic_cast<CDevPointEdit*>(m_pWidget);
        pEdit->setPortType(-1 < m_name.indexOf("输入点")? CDevPosMgr::PORTTYPE_IN : CDevPosMgr::PORTTYPE_OUT);
        if (0 != m_datacnt) CBinder::BindEdit(pEdit, pAddr + m_dataidx, 0);
        if (2 > m_datacnt) qDebug()<<"error: name "<<m_name<<" with datacount "<<m_datacnt;
    }

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
        UiCfgItem* pVisibleSource = nullptr;
        if (!m_visibleSource.isEmpty()){
            bool bOK = false;
            QStringList tList = m_visibleSource.split(':');
            if (2 == tList.count() && nullptr != (pVisibleSource = pGroup->findItemById(tList[0].trimmed()))){
                tList = tList[1].split(',');
                list<uint16_t> valList;
                foreach (QString val, tList){
                    uint16_t usVal = static_cast<uint16_t>(val.toInt(&bOK));
                    if (bOK) valList.push_back(usVal);
                }
                CKeyDnComboBox* pCmb = dynamic_cast<CKeyDnComboBox*>(pVisibleSource->getWidget());
                if (0 < valList.size() && nullptr != pCmb){
                    CVisibleMngr::instance()->registerVisibleUi(pCmb, pCmb->valuePtr(), &valList, this);
                }
            }
        }
    }
    return true;
}

uint16_t *UiCfgItem::paramAddress()
{
    GroupCfgItem* parent = dynamic_cast<GroupCfgItem*>(this->parent());
    while(nullptr != parent){
        if (nullptr != parent->paramAddress()){
            break;
        }
        parent = dynamic_cast<GroupCfgItem*>(parent->parent());
    }
    if (nullptr == parent)
        return nullptr;

    uint16_t* paramAddr = parent->paramAddress();

    return paramAddr+(this->parent()->dataidx()+m_dataidx);
}

UiCfgItem* UiCfgItem::createMyself()
{
    UiCfgItem* pItem = new UiCfgItem();
    copyTo(pItem);

    return pItem;
}
void UiCfgItem::copyTo(UiCfgItem* destItem)
{
    destItem->m_id = m_id;
    destItem->m_name = m_name;
    destItem->m_dataidx = m_dataidx;
    destItem->m_datacnt = m_datacnt;
    destItem->m_type = m_type;
    destItem->m_range = m_range;
    destItem->m_defaultVal = m_defaultVal;
    destItem->m_enableSourceId = m_enableSourceId;
    destItem->m_enableSourceVal = m_enableSourceVal;
    destItem->m_visibleSource = m_visibleSource;
    destItem->m_required = m_required;
    destItem->m_paName = m_paName;
}

QString UiCfgItem::previewInfo()
{
    if (0 < datacount()){
        QString strInfo(m_name + ": ");
        strInfo.append(strDataValue());

        return strInfo;
    }

    return "";
}
QString UiCfgItem::previewInfoEx(int nameLenMax)
{
    if (0 < datacount()){
        QString strInfo(m_name + ":");
        int tlen = 8;
        int len = getQstringShowLen(strInfo);
        if (len < nameLenMax)
            strInfo.append(QString((nameLenMax - len + tlen - 1)/tlen, QChar('\t')));
        strInfo.append(strDataValue());

        return strInfo;
    }

    return "";
}
QString UiCfgItem::strDataValue(uint16_t *pAddr)
{
    QString strValue;
    uint16_t* paramAddr = pAddr;
    if (nullptr == paramAddr) paramAddr = this->paramAddress();
    CDevPointEdit* pEdit = dynamic_cast<CDevPointEdit*>(m_pWidget);
    if (nullptr != pEdit){
        uint32_t devPt = (*(paramAddr+1) << 16) + *paramAddr;
        strValue = CDevPosMgr::instance()->makeStrDevPoint(devPt);
        return strValue;
    }
    QLineEdit* pEdit1 = dynamic_cast<QLineEdit*>(m_pWidget);
    if (nullptr != pEdit1){
        strValue = QString::number(*paramAddr);
        return strValue;
    }

    QComboBox* pBox = dynamic_cast<QComboBox*>(m_pWidget);
    if (nullptr != pBox){
        int idx = pBox->findData(*paramAddr);
        if (-1 < idx) strValue = pBox->itemText(idx);
    }
    return strValue;
}
//return data value in string; and data count, u16
QString UiCfgItem::getDataValue(uint16_t *pVal, int *dataCnt)
{
    *dataCnt = m_datacnt;
    return strDataValue(pVal);
}
QString UiCfgItem::enableReason()
{
    QString strReason;
    UiCfgItem* pItem = this;
    UiCfgItem* enSource = m_enableSource;
    while (nullptr != pItem){
        if (nullptr != enSource && *enSource->paramAddress() != pItem->m_enableSourceVal){
            strReason.append("原因:");
            CKeyDnComboBox* pCmb = dynamic_cast<CKeyDnComboBox*>(enSource->getWidget());
            GroupCfgItem* grp = dynamic_cast<GroupCfgItem*>(enSource->parent());
            strReason.append(grp->getNamePath(grp->titleDepth()-1)).append(enSource->previewInfo());
            qDebug()<<"enablesource "<<enSource->getNamePath(10)<<" enableval "<<pItem->m_enableSourceVal;
            break;
        }
        pItem = pItem->parent();
        enSource = pItem->m_enableSource;
    }

    return strReason;
}

void UiCfgItem::setDefaultVal()
{
    initData(m_dataidx, true);
    CMyCtl *pCtl = dynamic_cast<CMyCtl*>(getWidget());
    if (nullptr != pCtl) pCtl->updateText();
}
bool UiCfgItem::onMaxValChanged(uint32_t max)
{
    CKeyDnEdit* pEdit = dynamic_cast<CKeyDnEdit*>(m_pWidget);
    if (nullptr != pEdit){
        pEdit->onRangeChanged();
    }

    return true;
}
bool UiCfgItem::onMinValChanged(uint32_t min)
{
    CKeyDnEdit* pEdit = dynamic_cast<CKeyDnEdit*>(m_pWidget);
    if (nullptr != pEdit){
        pEdit->onRangeChanged();
    }

    return true;
}

bool UiCfgItem::isDataOK()
{
    if (m_required){
        CDevPointEdit* edit = dynamic_cast<CDevPointEdit*>(m_pWidget);
        if (nullptr != edit){
            uint16_t *pAddr = paramAddress();
            uint32_t  devPt = (*(pAddr + 1) << 16) + (*pAddr);
            int portType = (-1 < m_name.indexOf("输入点")? CDevPosMgr::PORTTYPE_IN : CDevPosMgr::PORTTYPE_OUT);
            if (!CDevPosMgr::instance()->isDevPointValid(devPt, portType))
                return false;
        }
    }
    return true;
}

bool UiCfgItem::loadFromJsonObject(QJsonObject *obj)
{
    uint16_t* pAddr = paramAddress();
    if (!m_paName.isEmpty() && nullptr != pAddr){
        int valTmp;
        if (1 == m_datacnt){
            if (getJsonObjectIntVal(obj, m_paName, &valTmp)){
                *pAddr = static_cast<uint16_t>(valTmp);
            }
        }else if (2 == m_datacnt){
            //deal device point
            QStringList paNameList = m_paName.split(';');
            if (strTypeDevPointEdit == m_type && 3 == paNameList.count()){
                int l = 0, m = 0, p = 0;
                uint32_t devPoint = 0;
                getJsonObjectIntVal(obj, paNameList[0], &l);
                getJsonObjectIntVal(obj, paNameList[1], &m);
                getJsonObjectIntVal(obj, paNameList[2], &p);
                m -= 1;
                p -= 1;
                devPoint = make_dev_point(l, m, p);
                *pAddr = devPoint;
                *(pAddr + 1) = (devPoint >> 16);
            }else {
                if (getJsonObjectIntVal(obj, m_paName, &valTmp)){
                    *pAddr = valTmp;
                    *(pAddr + 1) = (valTmp >> 16);
                }
            }
        }
    }
    return false;
}
bool UiCfgItem::saveToJsonObject(QJsonObject *obj)
{
    uint16_t* pAddr = paramAddress();
    if (!m_paName.isEmpty() && 0 < m_datacnt && nullptr != pAddr){
        if (1 == m_datacnt)
            setJsonObjectIntVal(obj, m_paName, *pAddr);
        else if (2 == m_datacnt){
            //deal device point
            QStringList paNameList = m_paName.split(';');
            if (strTypeDevPointEdit == m_type && 3 == paNameList.count()){
                uint32_t devPoint = static_cast<uint32_t>((*(pAddr+1) << 16) + *pAddr);
                int portType = (-1 < getName().indexOf("输入点"))? CDevPosMgr::PORTTYPE_IN : CDevPosMgr::PORTTYPE_OUT;
                if (CDevPosMgr::instance()->isDevPointValid(devPoint, portType)){
                    int l = get_line_from_dev_point(devPoint);//from 1
                    int m = get_machine_from_dev_point(devPoint);//from 0
                    int p = get_port_from_dev_point(devPoint);//from 0
                    setJsonObjectIntVal(obj, paNameList[0], l);
                    setJsonObjectIntVal(obj, paNameList[1], m+1);
                    setJsonObjectIntVal(obj, paNameList[2], p+1);
                }
            }else {
                setJsonObjectIntVal(obj, m_paName, (*(pAddr+1) << 16) + *pAddr);
            }
        }
        return true;
    }
    return false;
}

int UiCfgItem::getPortType()
{
    int type = CDevPosMgr::PORTTYPE_CNT;
    if (UiCfgItem::strTypeDevPointEdit == m_type){
        type = (-1 < getName().indexOf("输入点"))? CDevPosMgr::PORTTYPE_IN : CDevPosMgr::PORTTYPE_OUT;
    }

    return type;
}

//ComboboxItem
HNDZ_IMPLEMENT_DYNCREATE(UiCfgItemFloat, UiCfgItem)
bool UiCfgItemFloat::initFromDomElement(QDomElement element)
{
    UiCfgItem::initFromDomElement(element);
    setStrValue(m_unit, element, "unit");
    QString strVal = element.attribute("precision");
    if (!strVal.isEmpty()){
        bool bOK = false;
        m_precision = strVal.toFloat(&bOK);
        float f = m_precision;
        m_decimalPlaces = 0;
        while (fabs(f - 1) > 0.01){
            f = f * 10;
            m_decimalPlaces++;
        }
    }
    qDebug()<<"name "<<m_name<<" precision "<<m_precision<<" decimal "<<m_decimalPlaces;

    return true;
}
UiCfgItem* UiCfgItemFloat::createMyself()
{
    UiCfgItemFloat* pItem = new UiCfgItemFloat();
    copyTo(pItem);
    pItem->m_unit = m_unit;
    pItem->m_precision = m_precision;
    pItem->m_decimalPlaces = m_decimalPlaces;

    return pItem;
}
void UiCfgItemFloat::create(QWidget* parent)
{
    if (nullptr == m_pWidget){
        m_pWidget = new CKeyDnEdit(parent);
        QObject::connect(m_pWidget, SIGNAL(sig_valueChanged(uint16_t *, uint32_t)), dynamic_cast<UiPage*>(parent), SLOT(slot_valueChanged(uint16_t *, uint32_t )));
        if (!m_name.isEmpty()){
            QLabel* pBoxName = new QLabel(parent);
            pBoxName->setAlignment(Qt::AlignLeft);
            pBoxName->setText(getName());
            m_pWidName = pBoxName;
        }
        QLabel* pBoxDes = new QLabel(parent);
        pBoxDes->setAlignment(Qt::AlignLeft);
        QString strDes;
        if (0 < m_decimalPlaces){
            char strFormat[50] = {0}, strTmp[100] = {0};
            sprintf(strFormat, "%%.%df", m_decimalPlaces);
            sprintf(strTmp, strFormat, m_precision);
            strDes.append(strTmp).append(m_unit);
        }else{
            strDes = m_unit;
        }
        if (0 > m_range.indexOf('#')){
            strDes.append(m_range);
        }
        pBoxDes->setText(strDes);
        m_pWidDes = pBoxDes;

        qDebug()<<m_name<<" wid "<<m_pWidget<<" des "<<m_pWidDes<<" name "<<m_pWidName;
    }
}
QString UiCfgItemFloat::strDataValue(uint16_t *pAddr)
{
    QString strValue;
    if (nullptr == pAddr) pAddr = paramAddress();
    float fVal = *pAddr * m_precision;
    char strFormat[50] = {0}, strTmp[100] = {0};
    sprintf(strFormat, "%%.%df", m_decimalPlaces);
    sprintf(strTmp, strFormat, fVal);
    strValue.append(strTmp).append(m_unit);

    return strValue;
}

//SetIndexCfgItem
HNDZ_IMPLEMENT_DYNCREATE(SetIndexCfgItem, UiCfgItem)
bool SetIndexCfgItem::initFromDomElement(QDomElement element)
{
    UiCfgItem::initFromDomElement(element);

    m_type = UiCfgItem::strTypeSetIndexEdit;
    QString strVal = element.attribute("setsize");
    if (!strVal.isEmpty()){
        bool bOK = false;
        uint16_t val = strVal.toUShort(&bOK);
        if (bOK) m_setSize = val;
    }
    strVal = element.attribute("setcount");
    if (!strVal.isEmpty()){
        bool bOK = false;
        uint16_t val = strVal.toUShort(&bOK);
        if (bOK) m_setCnt = val;
    }
    m_datacnt = m_setCnt * m_setSize;
    setStrValue(m_previewCfg, element, "previewconfig");
    setStrValue(m_dataNameCfg, element, "datanameconfig");

    return true;
}
UiCfgItem* SetIndexCfgItem::createMyself()
{
    SetIndexCfgItem* pItem = new SetIndexCfgItem();
    UiCfgItem::copyTo(pItem);
    pItem->m_setSize = this->m_setSize;
    pItem->m_setCnt = this->m_setCnt;
    pItem->m_previewCfg = this->m_previewCfg;
    pItem->m_dataNameCfg = this->m_dataNameCfg;

    return pItem;
}
void SetIndexCfgItem::addItem(UiCfgItem* item)
{
    int idx = 0;
    for(auto it = m_itemList.begin(); it != m_itemList.end(); it++){
        SetItemCfgItem* pSetItem = dynamic_cast<SetItemCfgItem*>(*it);
        if (nullptr != pSetItem){
            idx += pSetItem->itemDataCnt();
        }
    }
    item->setDataidx(idx);
    m_itemList.push_back(item);
}
bool SetIndexCfgItem::initData(int idx, bool useDef)
{
    m_dataidx = idx;
    uint16_t *pBaseAddr = paramAddress();
    if (useDef){
        for(auto it = m_itemList.begin(); it != m_itemList.end(); it++){
            SetItemCfgItem* pSetItem = dynamic_cast<SetItemCfgItem*>(*it);
            if (nullptr != pSetItem && nullptr != pBaseAddr){
                for(int i = 0; i < m_setCnt; i++){
                    uint16_t *pAddr = pBaseAddr + i * m_setSize + pSetItem->dataidx();
                    pSetItem->initDataWithDefault(pAddr);
                }
                qWarning()<<"SetIndexCfg: item index count"<<pSetItem->dataidx()<<pSetItem->itemDataCnt();
            }
        }
    }
    return true;
}
bool SetIndexCfgItem::initUi(unsigned short* pStAddr, int w, int h)
{
    UiCfgItem::initUi(pStAddr, w, h);
    CKeyDnSetIndexEdit* pEdit = dynamic_cast<CKeyDnSetIndexEdit*>(m_pWidget);
    if (nullptr != pEdit){
        pEdit->initData(paramAddress(), m_setSize, m_setCnt);
    }

    return true;
}

QString SetIndexCfgItem::previewInfo()
{
    QString strInfo;
    QStringList pvCfgList = m_previewCfg.split('/');
#if 0
    if (0 < datacount() && 3 == pvCfgList.count()){
        QStringList valList = pvCfgList[2].split(';');
        uint16_t* param = paramAddress();
        int n = 1;
        if (999 < m_setCnt) n = 4;
        else if (99 < m_setCnt) n = 3;
        else if (9 < m_setCnt) n = 2;
        if (0 == valList.count()) qDebug()<<"error: wrong previewconfig "<<m_previewCfg;
        strInfo.append(pvCfgList[0] + ":\n");
        for (int s = 0; s < m_setCnt; s++){
            strInfo.append(QString::number(s+1) + pvCfgList[1] + ": ");
            if ((s+1) == 10 || (s+1) == 100 || (s+1) == 1000 ) n--;
            for (int i = 0; i < 2*(n-1); i++) strInfo.append(" ");//1*'0' == 2*' '
            for (int i = 0; i < m_setSize; i++){
                strInfo.append(QString::number(i+1) + "# ");
                if (*param < valList.count())
                    strInfo.append(valList[*param] + " ");
                else
                    strInfo.append(valList[0] + " ");
                param++;
            }
            strInfo.append("\n");
        }
    }
#endif
    if (0 < datacount() && 2 == pvCfgList.count()){
        uint16_t* param = paramAddress();
        int tlen = 8, tCntMin = 2;;
        vector<int> tCntList;
        QString strTemp = QString::number(m_setCnt)+ pvCfgList[1];
        int iTemp = getQstringShowLen(strTemp);
        iTemp = (iTemp + tlen - 1) / tlen;
        strInfo.append(QString(iTemp, '\t'));
        tCntList.push_back(iTemp);
        for (auto it = m_itemList.begin(); it != m_itemList.end(); it++){
            strTemp = (*it)->getName();
            int tCnt = getQstringShowLen(strTemp);
            if (0 == (tCnt % tlen)) tCnt = tCnt / tlen + 1;
            else tCnt = (tCnt + tlen - 1) / tlen;
            if (tCnt < tCntMin) tCnt = tCntMin;
            tCntList.push_back(tCnt);
            strInfo.append(strTemp);
            strInfo.append(QString(tCnt - getQstringShowLen(strTemp)/tlen, '\t'));
        }
        strInfo.append("\n");
        for (int s = 0; s < m_setCnt; s++){
            strTemp = QString::number(s+1) + pvCfgList[1] + ": ";
            strInfo.append(strTemp);
            strInfo.append(QString(tCntList[0] - getQstringShowLen(strTemp)/tlen, '\t'));
            int i = 1;
            for (auto it = m_itemList.begin(); it != m_itemList.end(); it++, i++){
                uint16_t* pAddr = param + s * m_setSize + (*it)->dataidx();
                strTemp = (*it)->strDataValue(pAddr);
                strInfo.append(strTemp);
                strInfo.append(QString(tCntList[i] - getQstringShowLen(strTemp)/tlen, '\t'));
                qWarning()<<getQstringShowLen(strTemp)<<tCntList[i] - getQstringShowLen(strTemp)/tlen;
            }
            strInfo.append("\n");
        }
    }

    qDebug()<<strInfo;
    return strInfo;
}
QString SetIndexCfgItem::previewInfoEx(int nameLenMax)
{
    return previewInfo();
}
void SetIndexCfgItem::setDefaultVal()
{
    initData(m_dataidx, true);
    CKeyDnSetIndexEdit* edit = dynamic_cast<CKeyDnSetIndexEdit*>(m_pWidget);
    if (nullptr != edit){
        edit->setEditText("1");
    }
}
QString SetIndexCfgItem::getFullName(int idx)
{
    QString fullName = UiCfgItem::getFullName();
    idx -= parent()->dataidx();
    if (m_dataidx <= idx && idx < m_dataidx + m_datacnt){
        int pos = fullName.lastIndexOf(QChar('-'));
        if (-1 < pos) fullName = fullName.left(pos+1);
        QStringList nameList = m_dataNameCfg.split('/');
        int s = (idx - m_dataidx) / m_setSize;
        int p = (idx - m_dataidx) % m_setSize;
        if (2 != nameList.count()) qDebug()<<"error: wrong data name "<<m_dataNameCfg;
        fullName.append(nameList[0] + QString::number(s+1) + nameList[1]);
        for (auto it = m_itemList.begin(); it != m_itemList.end(); it++){
            if (p == (*it)->dataidx()){
                fullName.append((*it)->getName());
            }
        }
    }

    return fullName;
}
QString SetIndexCfgItem::getDataValue(uint16_t *pVal, int *dataCnt)
{
    *dataCnt = 1;
    QStringList pvCfgList = m_previewCfg.split('/');
    if (2 != pvCfgList.count()) qDebug()<<"error: wrong previewconfig "<<m_previewCfg;
    uint16_t *pBaseAddr = paramAddress();
    int internalIdx = ((pVal - pBaseAddr) % m_setSize);
    for (auto it = m_itemList.begin(); it != m_itemList.end(); it++){
        if (internalIdx == (*it)->dataidx()){
            SetItemCfgItem* pSetItem = dynamic_cast<SetItemCfgItem*>(*it);
            if (nullptr != pSetItem) *dataCnt = pSetItem->itemDataCnt();
            return (*it)->strDataValue(pVal);
        }
    }

    qDebug()<<"error: wrong value "<<*pVal;
    return "";
}

void SetIndexCfgItem::findPortInfoByPortType(int line, int portType, list<pair<uint32_t,QString>> *infoList, bool chkVal)
{
    if (CDevPosMgr::instance()->isValidPortType(portType)){
        uint16_t* pBaseAddr = paramAddress();
        for (auto it = m_itemList.begin(); it != m_itemList.end(); it++){
            if ((*it)->getPortType() == portType){
                for(int s=0; s < m_setCnt; s++){
                    uint16_t* pAddr = pBaseAddr + s * m_setSize + (*it)->dataidx();
                    uint32_t  devPt = (*(pAddr + 1) << 16) + *pAddr;
                    int l = get_line_from_dev_point(devPt) - 1;
                    QString strName = (*it)->getFullName() + QString::number(s+1);
                    if (chkVal){
                        if (l == line && CDevPosMgr::instance()->isDevPointValid(devPt, portType))
                            infoList->push_back(pair<uint32_t, QString>(devPt, strName));
                    }else if(l == line){
                        infoList->push_back(pair<uint32_t, QString>(devPt, strName));
                    }

                }
            }
        }
    }
}

bool SetIndexCfgItem::loadFromJsonObject(QJsonObject *obj)
{
    uint16_t* pBaseAddr = paramAddress();
    for (int s = 0; s < m_setCnt; s++){
#ifdef USE_JSON_SRC
        if (!obj->exists(m_paName + "_" + QString::number(s))) continue;
        QJsonObject* childObj = obj->get(m_paName +  "_" + QString::number(s));
        for (auto it = m_itemList.begin(); it != m_itemList.end(); it++){
            uint16_t* pAddr = pBaseAddr + s * m_setSize + (*it)->dataidx();
            SetItemCfgItem* pSetItem = dynamic_cast<SetItemCfgItem*>(*it);
            if (nullptr != pSetItem){
                uint8_t dataCnt = pSetItem->itemDataCnt();
                int tmpVal;
                if (1 == dataCnt){
                    if(getJsonObjectIntVal(childObj, (*it)->paramName(), &tmpVal))
                        *pAddr = tmpVal;
                }else if (2 == dataCnt){
                    QStringList paNameList = (*it)->paramName().split(';');
                    if ((*it)->isType(strTypeDevPointEdit) && 3 == paNameList.count()){
                        int l = 0, m = 0, p = 0;
                        uint32_t devPoint = 0;
                        getJsonObjectIntVal(childObj, paNameList[0], &l);
                        getJsonObjectIntVal(childObj, paNameList[1], &m);
                        getJsonObjectIntVal(childObj, paNameList[2], &p);
                        if (0 < m) m--;
                        if (0 < p) p--;
                        devPoint = make_dev_point(l, m, p);
                        *pAddr = devPoint;
                        *(pAddr + 1) = (devPoint >> 16);
                    }else{
                        if(getJsonObjectIntVal(childObj, (*it)->paramName(), &tmpVal)){
                            *pAddr = tmpVal;
                            *(pAddr+1) = (tmpVal >> 16);
                        }
                    }
                }
            }
        }
#else
        QJsonValue  valTmp = obj->value(m_paName + QString::asprintf("_%d", s));
        if (valTmp.isUndefined()) continue;
        QJsonObject childObj = valTmp.toObject();
        for (auto it = m_itemList.begin(); it != m_itemList.end(); it++){
            uint16_t* pAddr = pBaseAddr + s * m_setSize + (*it)->dataidx();
            SetItemCfgItem* pSetItem = dynamic_cast<SetItemCfgItem*>(*it);
            if (nullptr != pSetItem){
                uint8_t dataCnt = pSetItem->itemDataCnt();
                valTmp = childObj.value((*it)->paramName());
                if (1 == dataCnt){
                    if (!valTmp.isUndefined())
                        *pAddr = static_cast<uint16_t>(valTmp.toInt());
                }else if (2 == dataCnt){
                    QStringList paNameList = (*it)->paramName().split(';');
                    if ((*it)->isType(strTypeDevPointEdit) && 3 == paNameList.count()){
                        int l = 0, m = 0, p = 0;
                        uint32_t devPoint = 0;
                        valTmp = childObj.value(paNameList[0]);
                        if (!valTmp.isUndefined())
                            l = valTmp.toInt(l);
                        valTmp = childObj.value(paNameList[1]);
                        if (!valTmp.isUndefined())
                            m = valTmp.toInt(m) - 1;
                        valTmp = childObj.value(paNameList[2]);
                        if (!valTmp.isUndefined())
                            p = valTmp.toInt(p) - 1;
                        devPoint = make_dev_point(l, m, p);
                        *pAddr = devPoint;
                        *(pAddr + 1) = (devPoint >> 16);
                    }else{
                        if (!valTmp.isUndefined()){
                        uint32_t val = static_cast<uint32_t>(valTmp.toInt());
                        *pAddr = val;
                        *(pAddr+1) = (val >> 16);
                        }
                    }
                }
            }
        }
#endif
    }
    return true;
}
bool SetIndexCfgItem::saveToJsonObject(QJsonObject *obj)
{
    uint16_t* pBaseAddr = paramAddress();
    for (int s = 0; s < m_setCnt; s++){
#ifdef USE_JSON_SRC
        QJsonObject *childObj = obj->addObject(m_paName + "_" + QString::number(s));
        for (auto it = m_itemList.begin(); it != m_itemList.end(); it++){
            uint16_t* pAddr = pBaseAddr + s * m_setSize + (*it)->dataidx();
            SetItemCfgItem* pSetItem = dynamic_cast<SetItemCfgItem*>(*it);
            if (nullptr != pSetItem){
                uint8_t dataCnt = pSetItem->itemDataCnt();
                if (1 == dataCnt){
                    childObj->add((*it)->paramName(), *pAddr);
                }else if (2 == dataCnt){
                    int val = (*(pAddr+1)<<16) + *pAddr;
                    QStringList paNameList = (*it)->paramName().split(';');
                    if ((*it)->isType(strTypeDevPointEdit) && 3 == paNameList.count()){
                        int portType = (-1 < (*it)->getName().indexOf("输入点"))? CDevPosMgr::PORTTYPE_IN : CDevPosMgr::PORTTYPE_OUT;
                        if (!CDevPosMgr::instance()->isDevPointValid(val, portType)) continue;

                        int l = get_line_from_dev_point(val);//from 1
                        int m = get_machine_from_dev_point(val);//from 0
                        int p = get_port_from_dev_point(val);//from 0
                        childObj->add(paNameList[0], l);
                        childObj->add(paNameList[1], m+1);
                        childObj->add(paNameList[2], p+1);
                    }else{
                        childObj->add((*it)->paramName(), val);
                    }
                }
            }
        }
#else
        QJsonObject childObj;
        for (auto it = m_itemList.begin(); it != m_itemList.end(); it++){
            uint16_t* pAddr = pBaseAddr + s * m_setSize + (*it)->dataidx();
            SetItemCfgItem* pSetItem = dynamic_cast<SetItemCfgItem*>(*it);
            if (nullptr != pSetItem){
                uint8_t dataCnt = pSetItem->itemDataCnt();
                if (1 == dataCnt){
                    childObj.insert((*it)->paramName(), QJsonValue(*pAddr));
                }else if (2 == dataCnt){
                    int val = (*(pAddr+1)<<16) + *pAddr;
                    QStringList paNameList = (*it)->paramName().split(';');
                    if ((*it)->isType(strTypeDevPointEdit) && 3 == paNameList.count()){
                        int portType = (-1 < (*it)->getName().indexOf("输入点"))? CDevPosMgr::PORTTYPE_IN : CDevPosMgr::PORTTYPE_OUT;
                        if (!CDevPosMgr::instance()->isDevPointValid(val, portType)) continue;

                        int l = get_line_from_dev_point(val);//from 1
                        int m = get_machine_from_dev_point(val);//from 0
                        int p = get_port_from_dev_point(val);//from 0
                        childObj.insert(paNameList[0], QJsonValue(l));
                        childObj.insert(paNameList[1], QJsonValue(m+1));
                        childObj.insert(paNameList[2], QJsonValue(p+1));
                    }else{
                        childObj.insert((*it)->paramName(), QJsonValue(val));
                    }
                }
            }
        }
        obj->insert(m_paName + QString::asprintf("_%d", s), childObj);
#endif
    }
    return true;
}

//ComboboxItem
HNDZ_IMPLEMENT_DYNCREATE(ComboboxCfgItem, UiCfgItem)
bool ComboboxCfgItem::initFromDomElement(QDomElement element)
{
    UiCfgItem::initFromDomElement(element);
    if (m_type.isEmpty())
        m_type = UiCfgItem::strTypeCombobox;
    QString strParams;
    setStrValue(strParams, element, "params");
    qDebug()<<strParams;
    //check value
    m_params = strParams.split(";");
    qDebug()<<m_params;

    return true;
}
UiCfgItem* ComboboxCfgItem::createMyself()
{
    ComboboxCfgItem* pItem = new ComboboxCfgItem();
    copyTo(pItem);

    return pItem;
}
void ComboboxCfgItem::copyTo(UiCfgItem* destItem)
{
    UiCfgItem::copyTo(destItem);
    ComboboxCfgItem* pCmbItem = dynamic_cast<ComboboxCfgItem*>(destItem);
    if (nullptr != pCmbItem){
        pCmbItem->m_params = this->m_params;
    }
}

//bool ComboboxCfgItem::init()
//{
//    if (UiCfgItem::init() && !m_params.isEmpty()){
//        QComboBox *pBox = dynamic_cast<QComboBox*>(m_pWidget);
//        if (nullptr != pBox){
//            pBox->insertItems(0, m_params);
//        }
//    }
//
//    return true;
//}
bool ComboboxCfgItem::initUi(unsigned short *pStAddr, int w, int h)
{
    UiCfgItem::initUi(pStAddr, w, h);

    CKeyDnComboBox* pBox = dynamic_cast<CKeyDnComboBox*>(m_pWidget);
    if (nullptr != pBox && -1 < m_dataidx){
        unsigned short* pAddr = pStAddr + parent()->dataidx();
        CBinder::BindComboBox(pBox, pAddr+m_dataidx, m_params, m_defaultVal);
        qDebug()<<"name "<<getName()<<" addr "<<pAddr+m_dataidx<<" value "<<*(pAddr+m_dataidx);
    }
}

//ComboboxSetCfgItem
HNDZ_IMPLEMENT_DYNCREATE(ComboboxSetCfgItem, ComboboxCfgItem)
bool ComboboxSetCfgItem::initFromDomElement(QDomElement element)
{
    ComboboxCfgItem::initFromDomElement(element);
    m_type = UiCfgItem::strTypeComboboxSet;
    setStrValue(m_setIndexSource, element, "setindexsource");
    setStrValue(m_previewCfg, element, "previewconfig");
    setStrValue(m_setSize, element, "setsize");

    return true;
}
UiCfgItem* ComboboxSetCfgItem::createMyself()
{
    ComboboxSetCfgItem* pItem = new ComboboxSetCfgItem();
    ComboboxCfgItem::copyTo(pItem);
    pItem->m_setIndexSource = this->m_setIndexSource;
    pItem->m_previewCfg = this->m_previewCfg;
    pItem->m_setSize = this->m_setSize;

    return pItem;
}

void ComboboxSetCfgItem::create(QWidget* parent)
{
    if (nullptr == m_pWidget){
        UiPage* page = dynamic_cast<UiPage*>(parent);
        CKeyDnComboBoxSet *pCmb = nullptr;
        CIndexLabel *pIdxLbl = nullptr;
        if (m_type == UiCfgItem::strTypeComboboxSet){
            pCmb = new CKeyDnComboBoxSet(parent);
            QObject::connect(m_pWidget, SIGNAL(sig_valueChanged(uint16_t *, uint32_t)), page, SLOT(slot_valueChanged(uint16_t *, uint32_t )));
            m_pWidget = pCmb;
        }
        if (!m_name.isEmpty()){
            pIdxLbl = new CIndexLabel(parent);
            pIdxLbl->setDataName(m_name);
            pIdxLbl->setAlignment(Qt::AlignLeft);
            QObject::connect(pCmb, SIGNAL(sig_dataIndexChanged(int)), pIdxLbl, SLOT(slot_dataIndexChanged(int)));
            m_pWidName = pIdxLbl;
        }
        if (!m_range.isEmpty() && 0 > m_range.indexOf('#')){
            QLabel* pBoxDes = new QLabel(parent);
            pBoxDes->setAlignment(Qt::AlignLeft);
            pBoxDes->setText(m_range);
            m_pWidDes = pBoxDes;
        }
        qDebug()<<m_name<<" wid "<<m_pWidget<<" des "<<m_pWidDes<<" name "<<m_pWidName<<" parent "<<parent;
    }
}
bool ComboboxSetCfgItem::initUi(unsigned short *pStAddr, int w, int h)
{
    UiCfgItem::initUi(pStAddr, w, h);
    QString strMaxLenName = m_name + QString::number(m_datacnt);
    int width = QFontMetrics(m_pWidName->font()).width(strMaxLenName);
    m_pWidName->resize(width, h);
    qDebug()<<m_pWidName<<m_pWidName->width()<<m_pWidName->size().width();

    CKeyDnComboBoxSet* pBox = dynamic_cast<CKeyDnComboBoxSet*>(m_pWidget);
    if (nullptr != pBox){
        unsigned short* pAddr = pStAddr + parent()->dataidx();
        pBox->addItems(m_params);
        qDebug()<<"name "<<getName()<<" addr "<<pAddr+m_dataidx<<" value "<<*(pAddr+m_dataidx);
        GroupCfgItem *pGroup = dynamic_cast<GroupCfgItem*>(m_parent);
        if (nullptr!= pGroup){
            UiCfgItem* pSetSource = nullptr;
            if (nullptr != (pSetSource = pGroup->findItemById(m_setIndexSource))){
                CKeyDnSetIndexEdit* pEdit = dynamic_cast<CKeyDnSetIndexEdit*>(pSetSource->getWidget());
                QObject::connect(pEdit, SIGNAL(sig_dataSetChanged(uint16_t*, uint16_t)), pBox, SLOT(slot_dataSetChanged(uint16_t*, uint16_t)));
                pSetIndexSource = pSetSource;
            }else if (-1 < m_dataidx){
                pBox->initData(pAddr + m_dataidx, m_datacnt);
            }
        }
        //connect set size
        if (!m_setSize.isEmpty()){
            bool bOK = false;
            int pos = -1;
            uint16_t setsize = m_setSize.toUShort(&bOK);
            if (bOK && setsize <= m_datacnt){
                pBox->initData(pAddr + m_dataidx, setsize);
            }else if(-1 < (pos = m_setSize.indexOf("#"))){
                QString maxID = m_setSize.right(m_setSize.length() - pos -1);
                UiCfgItem *pMaxItem = pGroup->findItemById(maxID);
                if (nullptr != pMaxItem){
                    CMaxValMngr::instance()->registerMaxValUi(dynamic_cast<CKeyDnEdit*>(pMaxItem->getWidget()), pMaxItem->paramAddress(), this);
                    pBox->initData(pAddr + m_dataidx, *(pMaxItem->paramAddress()));
                }
            }
        }
    }
    return true;
}

QString ComboboxSetCfgItem::previewInfo()
{
    QString strInfo;
    QStringList pvCfgList = m_previewCfg.split('/');//section name/port name/ port view group/values

    if (0 < datacount() && 4 == pvCfgList.count()){
        QStringList valList = pvCfgList[3].split(';');
        int grpSize = pvCfgList[2].toInt();
        uint16_t* param = paramAddress();
        int n = 1;
        if (999 < m_datacnt) n = 4;
        else if (99 < m_datacnt) n = 3;
        else if (9 < m_datacnt) n = 2;
        if (0 == valList.count()) qDebug()<<"error: wrong previewconfig "<<m_previewCfg;
        strInfo.append(pvCfgList[0] + ":\n");
        for (int p = 0; p < m_datacnt; p+= grpSize){
            for (int i = 0; i < grpSize; i++){
                int k = p+i+1;
                if (k > m_datacnt) break;
                if (k == 10 || k == 100 || k == 1000 ) n--;
                //for (int j = 0; j < (n-1); j++) strInfo.append("   ");//1*'0' == 2*' '
                //strInfo.append(QString::number(k) + "#" + pvCfgList[1] + "\t");
                strInfo.append(QString::number(k) + "#" + pvCfgList[1] + " ");
                for (int j = 0; j < (n-1); j++) strInfo.append("  ");//1*'0' == 2*' '
                if (*param < valList.count())
                    strInfo.append(valList[*param] + "\t");
                else
                    strInfo.append(valList[0] + "\t");
                param++;
            }
            strInfo.append("\n");
        }
    }
    qDebug()<<strInfo;
    return strInfo;
}
QString ComboboxSetCfgItem::previewInfoEx(int nameLenMax)
{
    return previewInfo();
}
void ComboboxSetCfgItem::setDefaultVal()
{
    if(0 < datacount() && !m_defaultVal.isEmpty()){
        //set data to default value
    }
}
QString ComboboxSetCfgItem::getFullName(int idx)
{
    QString fullName = UiCfgItem::getFullName();
    int baseIdx = parent()->dataidx() + m_dataidx;
    if (baseIdx <= idx && idx < baseIdx + m_datacnt){
        int pos = fullName.lastIndexOf(QChar('-'));
        if (-1 < pos) fullName = fullName.left(pos+1);
        fullName.append(m_name + QString::number(idx - baseIdx + 1));
    }

    return fullName;
}
QString ComboboxSetCfgItem::getDataValue(uint16_t *pVal, int *dataCnt)
{
    *dataCnt = 1;
    if (0 < m_params.count()){
        if (*pVal < m_params.count())
            return m_params[*pVal];
        else
            qDebug()<<"error: wrong value"<<m_name<<*pVal;
    }
    return "";
}

bool ComboboxSetCfgItem::onMaxValChanged(uint32_t max)
{
    CKeyDnComboBoxSet *pCmb = dynamic_cast<CKeyDnComboBoxSet*>(m_pWidget);
    if (nullptr != pCmb){
        uint16_t oldDataCnt = pCmb->dataCount();
        if (oldDataCnt > max){
            //restor default value for some data
            uint16_t defVal = m_params.count();
            for (int i = 0; i < m_params.count(); i++){
                if (m_defaultVal == m_params[i]){
                    defVal = i;
                    break;
                }
            }
            if (defVal >= m_params.count())
                defVal = 0;

            uint16_t *pAddr = paramAddress();
            for (int i = max; i < oldDataCnt; i++){
                pAddr[i] = defVal;
            }
        }
        pCmb->onDataCountChanged(max);
    }
    return true;
}

//EditSetCfgItem
HNDZ_IMPLEMENT_DYNCREATE(EditSetCfgItem, UiCfgItem)
bool EditSetCfgItem::initFromDomElement(QDomElement element)
{
    UiCfgItem::initFromDomElement(element);
    m_type = UiCfgItem::strTypeSetEdit;
    setIntValue(m_setSize, element, "setsize");
    m_datacnt = m_setSize;

    return true;
}
UiCfgItem* EditSetCfgItem::createMyself()
{
    EditSetCfgItem* pItem = new EditSetCfgItem();
    UiCfgItem::copyTo(pItem);
    pItem->m_setSize = this->m_setSize;

    return pItem;
}

bool EditSetCfgItem::initUi(unsigned short *pStAddr, int w, int h)
{
    UiCfgItem::initUi(pStAddr, w, h);
    CKeyDnSetEdit *pEdit = dynamic_cast<CKeyDnSetEdit*>(m_pWidget);
    if (nullptr != pEdit){
        pEdit->initData(paramAddress(), m_setSize);
    }

    return true;
}

QString EditSetCfgItem::previewInfo()
{
    QString strInfo;
    return strInfo;
}
QString EditSetCfgItem::previewInfoEx(int nameLenMax)
{
    QString strInfo, strValue;
    CKeyDnSetEdit * pEdit = dynamic_cast<CKeyDnSetEdit*>(m_pWidget);
    uint16_t *pAddr = paramAddress();
    if (nullptr != pEdit && nullptr != pAddr){
        int grpCnt = 10;
        for (int i = 0; i < m_setSize; i++){
            if (pEdit->isValid(pAddr[i])){
                int intIdx = i % grpCnt;
                if (0 < intIdx) strValue.append(",\t");
                strValue.append(QString::number(pAddr[i]));
                if (grpCnt-1 == intIdx)strValue.append(",\n");
            }else
                break;
        }
    }
    if (strValue.isEmpty()){
        int tlen = 8;
        strInfo = m_name + ":";
        int len = getQstringShowLen(strInfo);
        if (len < nameLenMax)
            strInfo.append(QString((nameLenMax - len + tlen - 1)/tlen, QChar('\t')));
        strInfo.append("无");
    }else {
        strInfo.append(m_name + ":\n").append(strValue);
    }
    return strInfo;
}
QString EditSetCfgItem::getDataValue(uint16_t *pVal, int *dataCnt)
{
    QString val;
    uint16_t *pBaseAddr = paramAddress();
    if (pBaseAddr <= pVal && pVal < pBaseAddr + m_setSize){
        int idx = pVal - pBaseAddr;
        *dataCnt = m_setSize - idx;
        CKeyDnSetEdit * pEdit = dynamic_cast<CKeyDnSetEdit*>(m_pWidget);
        if (nullptr != pEdit && nullptr != pBaseAddr){
            for (int i = 0; i < m_setSize; i++){
                if (pEdit->isValid(pBaseAddr[i])){
                    if (0 < i)val.append(",");
                    val.append(QString::number(pBaseAddr[i]));
                }else
                    break;
            }
        }
    }
    return val;
}
bool EditSetCfgItem::loadFromJsonObject(QJsonObject *obj)
{
    uint16_t *pBaseAddr = paramAddress();
    if (!m_paName.isEmpty() && nullptr != pBaseAddr){
        for (int i = 0; i < m_setSize; i++){
            int tmpVal;
            if (getJsonObjectIntVal(obj, m_paName + "_" + QString::number(i), &tmpVal)){
                pBaseAddr[i] = tmpVal;
            }
        }
    }
    return true;
}
bool EditSetCfgItem::saveToJsonObject(QJsonObject *obj)
{
    uint16_t *pBaseAddr = paramAddress();
    CKeyDnSetEdit * pEdit = dynamic_cast<CKeyDnSetEdit*>(m_pWidget);
    if (!m_paName.isEmpty() && nullptr != pEdit && nullptr != pBaseAddr){
        for (int i = 0; i < m_setSize; i++){
            if (pEdit->isValid(pBaseAddr[i])){
                setJsonObjectIntVal(obj, m_paName + " " + QString::number(i), pBaseAddr[i]);
            }else
                break;
        }
    }
    return true;
}

//SetItemCfgItem
HNDZ_IMPLEMENT_DYNCREATE(SetItemCfgItem, UiCfgItem)
bool SetItemCfgItem::initFromDomElement(QDomElement element)
{
    UiCfgItem::initFromDomElement(element);

    int dataCnt = m_itemDataCnt;
    setIntValue(dataCnt, element, "itemdatacnt");
    m_itemDataCnt = static_cast<uint8_t>(dataCnt);
    setStrValue(m_setIndexSource, element, "setindexsource");

    return true;
}
UiCfgItem* SetItemCfgItem::createMyself()
{
    SetItemCfgItem* pItem = new SetItemCfgItem();
    copyTo(pItem);

    return pItem;
}
void SetItemCfgItem::copyTo(UiCfgItem* destItem)
{
    UiCfgItem::copyTo(destItem);
    SetItemCfgItem* pItem = dynamic_cast<SetItemCfgItem*>(destItem);
    if (nullptr != pItem){
        pItem->m_itemDataCnt = m_itemDataCnt;
        pItem->m_setIndexSource = m_setIndexSource;
    }
}
bool SetItemCfgItem::initData(int idx, bool useDef)
{
    GroupCfgItem *pGroup = dynamic_cast<GroupCfgItem*>(m_parent);
    if (nullptr!= pGroup){
        UiCfgItem* pSetSource = nullptr;
        if (nullptr != (pSetSource = pGroup->findItemById(m_setIndexSource))){
            m_pSetIndexSource = pSetSource;
        }
    }
    SetIndexCfgItem* pSetIdx = dynamic_cast<SetIndexCfgItem*>(m_pSetIndexSource);
    if (nullptr != pSetIdx)
        pSetIdx->addItem(this);

    return true;
}
bool SetItemCfgItem::initDataWithDefault(uint16_t* pAddr)
{
    uint16_t usDef = static_cast<unsigned short>(m_defaultVal.toInt());
    if (nullptr != pAddr){
        for (int i = 0; i < m_itemDataCnt; i++){
            *(pAddr + i) = usDef;
        }
    }
    return true;
}
bool SetItemCfgItem::initUi(unsigned short* pStAddr, int w, int h)
{
    if (nullptr != m_pWidget){
        m_pWidget->resize(w, h);
        m_pWidget->show();
    }
    if (nullptr != m_pWidName){
        int width = QFontMetrics(m_pWidName->font()).width(m_name);
        m_pWidName->resize(width, h);
        m_pWidName->show();
    }
    if (nullptr != m_pWidDes){
        QLabel *lbl = dynamic_cast<QLabel*>(m_pWidDes);
        int width = QFontMetrics(m_pWidDes->font()).width(lbl->text());
        m_pWidDes->resize(width, h);
        m_pWidDes->show();
    }

    unsigned short usMin = 0, usMax = 0, usDef = 0;
    int iPos0 =  m_range.indexOf("(");
    int iPos1 =  m_range.indexOf(")");
    if (-1 < iPos0 && iPos0 < iPos1){
        QString strRange = m_range.mid(iPos0+1, iPos1 - iPos0 - 1).remove(' ');
        int pos = strRange.indexOf('-');
        if (-1 < pos){
            QString strMin = strRange.left(pos).trimmed();
            QString strMax = strRange.right(strRange.length() - pos - 1).trimmed();
            UiCfgItem *pMinItem = nullptr, *pMaxItem = nullptr;
            uint16_t   usMin = 0xFFFF, usMax = 0xFFFF, usDef;
            uint16_t  *pMin = nullptr, *pMax = nullptr;
            GroupCfgItem *pGroup = dynamic_cast<GroupCfgItem*>(m_parent);
            if (strMin[0] == '#' && nullptr != pGroup){
                pMinItem = pGroup->findItemById(strMin.right(strMin.length() - 1));
                pMin = pMinItem->paramAddress();
                CMinValMngr::instance()->registerMinValUi(dynamic_cast<CKeyDnEdit*>(pMinItem->getWidget()), pMin, this);
            }else {
                bool bOk = false;
                usMin = strMin.toUShort(&bOk);
                pMin = Uint16PtrMngr::instance()->registerValue(usMin);
            }
            if (strMax[0] == '#' && nullptr != pGroup){
                pMaxItem = pGroup->findItemById(strMax.right(strMax.length() - 1));
                pMax = pMaxItem->paramAddress();
                CMinValMngr::instance()->registerMinValUi(dynamic_cast<CKeyDnEdit*>(pMaxItem->getWidget()), pMax, this);
            }else {
                bool bOk = false;
                usMax = strMax.toUShort(&bOk);
                pMax = Uint16PtrMngr::instance()->registerValue(usMax);
            }
            usDef = m_defaultVal.toUShort();
            //qDebug()<<"name "<<m_name<<" address "<<pAddr+m_dataidx;
            CKeyDnEdit* pEdit = dynamic_cast<CKeyDnEdit*>(m_pWidget);
            if (nullptr != pEdit){
                CBinder::BindEdit(pEdit, nullptr, pMin, pMax, usDef);
            }
        }
    }

    if (nullptr != m_pSetIndexSource){
        //bind data
        unsigned short* pAddr = pStAddr + m_pSetIndexSource->parent()->dataidx() + m_pSetIndexSource->dataidx();
        CDevPointEdit* pDevPtEdit = dynamic_cast<CDevPointEdit*>(m_pWidget);
        CKeyDnEdit* pEdit = dynamic_cast<CKeyDnEdit*>(m_pWidget);
        if (nullptr != pDevPtEdit){
            pDevPtEdit->setPortType(-1 < m_name.indexOf("输入点")? CDevPosMgr::PORTTYPE_IN : CDevPosMgr::PORTTYPE_OUT);
            pDevPtEdit->setItemIndex(m_dataidx);
            pDevPtEdit->setValuePtr(pAddr + m_dataidx);
            QObject::connect(dynamic_cast<CKeyDnSetIndexEdit*>(m_pSetIndexSource->getWidget()), SIGNAL(sig_dataSetChanged(uint16_t*, uint16_t)), pDevPtEdit, SLOT(slot_dataSetChanged(uint16_t*, uint16_t)));
        }else if (nullptr != pEdit){
            pEdit->setItemIndex(m_dataidx);
            pEdit->bindDataPtr(pAddr + m_dataidx);
            QObject::connect(dynamic_cast<CKeyDnSetIndexEdit*>(m_pSetIndexSource->getWidget()), SIGNAL(sig_dataSetChanged(uint16_t*, uint16_t)), pEdit, SLOT(slot_dataSetChanged(uint16_t*, uint16_t)));
        }
    }
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
        UiCfgItem* pVisibleSource = nullptr;
        if (!m_visibleSource.isEmpty()){
            bool bOK = false;
            QStringList tList = m_visibleSource.split(':');
            if (2 == tList.count() && nullptr != (pVisibleSource = pGroup->findItemById(tList[0].trimmed()))){
                tList = tList[1].split(',');
                list<uint16_t> valList;
                foreach (QString val, tList){
                    uint16_t usVal = static_cast<uint16_t>(val.toInt(&bOK));
                    if (bOK) valList.push_back(usVal);
                }
                CKeyDnComboBox* pCmb = dynamic_cast<CKeyDnComboBox*>(pVisibleSource->getWidget());
                if (0 < valList.size() && nullptr != pCmb){
                    SetItemCfgItem* pItemVisSource = dynamic_cast<SetItemCfgItem*>(pVisibleSource);
                    if (nullptr != pItemVisSource){
                        SetIndexCfgItem* pVisSourceSetIndex = dynamic_cast<SetIndexCfgItem*>(pItemVisSource->getSetIndexSource());
                        uint16_t setSize = pVisSourceSetIndex->setSize();
                        uint16_t setCnt = pVisSourceSetIndex->setCount();
                        uint16_t *pDataAddr = pCmb->valuePtr();
                        for (int i = 0; i < setCnt; i++){
                            CVisibleMngr::instance()->registerVisibleUi(pCmb, pDataAddr+i*setSize, &valList, this);
                        }
                    }else {
                        CVisibleMngr::instance()->registerVisibleUi(pCmb, pCmb->valuePtr(), &valList, this);
                    }
                }
            }
        }
    }

    return true;
}

void SetItemCfgItem::setDefaultVal()
{
    //nothine; setindex has done
}
//SetItemCfgItemFloat
HNDZ_IMPLEMENT_DYNCREATE(SetItemCfgItemFloat, UiCfgItem)
bool SetItemCfgItemFloat::initFromDomElement(QDomElement element)
{
    SetItemCfgItem::initFromDomElement(element);
    setStrValue(m_unit, element, "unit");
    QString strVal = element.attribute("precision");
    if (!strVal.isEmpty()){
        bool bOK = false;
        m_precision = strVal.toFloat(&bOK);
        float f = m_precision;
        m_decimalPlaces = 0;
        while (fabs(f - 1) > 0.01){
            f = f * 10;
            m_decimalPlaces++;
        }
    }
    qDebug()<<"name "<<m_name<<" precision "<<m_precision<<" decimal "<<m_decimalPlaces;

    return true;
}
UiCfgItem* SetItemCfgItemFloat::createMyself()
{
    SetItemCfgItemFloat* pItem = new SetItemCfgItemFloat();
    copyTo(pItem);
    pItem->m_unit = m_unit;
    pItem->m_precision = m_precision;
    pItem->m_decimalPlaces = m_decimalPlaces;

    return pItem;
}
void SetItemCfgItemFloat::create(QWidget* parent)
{
    if (nullptr == m_pWidget){
        m_pWidget = new CKeyDnEdit(parent);
        QObject::connect(m_pWidget, SIGNAL(sig_valueChanged(uint16_t *, uint32_t)), dynamic_cast<UiPage*>(parent), SLOT(slot_valueChanged(uint16_t *, uint32_t )));
        if (!m_name.isEmpty()){
            QLabel* pBoxName = new QLabel(parent);
            pBoxName->setAlignment(Qt::AlignLeft);
            pBoxName->setText(getName());
            m_pWidName = pBoxName;
        }
        QLabel* pBoxDes = new QLabel(parent);
        pBoxDes->setAlignment(Qt::AlignLeft);
        QString strDes;
        if (0 < m_decimalPlaces){
            char strFormat[50] = {0}, strTmp[100] = {0};
            sprintf(strFormat, "%%.%df", m_decimalPlaces);
            sprintf(strTmp, strFormat, m_precision);
            strDes.append(strTmp).append(m_unit);
        }else{
            strDes = m_unit;
        }
        if (0 > m_range.indexOf('#')){
            strDes.append(m_range);
        }
        pBoxDes->setText(strDes);
        m_pWidDes = pBoxDes;

        qDebug()<<m_name<<" wid "<<m_pWidget<<" des "<<m_pWidDes<<" name "<<m_pWidName;
    }
}
QString SetItemCfgItemFloat::strDataValue(uint16_t *pAddr)
{
    QString strValue;
    if (nullptr == pAddr) pAddr = paramAddress();
    float fVal = *pAddr * m_precision;
    char strFormat[50] = {0}, strTmp[100] = {0};
    sprintf(strFormat, "%%.%df", m_decimalPlaces);
    sprintf(strTmp, strFormat, fVal);
    strValue.append(strTmp).append(m_unit);
    return strValue;
}

//ComboboxSetItemCfgItem
HNDZ_IMPLEMENT_DYNCREATE(ComboboxSetItemCfgItem, UiCfgItem)
bool ComboboxSetItemCfgItem::initFromDomElement(QDomElement element)
{
    SetItemCfgItem::initFromDomElement(element);
    m_type = UiCfgItem::strTypeCombobox;
    QString strParams;
    setStrValue(strParams, element, "params");
    qDebug()<<strParams;
    //check value
    m_params = strParams.split(";");
    qDebug()<<m_params;

    return true;
}
UiCfgItem* ComboboxSetItemCfgItem::createMyself()
{
    ComboboxSetItemCfgItem* pItem = new ComboboxSetItemCfgItem();
    copyTo(pItem);
    pItem->m_params = m_params;

    return pItem;
}

bool ComboboxSetItemCfgItem::initUi(unsigned short *pStAddr, int w, int h)
{
    CKeyDnComboBox* pBox = dynamic_cast<CKeyDnComboBox*>(m_pWidget);
    if (nullptr != pBox && -1 < m_dataidx){
        if (nullptr != m_pSetIndexSource){
            uint16_t* pAddr = pStAddr + m_pSetIndexSource->parent()->dataidx() + m_pSetIndexSource->dataidx();
            pBox->setItemIndex(m_dataidx);
            CBinder::BindComboBox(pBox, pAddr+m_dataidx, m_params, m_defaultVal);
            QObject::connect(dynamic_cast<CKeyDnSetIndexEdit*>(m_pSetIndexSource->getWidget()), SIGNAL(sig_dataSetChanged(uint16_t*, uint16_t)), pBox, SLOT(slot_dataSetChanged(uint16_t*, uint16_t)));
            qDebug()<<"name "<<getName()<<" addr "<<pAddr+m_dataidx<<" value "<<*(pAddr+m_dataidx);
        }
    }

    SetItemCfgItem::initUi(pStAddr, w, h);
}
