#ifndef UIITEM_H
#define UIITEM_H
#include "ItemBase.h"

class QWidget;
class UiCfgItem : public XmlItem
{
public:
    HNDZ_DECLARE_DYNCREATE(UiCfgItem)
    UiCfgItem(UiCfgItem *parent = nullptr) : m_left(0), m_top(0), m_width(0), m_height(0),
                  m_pWidget(nullptr), m_pWidName(nullptr), m_pWidDes(nullptr){
        m_pParent = parent;
    }
    virtual ~UiCfgItem() {}

    static QString strTypeEdit;
    static QString strTypeCombobox;
    static QString strTypeCheckBox;
    static QString strTypeLabel;
    static QString strTypeGroup;

    void setPos(int left, int top) { m_left = left; m_top = top;}
    void setSize(int width, int height) { m_width =width; m_height = height; }
    int  left() {	return m_left;	}
    int  top() {	return m_top;	}
    int  width() {	return m_width;	}
    int  height() {	return m_height;	}
    bool isType(const QString &type) {	return m_type == type; }
    QWidget* getWidget() {	return m_pWidget;	}
    QWidget* getWidName() {	return m_pWidName;	}
    QWidget* getWidDes() {	return m_pWidDes;	}

    virtual bool initFromDomElement(QDomElement element);

    virtual void create(QWidget* parent);
    virtual bool init();

protected:
    int m_left, m_top, m_width, m_height;
    QString m_type;
    QString m_description;
    UiCfgItem  *m_pParent;//delete later
    QWidget *m_pWidget;
    QWidget *m_pWidName;
    QWidget *m_pWidDes;//description
};
class ComboboxCfgItem : public UiCfgItem
{
public:
    HNDZ_DECLARE_DYNCREATE(ComboboxCfgItem)
    ComboboxCfgItem() { m_type = UiCfgItem::strTypeCombobox;	}
    virtual ~ComboboxCfgItem() {}

    virtual bool initFromDomElement(QDomElement element);

    //virtual QWidget* create();
    virtual bool init();

protected:
    QStringList m_params;
};
#endif

