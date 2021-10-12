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
    //check value

    return true;
}
QWidget* UiCfgItem::create()
{
    QWidget *ctl = nullptr;
    if (m_type == "checkbox"){
        //QCheckBox* pBox = new QCheckBox();
        CStateCheckBox *pBox = new CStateCheckBox();
        pBox->setText(getName());
        ctl = pBox;
    }else if (m_type == UiCfgItem::strTypeEdit){
        //ctl = new QLineEdit();
        ctl = new CStateLineEdit();
    }else if (m_type == "combobox"){
        //ctl = new QComboBox();
        ctl = new CStateComboBox();
    }else if (m_type == "groupbox"){
        QGroupBox* pBox = new QGroupBox();
        pBox->setTitle(getName());
        ctl = pBox;
    }else if (m_type == "label"){
        QLabel* pBox = new QLabel();
        pBox->setAlignment(Qt::AlignLeft);
        pBox->setText(getName());
        ctl = pBox;
    }

    return ctl;
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
