#include "UiItem.h"
#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QDebug>
#include "UiCommon/uistatectl.h"

QString UiCfgItem::strTypeEdit = "editbox";
QString UiCfgItem::strTypeCombobox = "combobox";
QString UiCfgItem::strTypeCheckBox = "checkbox";
QString UiCfgItem::strTypeLabel    = "label";
QString UiCfgItem::strTypeGroup    = "groupbox";

HNDZ_IMPLEMENT_DYNCREATE(UiCfgItem, XmlItem)
bool UiCfgItem::initFromDomElement(QDomElement element)
{
    XmlItem::initFromDomElement(element);

    setIntValue(m_left, element, "left");
    setIntValue(m_top, element, "top");
    setIntValue(m_width, element, "width");
    setIntValue(m_height, element, "height");
    //check value

    setStrValue(m_type, element, "type");
    setStrValue(m_description, element, "description");
    //check value

    return true;
}
void UiCfgItem::create(QWidget* parent)
{
    if (nullptr == m_pWidget){
        bool bGroup = false;
        if (m_type == "checkbox"){
            //QCheckBox* pBox = new QCheckBox();
            CStateCheckBox *pBox = new CStateCheckBox(parent);
            pBox->setText(getName());
            m_pWidget = pBox;
        }else if (m_type == UiCfgItem::strTypeEdit){
            //ctl = new QLineEdit();
            m_pWidget = new CStateLineEdit(parent);
        }else if (m_type == "combobox"){
            //ctl = new QComboBox();
            m_pWidget = new CStateComboBox(parent);
        }else if (m_type == "groupbox"){
            QGroupBox* pBox = new QGroupBox(parent);
            pBox->setTitle(getName());
            m_pWidget = pBox;
            bGroup = true;
        }
        if (!bGroup){
            QLabel* pBoxName = new QLabel(parent);
            pBoxName->setAlignment(Qt::AlignLeft);
            pBoxName->setText(getName());
            m_pWidName = pBoxName;
            QLabel* pBoxDes = new QLabel(parent);
            pBoxDes->setAlignment(Qt::AlignLeft);
            pBoxDes->setText(m_description);
            m_pWidDes = pBoxDes;
        }
    }
}
bool UiCfgItem::init()
{
    if (nullptr != m_pWidget){
        m_pWidget->resize(m_width, m_height);
        m_pWidget->move(m_left, m_top);
        m_pWidget->show();
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
bool ComboboxCfgItem::init()
{
    if (UiCfgItem::init() && !m_params.isEmpty()){
        QComboBox *pBox = dynamic_cast<QComboBox*>(m_pWidget);
        if (nullptr != pBox){
            pBox->insertItems(0, m_params);
        }
    }

    return true;
}
