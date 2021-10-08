#include "UiItem.h"
#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QDebug>

QString UiItem::strTypeEdit = "editbox";
QString UiItem::strTypeCombobox = "combobox";
QString UiItem::strTypeCheckBox = "checkbox";
QString UiItem::strTypeLabel    = "label";
QString UiItem::strTypeGroup    = "groupbox";

HNDZ_IMPLEMENT_DYNCREATE(UiItem, XmlItem)
bool UiItem::initFromDomElement(QDomElement element)
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
QWidget* UiItem::create()
{
    QWidget *ctl = nullptr;
    if (m_type == "checkbox"){
        QCheckBox* pBox = new QCheckBox();
        pBox->setText(getName());
        ctl = pBox;
    }else if (m_type == "editbox"){
        ctl = new QLineEdit();
    }else if (m_type == "combobox"){
        ctl = new QComboBox();
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
bool UiItem::init(QWidget* w)
{
    if (nullptr != w){
        w->resize(m_width, m_height);
        w->move(m_left, m_top);
        w->show();
    }

    return true;
}

//ComboboxItem
HNDZ_IMPLEMENT_DYNCREATE(ComboboxItem, UiItem)
bool ComboboxItem::initFromDomElement(QDomElement element)
{
    UiItem::initFromDomElement(element);
    if (m_type.isEmpty())
        m_type = UiItem::strTypeCombobox;
    QString strParams;
    setStrValue(strParams, element, "params");
    qDebug()<<strParams;
    //check value
    m_params = strParams.split(";");
    qDebug()<<m_params;

    return true;
}
bool ComboboxItem::init(QWidget* w)
{
    if (UiItem::init(w) && !m_params.isEmpty()){
        QComboBox *pBox = dynamic_cast<QComboBox*>(w);
        if (nullptr != pBox){
            pBox->insertItems(0, m_params);
        }
    }

    return true;
}
