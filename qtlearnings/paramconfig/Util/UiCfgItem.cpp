#include "UiCfgItem.h"
#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <stack>
#include <QDebug>
#include "UiCommon/uistatectl.h"
#include "UiCommon/ckeydnedit.h"
#include "UiCommon/ckeydncombobox.h"
#include "UiCommon/cbinder.h"
#include "UiCommon/cenablemngr.h"
#include "Util/PageCfg.h"
using namespace std;

QString UiCfgItem::strTypeEdit = "editbox";
QString UiCfgItem::strTypeCombobox = "combobox";
QString UiCfgItem::strTypeCheckBox = "checkbox";
QString UiCfgItem::strTypeLabel    = "label";
QString UiCfgItem::strTypeGroup    = "groupbox";
QString UiCfgItem::strTypePage     = "page";

HNDZ_IMPLEMENT_DYNCREATE(UiCfgItem, XmlItem)
bool UiCfgItem::initFromDomElement(QDomElement element)
{
    XmlItem::initFromDomElement(element);

    setIntValue(m_left, element, "left");
    setIntValue(m_top, element, "top");
    setIntValue(m_width, element, "width");
    setIntValue(m_height, element, "height");
    setIntValue(m_dataidx, element, "dataidx");
    //check value

    setStrValue(m_type, element, "type");
    setStrValue(m_description, element, "description");
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
void UiCfgItem::dump()
{
    qDebug()<<"name "<<m_name<<" dataidx "<<m_dataidx<<" addr "<<paramAddress();
}
void UiCfgItem::create(QWidget* parent)
{
    if (nullptr == m_pWidget){
        bool bGroup = false;
        if (m_type == UiCfgItem::strTypeCheckBox){
            //QCheckBox* pBox = new QCheckBox();
            CStateCheckBox *pBox = new CStateCheckBox(parent);
            m_pWidget = pBox;
        }else if (m_type == UiCfgItem::strTypeEdit){
            m_pWidget = new CMyLineEdit(parent);
            //m_pWidget = new CStateLineEdit(parent);
            //m_pWidget = new CKeyDnEdit(parent);
        }else if (m_type == UiCfgItem::strTypeCombobox){
            m_pWidget = new CMyComboBox(parent);
            //m_pWidget = new CKeyDnComboBox(parent);
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
            if (!m_description.isEmpty()){
                QLabel* pBoxDes = new QLabel(parent);
                pBoxDes->setAlignment(Qt::AlignLeft);
                pBoxDes->setText(m_description);
                m_pWidDes = pBoxDes;
            }
        }
    }
}
bool UiCfgItem::init()
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
        int width = QFontMetrics(m_pWidDes->font()).width(m_description);
        m_pWidDes->resize(width, height());
        m_pWidDes->show();
    }

    return true;
}
bool UiCfgItem::initData(unsigned short* pStAddr)
{
    unsigned short usMin = 0, usMax = 0, usDef = 0;
    int iPos0 =  m_description.indexOf("(");
    int iPos1 =  m_description.indexOf(")");
    if (-1 < iPos0 && iPos0 < iPos1){
        QString strRange = m_description.mid(iPos0+1, iPos1 - iPos0 - 1).remove(' ');
        iPos0 = strRange.indexOf(QRegExp("\\d+-\\d+"));
        if (-1< iPos0){
            unsigned short* pAddr = pStAddr + parent()->dataidx();
            iPos1 = strRange.indexOf("-", iPos0);
            usMin = static_cast<unsigned short>(strRange.mid(iPos0, iPos1 - iPos0).toInt());
            usMax = static_cast<unsigned short>(strRange.mid(iPos1 + 1).toInt());
            usDef = static_cast<unsigned short>(m_defaultVal.toInt());
            qDebug()<<"name "<<m_name<<" min "<<usMin<<" max "<<usMax<<" addr "<<pAddr+m_dataidx;
            CKeyDnEdit* pEdit = dynamic_cast<CKeyDnEdit*>(m_pWidget);
            CBinder::BindEdit(pEdit, pAddr+m_dataidx, usMin, usMax, usDef);
        }
    }

    GroupCfgItem *pGroup = dynamic_cast<GroupCfgItem*>(m_parent);
    if (nullptr!= pGroup){
        UiCfgItem* pEnSource = nullptr;
        if (nullptr != (pEnSource = pGroup->findItemById(m_enableSourceId))){
            CKeyDnComboBox* pCmb = dynamic_cast<CKeyDnComboBox*>(pEnSource->getWidget());
            if(nullptr != pCmb){
                CEnableMngr::instance()->registerEableUi(pCmb, pCmb->valuePtr(), m_enableSourceVal, m_pWidget);
            }
        }
    }
    return true;
}

uint16_t *UiCfgItem::paramAddress()
{
    stack<GroupCfgItem*> parentStack;
    GroupCfgItem* parent = dynamic_cast<GroupCfgItem*>(this->parent());
    while(nullptr != parent){
        parentStack.push(parent);
        if (nullptr != parent->paramAddress()){
            break;
        }
        parent = dynamic_cast<GroupCfgItem*>(parent->parent());
    }
    if (nullptr == parent)
        return nullptr;

    uint16_t* paramAddr = parent->paramAddress();
    parentStack.pop();
    int dataCnt = 0;
    while(!parentStack.empty()){
        GroupCfgItem* pItem =parentStack.top();
        GroupCfgItem* pTmp = dynamic_cast<GroupCfgItem*>(parent->getHead());
        while(nullptr != pTmp){
            if (pTmp == pItem)
                break;
            dataCnt += pTmp->getDataCount();
            pTmp = dynamic_cast<GroupCfgItem*>(parent->getNext());
        }
        parent = pItem;
        parentStack.pop();
    }

    return paramAddr+dataCnt+m_dataidx;
}

UiCfgItem* UiCfgItem::createMyself()
{
    UiCfgItem* pItem = new UiCfgItem();
    pItem->m_id = m_id;
    pItem->m_name = m_name;
    pItem->m_left = m_left;
    pItem->m_top = m_top;
    pItem->m_width = m_width;
    pItem->m_height = m_height;
    pItem->m_dataidx = m_dataidx;
    pItem->m_type = m_type;
    pItem->m_description = m_description;
    pItem->m_defaultVal = m_defaultVal;
    pItem->m_enableSourceId = m_enableSourceId;
    pItem->m_enableSourceVal = m_enableSourceVal;

    return pItem;
}

//ComboboxItem
HNDZ_IMPLEMENT_DYNCREATE(UiCfgItemEx, UiCfgItem)
bool UiCfgItemEx::initFromDomElement(QDomElement element)
{
    UiCfgItem::initFromDomElement(element);
    setStrValue(m_range, element, "range");

    return true;
}
UiCfgItem* UiCfgItemEx::createMyself()
{
    UiCfgItemEx* pItem = new UiCfgItemEx();
    pItem->m_id = m_id;
    pItem->m_name = m_name;
    pItem->m_left = m_left;
    pItem->m_top = m_top;
    pItem->m_width = m_width;
    pItem->m_height = m_height;
    pItem->m_dataidx = m_dataidx;
    pItem->m_type = m_type;
    pItem->m_description = m_description;
    pItem->m_defaultVal = m_defaultVal;
    pItem->m_enableSourceId = m_enableSourceId;
    pItem->m_enableSourceVal = m_enableSourceVal;
    pItem->m_range = m_range;

    return pItem;
}
bool UiCfgItemEx::initData(unsigned short *pStAddr)
{
    UiCfgItem::initData(pStAddr);

    int pos = m_range.indexOf('-');
    if (-1 < pos){
        QString strMin = m_range.left(pos).trimmed();
        QString strMax = m_range.right(m_range.length() - pos - 1).trimmed();
        UiCfgItem *pMinItem = nullptr, *pMaxItem = nullptr;
        uint16_t   usMin = 0xFFFF, usMax = 0xFFFF, usDef;
        GroupCfgItem *pGroup = dynamic_cast<GroupCfgItem*>(m_parent);
        if (strMin[0] == '#' && nullptr != pGroup){
            pMinItem = pGroup->findItemById(strMin.right(strMin.length() - 1));
        }else {
            bool bOk = false;
            usMin = strMin.toUShort(&bOk);
            bOk = false;
        }
        if (strMax[0] == '#' && nullptr != pGroup){
            pMaxItem = pGroup->findItemById(strMax.right(strMax.length() - 1));
        }else {
            bool bOk = false;
            usMax = strMax.toUShort(&bOk);
            bOk = false;
        }
        usDef = m_defaultVal.toUShort();
        unsigned short* pAddr = pStAddr + parent()->dataidx();
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
    pItem->m_left = m_left;
    pItem->m_top = m_top;
    pItem->m_width = m_width;
    pItem->m_height = m_height;
    pItem->m_dataidx = m_dataidx;
    pItem->m_type = m_type;
    pItem->m_description = m_description;
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
bool ComboboxCfgItem::initData(unsigned short *pStAddr)
{
    UiCfgItem::initData(pStAddr);

    CKeyDnComboBox* pBox = dynamic_cast<CKeyDnComboBox*>(m_pWidget);
    if (nullptr != pBox){
        unsigned short* pAddr = pStAddr + parent()->dataidx();
        CBinder::BindComboBox(pBox, pAddr+m_dataidx, m_params, m_defaultVal);
        qDebug()<<"name "<<getName()<<" addr "<<pAddr+m_dataidx<<" value "<<*(pAddr+m_dataidx);
    }
}
