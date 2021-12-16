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
#include "Util/PageCfg.h"
using namespace std;

QString UiCfgItem::strTypeEdit = "editbox";
QString UiCfgItem::strTypeDevPointEdit = "devpointedit";
QString UiCfgItem::strTypeCombobox = "combobox";
QString UiCfgItem::strTypeCheckBox = "checkbox";
QString UiCfgItem::strTypeLabel    = "label";
QString UiCfgItem::strTypeGroup    = "groupbox";
QString UiCfgItem::strTypePage     = "page";

HNDZ_IMPLEMENT_DYNCREATE(UiCfgItem, XmlItem)
bool UiCfgItem::initFromDomElement(QDomElement element)
{
    XmlItem::initFromDomElement(element);

    setIntValue(m_col, element, "col");
    setIntValue(m_row, element, "row");
    setIntValue(m_width, element, "width");
    setIntValue(m_height, element, "height");
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
        if (nullptr != m_pWidget) m_pWidget->move(m_col, m_row);
        if (nullptr != m_pWidName) m_pWidName->move(m_col, m_row);
        if (nullptr != m_pWidDes) m_pWidDes->move(m_col, m_row);
        qDebug()<<m_name<<" wid "<<m_pWidget<<" des "<<m_pWidDes<<" name "<<m_pWidName<<" parent "<<parent;
    }
}
bool UiCfgItem::initData(int idx, bool useDef)
{
    if (!m_defaultVal.isEmpty() && -1 < m_dataidx && useDef){
        uint16_t usDef = static_cast<unsigned short>(m_defaultVal.toInt());
        uint16_t *pAddr = paramAddress();
        if (nullptr != pAddr){
            *pAddr = usDef;
        }
        qDebug()<<"name "<<m_name<<" address "<<pAddr<<" default "<<usDef;
    }
    return true;
}
bool UiCfgItem::initUi(unsigned short* pStAddr)
{
    if (nullptr != m_pWidget){
        m_pWidget->resize(m_width, m_height);
        m_pWidget->show();
    }
    if (nullptr != m_pWidName){
        int width = QFontMetrics(m_pWidName->font()).width(m_name);
        m_pWidName->resize(width, height());
        m_pWidName->show();
    }
    if (nullptr != m_pWidDes){
        QLabel *lbl = dynamic_cast<QLabel*>(m_pWidDes);
        int width = QFontMetrics(m_pWidDes->font()).width(lbl->text());
        m_pWidDes->resize(width, height());
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
            GroupCfgItem *pGroup = dynamic_cast<GroupCfgItem*>(m_parent);
            if (strMin[0] == '#' && nullptr != pGroup){
                pMinItem = pGroup->findItemById(strMin.right(strMin.length() - 1));
            }else {
                bool bOk = false;
                usMin = strMin.toUShort(&bOk);
            }
            if (strMax[0] == '#' && nullptr != pGroup){
                pMaxItem = pGroup->findItemById(strMax.right(strMax.length() - 1));
            }else {
                bool bOk = false;
                usMax = strMax.toUShort(&bOk);
            }
            usDef = m_defaultVal.toUShort();
            unsigned short* pAddr = pStAddr + parent()->dataidx();
            qDebug()<<"name "<<m_name<<" address "<<pAddr+m_dataidx;
            if (nullptr != pMinItem || nullptr != pMaxItem){
                CKeyDnEdit* pEdit = dynamic_cast<CKeyDnEdit*>(m_pWidget);
                if (nullptr != pMinItem){
                    if (nullptr == pMaxItem)
                        CBinder::BindEdit(pEdit, pAddr+m_dataidx, pMinItem->paramAddress(), usMax, usDef);
                    else
                        CBinder::BindEdit(pEdit, pAddr+m_dataidx, pMinItem->paramAddress(), pMaxItem->paramAddress(), usDef);
                }else {
                    CBinder::BindEdit(pEdit, pAddr+m_dataidx, usMin, pMaxItem->paramAddress(), usDef);
                }

            }else{
                CKeyDnEdit* pEdit = dynamic_cast<CKeyDnEdit*>(m_pWidget);
                CBinder::BindEdit(pEdit, pAddr+m_dataidx, usMin, usMax, usDef);
            }
        }
    }

    if (UiCfgItem::strTypeDevPointEdit == m_type){
        unsigned short* pAddr = pStAddr + parent()->dataidx();
        CDevPointEdit* pEdit = dynamic_cast<CDevPointEdit*>(m_pWidget);
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
    pItem->m_id = m_id;
    pItem->m_name = m_name;
    pItem->m_col = m_col;
    pItem->m_row = m_row;
    pItem->m_width = m_width;
    pItem->m_height = m_height;
    pItem->m_dataidx = m_dataidx;
    pItem->m_datacnt = m_datacnt;
    pItem->m_type = m_type;
    pItem->m_range = m_range;
    pItem->m_defaultVal = m_defaultVal;
    pItem->m_enableSourceId = m_enableSourceId;
    pItem->m_enableSourceVal = m_enableSourceVal;

    return pItem;
}

QString UiCfgItem::previewInfo()
{
    QString strInfo(m_name + ": ");
    strInfo.append(strDataValue());

    return strInfo;
}

QString UiCfgItem::strDataValue()
{
    QString strValue;
    QLineEdit* pEdit = dynamic_cast<QLineEdit*>(m_pWidget);
    if (nullptr != pEdit)
        strValue = pEdit->text();
    else{
        QComboBox* pBox = dynamic_cast<QComboBox*>(m_pWidget);
        if (nullptr != pBox)
            strValue = pBox->currentText();
    }
    return strValue;
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
    pItem->m_id = m_id;
    pItem->m_name = m_name;
    pItem->m_col = m_col;
    pItem->m_row = m_row;
    pItem->m_width = m_width;
    pItem->m_height = m_height;
    pItem->m_dataidx = m_dataidx;
    pItem->m_datacnt = m_datacnt;
    pItem->m_type = m_type;
    pItem->m_range = m_range;
    pItem->m_defaultVal = m_defaultVal;
    pItem->m_enableSourceId = m_enableSourceId;
    pItem->m_enableSourceVal = m_enableSourceVal;
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

        if (nullptr != m_pWidget) m_pWidget->move(m_col, m_row);
        if (nullptr != m_pWidName) m_pWidName->move(m_col, m_row);
        if (nullptr != m_pWidDes) m_pWidDes->move(m_col, m_row);
        qDebug()<<m_name<<" wid "<<m_pWidget<<" des "<<m_pWidDes<<" name "<<m_pWidName;
    }
}
//bool UiCfgItemFloat::initUi(unsigned short *pStAddr)
//{
//    UiCfgItem::initUi(pStAddr);
//
//}
QString UiCfgItemFloat::strDataValue()
{
    QString strValue;
    uint16_t* param = paramAddress();
    if (nullptr != param){
        if (0 == *param || 0 == m_decimalPlaces){
            strValue.append(QString::asprintf("%d", *param));
        }else {
            QString strFormat = QString::asprintf("%%.%df", m_decimalPlaces);
            QString strDes = QString::asprintf(strFormat.toStdString().c_str(), (*param)*m_precision);
            strValue.append(strDes);
        }
        strValue.append(m_unit);
    }else{
        qDebug()<<"error: name "<<m_name<<" param address";
    }

    return strValue;
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
    pItem->m_id = m_id;
    pItem->m_name = m_name;
    pItem->m_col = m_col;
    pItem->m_row = m_row;
    pItem->m_width = m_width;
    pItem->m_height = m_height;
    pItem->m_dataidx = m_dataidx;
    pItem->m_datacnt = m_datacnt;
    pItem->m_type = m_type;
    pItem->m_defaultVal = m_defaultVal;
    pItem->m_params = m_params;
    pItem->m_enableSourceId = m_enableSourceId;
    pItem->m_enableSourceVal = m_enableSourceVal;

    return pItem;
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
bool ComboboxCfgItem::initUi(unsigned short *pStAddr)
{
    UiCfgItem::initUi(pStAddr);

    CKeyDnComboBox* pBox = dynamic_cast<CKeyDnComboBox*>(m_pWidget);
    if (nullptr != pBox){
        unsigned short* pAddr = pStAddr + parent()->dataidx();
        CBinder::BindComboBox(pBox, pAddr+m_dataidx, m_params, m_defaultVal);
        qDebug()<<"name "<<getName()<<" addr "<<pAddr+m_dataidx<<" value "<<*(pAddr+m_dataidx);
    }
}
