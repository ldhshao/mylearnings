#ifndef UIITEM_H
#define UIITEM_H
#include "ItemBase.h"

class QWidget;
class UiItem : public XmlItem
{
public:
    HNDZ_DECLARE_DYNCREATE(UiItem)
    UiItem() : m_left(0), m_top(0), m_width(0), m_height(0) {}
    virtual ~UiItem() {}

    static QString strTypeEdit;
    static QString strTypeCombobox;
    static QString strTypeCheckBox;
    static QString strTypeLabel;
    static QString strTypeGroup;

    void setPos(int left, int top) { m_left = left; m_top = top;}
    void setSize(int width, int height) { m_width =width; m_height = height; }
    bool isType(const QString &type) {	return m_type == type; }

    virtual bool initFromDomElement(QDomElement element);

    virtual QWidget* create();
    virtual bool init(QWidget* w);

protected:
    int m_left, m_top, m_width, m_height;
    QString m_type;
};
class ComboboxItem : public UiItem
{
public:
    HNDZ_DECLARE_DYNCREATE(ComboboxItem)
    ComboboxItem() { m_type = UiItem::strTypeCombobox;	}
    virtual ~ComboboxItem() {}

    virtual bool initFromDomElement(QDomElement element);

    //virtual QWidget* create();
    virtual bool init(QWidget* w);

protected:
    QStringList m_params;
};
#endif

