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
#include <math.h>
using namespace std;

QString UiCfgItem::strTypeEdit = "editbox";
QString UiCfgItem::strTypeDevPointEdit = "devpointedit";
QString UiCfgItem::strTypeSetIndexEdit = "setindexeditbox";
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
        if (('0' <= *it && '9' >= *it) || ('a' <= *it && 'z' >= *it) || ('A' <= *it && 'Z' >= *it) || ':' == *it)
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

    setIntValue(m_dataidx, element, "dataidx");
    setIntValue(m_datacnt, element, "datacnt");
    //check value

    setStrValue(m_type, element, "type");
    setStrValue(m_range, element, "range");
    setStrValue(m_defaultVal, element, "defaultvalue");
    QString strEnableSource;
    setStrValue(strEnableSource, element, "enablesource");
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
    if (!m_defaultVal.isEmpty() && -1 < m_dataidx && useDef){
        uint16_t usDef = static_cast<unsigned short>(m_defaultVal.toInt());
        uint16_t *pAddr = paramAddress();
        if (nullptr != pAddr){
            for (int i = 0; i < m_datacnt; i++)
                *(pAddr+i) = usDef;
        }
        qWarning()<<"name "<<m_name<<" address "<<pAddr<<" default "<<usDef;
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
                CKeyDnSetIndexEdit* pSetEdit = dynamic_cast<CKeyDnSetIndexEdit*>(m_pWidget);
                if (nullptr != pSetEdit){
                    pSetEdit->initIndexRange(pMin, pMax);
                }else {
                    if (-1 < m_dataidx) CBinder::BindEdit(pEdit, pAddr+m_dataidx, pMin, pMax, usDef);
                    else qDebug()<<"error addr:"<<pAddr<<dataidx()<<pAddr+m_dataidx;
                }
            }else {
            }
        }
    }

    if (UiCfgItem::strTypeDevPointEdit == m_type){
        unsigned short* pAddr = pStAddr + parent()->dataidx();
        CDevPointEdit* pEdit = dynamic_cast<CDevPointEdit*>(m_pWidget);
        pEdit->setPortType(-1 < m_name.indexOf("输入点")? CDevPosMgr::PORTTYPE_IN : CDevPosMgr::PORTTYPE_OUT);
        CBinder::BindEdit(pEdit, pAddr + m_dataidx, 0);
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
    destItem->m_required = m_required;
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
    if (nullptr != pBox && *paramAddr < pBox->count())
        strValue = pBox->itemText(*paramAddr);
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
    if(!m_defaultVal.isEmpty()){
        CKeyDnEdit* edit = dynamic_cast<CKeyDnEdit*>(m_pWidget);
        if (nullptr != edit){
            edit->setEditText(m_defaultVal);
        }
        CDevPointEdit* edit1 = dynamic_cast<CDevPointEdit*>(m_pWidget);
        if (nullptr != edit1){
            edit1->setEditText(m_defaultVal);
        }
        CKeyDnComboBox* cmb = dynamic_cast<CKeyDnComboBox*>(m_pWidget);
        if (nullptr != cmb){
            cmb->setEditText(m_defaultVal);
        }
    }
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
            QString strFormat = QString::asprintf("%%.%df", m_decimalPlaces);
            strDes = QString::asprintf(strFormat.toStdString().c_str(), m_precision).append(m_unit);
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
    QString strFormat = QString::asprintf("%%.%df", m_decimalPlaces);
    strValue = QString::asprintf(strFormat.toStdString().c_str(), fVal).append(m_unit);

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
    qDebug()<<strInfo;
    return strInfo;
}
QString SetIndexCfgItem::previewInfoEx(int nameLenMax)
{
    return previewInfo();
}
void SetIndexCfgItem::setDefaultVal()
{
    if(!m_defaultVal.isEmpty()){
        //set data to default value
        uint16_t defVal = m_defaultVal.toUShort();
        uint16_t *pAddr = paramAddress();
        for (int i = 0; i < m_datacnt; i++){
            *pAddr++ = defVal;
        }
        CKeyDnSetIndexEdit* edit = dynamic_cast<CKeyDnSetIndexEdit*>(m_pWidget);
        if (nullptr != edit){
            edit->setEditText("1");
        }
    }
}
QString SetIndexCfgItem::getFullName(int idx)
{
    QString fullName = UiCfgItem::getFullName();
    if (m_dataidx <= idx && idx < m_dataidx + m_datacnt){
        int pos = fullName.lastIndexOf(QChar('-'));
        if (-1 < pos) fullName = fullName.left(pos+1);
        QStringList nameList = m_dataNameCfg.split('/');
        int s = (idx - m_dataidx) / m_setSize;
        int p = (idx - m_dataidx) % m_setSize;
        if (3 != nameList.count()) qDebug()<<"error: wrong data name "<<m_dataNameCfg;
        fullName.append(nameList[0] + nameList[1] + QString::number(s+1) + nameList[2] + QString::number(p+1));
    }

    return fullName;
}
QString SetIndexCfgItem::getDataValue(uint16_t *pVal, int *dataCnt)
{
    *dataCnt = 1;
    QStringList pvCfgList = m_previewCfg.split('/');
    if (3 != pvCfgList.count()) qDebug()<<"error: wrong previewconfig "<<m_previewCfg;
    QStringList valList = pvCfgList[2].split(';');
    if (0 == valList.count())  qDebug()<<"error: wrong previewconfig value "<<pvCfgList[2];
    if (*pVal < valList.count())
        return valList[*pVal];
    qDebug()<<"error: wrong value "<<*pVal;
    return "";
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
        for (int i = 0; i < m_datacnt; i++){
            *pAddr++ = defVal;
        }
        CKeyDnComboBoxSet* edit = dynamic_cast<CKeyDnComboBoxSet*>(m_pWidget);
        if (nullptr != edit){
            edit->initUi();
        }
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
