#ifndef UIITEM_H
#define UIITEM_H
#include "ItemBase.h"

class QWidget;
class UiCfgItem : public XmlItem
{
public:
    HNDZ_DECLARE_DYNCREATE(UiCfgItem)
    UiCfgItem(UiCfgItem *parent = nullptr) : m_left(0), m_top(0), m_width(0), m_height(0),m_dataidx(-1),
                  m_pWidget(nullptr), m_pWidName(nullptr), m_pWidDes(nullptr){
        m_enableSourceId = -1; m_enableSourceVal = 0;
        m_parent = parent;
    }
    virtual ~UiCfgItem() {}

    static QString strTypeEdit;
    static QString strTypeCombobox;
    static QString strTypeCheckBox;
    static QString strTypeLabel;
    static QString strTypeGroup;
    static QString strTypePage;

    void setPos(int left, int top) { m_left = left; m_top = top;}
    void setSize(int width, int height) { m_width =width; m_height = height; }
    virtual void setDataidx(int idx) { m_dataidx = idx; }
    void setParent(UiCfgItem* parent) { m_parent = parent; }
    int  left() {	return m_left;	}
    int  top() {	return m_top;	}
    int  width() {	return m_width;	}
    int  height() {	return m_height;	}
    int  dataidx() { return m_dataidx; }
    QString  enableSourceId() {	return m_enableSourceId; }
    uint16_t  enableSourceVal() { return m_enableSourceVal; }
    UiCfgItem* parent() { return m_parent; }
    bool isType(const QString &type) {	return m_type == type; }
    QWidget* getWidget() {	return m_pWidget;	}
    QWidget* getWidName() {	return m_pWidName;	}
    QWidget* getWidDes() {	return m_pWidDes;	}

    virtual bool initFromDomElement(QDomElement element);
    virtual void dump();

    virtual UiCfgItem* createMyself();

    virtual void create(QWidget* parent);
    virtual bool init();
    virtual bool initData(unsigned short* pStAddr);//init data

    virtual uint16_t *paramAddress();

protected:
    int m_left, m_top, m_width, m_height;
    int m_dataidx;
    QString m_type;
    QString m_description;
    QString m_defaultVal;
    QString m_enableSourceId;//ID path; example: 1)5 2)../5/7
    uint16_t m_enableSourceVal;
    UiCfgItem  *m_parent;
    QWidget *m_pWidget;
    QWidget *m_pWidName;
    QWidget *m_pWidDes;//description
};
class UiCfgItemEx : public UiCfgItem
{
public:
    HNDZ_DECLARE_DYNCREATE(UiCfgItemEx)
    UiCfgItemEx() { 	}
    virtual ~UiCfgItemEx() {}

    virtual bool initFromDomElement(QDomElement element);
    virtual UiCfgItem* createMyself();

    virtual bool initData(unsigned short* pStAddr);//init data

protected:
    QString m_range;
};
class ComboboxCfgItem : public UiCfgItem
{
public:
    HNDZ_DECLARE_DYNCREATE(ComboboxCfgItem)
    ComboboxCfgItem() { m_type = UiCfgItem::strTypeCombobox;	}
    virtual ~ComboboxCfgItem() {}

    virtual bool initFromDomElement(QDomElement element);
    virtual UiCfgItem* createMyself();

    //virtual QWidget* create();
    //virtual bool init();
    virtual bool initData(unsigned short* pStAddr);//init data

protected:
    QStringList m_params;
};
#endif

