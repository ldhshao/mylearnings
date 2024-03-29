#include "UiItem.h"
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
    //check value

    return true;
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
            //ctl = new QLineEdit();
            //m_pWidget = new CStateLineEdit(parent);
            m_pWidget = new CKeyDnEdit(parent);
        }else if (m_type == UiCfgItem::strTypeCombobox){
            //ctl = new QComboBox();
            m_pWidget = new CKeyDnComboBox(parent);
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
    unsigned short usMin = 0, usMax = 0;
    int iPos0 =  m_description.indexOf("(");
    int iPos1 =  m_description.indexOf(")");
    if (-1 < iPos0 && iPos0 < iPos1){
        QString strRange = m_description.mid(iPos0+1, iPos1 - iPos0 - 1).remove(' ');
        iPos0 = strRange.indexOf(QRegExp("\\d+-\\d+"));
        if (-1< iPos0){
            iPos1 = strRange.indexOf("-", iPos0);
            usMin = static_cast<unsigned short>(strRange.mid(iPos0, iPos1 - iPos0).toInt());
            usMax = static_cast<unsigned short>(strRange.mid(iPos1 + 1).toInt());
            qDebug()<<"name "<<m_name<<" min "<<usMin<<" max "<<usMax;
            CKeyDnEdit* pEdit = dynamic_cast<CKeyDnEdit*>(m_pWidget);
            CBinder::BindEdit(pEdit, pStAddr+m_dataidx, usMin, usMax);
            if (!m_defaultVal.isEmpty())
                pEdit->setEditText(m_defaultVal);
        }
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
    CKeyDnComboBox* pBox = dynamic_cast<CKeyDnComboBox*>(m_pWidget);
    if (nullptr != pBox){
        CBinder::BindComboBox(pBox, pStAddr+m_dataidx, m_params, m_defaultVal);
    }
}
