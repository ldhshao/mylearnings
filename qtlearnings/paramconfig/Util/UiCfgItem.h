#ifndef UIITEM_H
#define UIITEM_H
#include "ItemBase.h"

class QWidget;
class UiCfgItem : public XmlItem
{
public:
    HNDZ_DECLARE_DYNCREATE(UiCfgItem)
    UiCfgItem(UiCfgItem *parent = nullptr) : m_col(0), m_row(0), m_width(0), m_height(0),m_dataidx(-1),m_datacnt(1),
                  m_pWidget(nullptr), m_pWidName(nullptr), m_pWidDes(nullptr){
        m_enableSourceId = -1; m_enableSourceVal = 0;
        m_parent = parent; m_enableSource = nullptr;
    }
    virtual ~UiCfgItem() {}

    static QString strTypeEdit;
    static QString strTypeDevPointEdit;
    static QString strTypeSetIndexEdit;
    static QString strTypeCombobox;
    static QString strTypeComboboxSet;
    static QString strTypeCheckBox;
    static QString strTypeLabel;
    static QString strTypeGroup;
    static QString strTypePage;

    QString getNamePath(int ancestorDepth);
    QString getFullName();
    virtual QString getFullName(int idx);
    void setSize(int width, int height) { m_width =width; m_height = height; }
    virtual void setDataidx(int idx) { m_dataidx = idx; }
    void setParent(UiCfgItem* parent) { m_parent = parent; }
    int  col() {	return m_col;	}
    int  row() {	return m_row;	}
    int  width() {	return m_width;	}
    int  height() {	return m_height;	}
    int  dataidx() { return m_dataidx; }
    int  datacount() { return m_datacnt; }
    QString  enableSourceId() {	return m_enableSourceId; }
    uint16_t  enableSourceVal() { return m_enableSourceVal; }
    UiCfgItem* enableSource() { return m_enableSource; }
    QString    enableReason();
    UiCfgItem* parent() { return m_parent; }
    bool isType(const QString &type) {	return m_type == type; }
    QWidget* getWidget() {	return m_pWidget;	}
    QWidget* getWidName() {	return m_pWidName;	}
    QWidget* getWidDes() {	return m_pWidDes;	}

    virtual bool initFromDomElement(QDomElement element);
    virtual void dump();

    virtual UiCfgItem* createMyself();
    virtual void copyTo(UiCfgItem* destItem);

    virtual void create(QWidget* parent);
    virtual bool initData(int idx, bool useDef);
    virtual bool initUi(unsigned short* pStAddr);//init data

    virtual uint16_t *paramAddress();

    virtual QString previewInfo();
    virtual QString strDataValue(uint16_t* pAddr = nullptr);
    virtual QString getDataValue(uint16_t *pVal, int *dataCnt);//return data value in string; and data count, u16
    virtual void setDefaultVal();

    virtual bool onMaxValChanged(uint32_t max);

    virtual bool isDataOK();

protected:
    int m_col, m_row;
    int m_width, m_height;
    int m_dataidx, m_datacnt;
    QString m_type;
    QString m_range;
    QString m_defaultVal;
    QString m_enableSourceId;//ID path; example: 1)5 2)../5/7
    uint16_t m_enableSourceVal;
    UiCfgItem  *m_parent;
    UiCfgItem  *m_enableSource;
    QWidget *m_pWidget;
    QWidget *m_pWidName;
    QWidget *m_pWidDes;//description
};
class UiCfgItemFloat : public UiCfgItem
{
public:
    HNDZ_DECLARE_DYNCREATE(UiCfgItemFloat)
    UiCfgItemFloat() :m_precision(1), m_decimalPlaces(0){ 	}
    virtual ~UiCfgItemFloat() {}

    virtual bool initFromDomElement(QDomElement element);
    virtual UiCfgItem* createMyself();

    virtual void create(QWidget* parent);
    //virtual bool initUi(unsigned short* pStAddr);//init data

    virtual QString strDataValue(uint16_t* pAddr = nullptr);
protected:
    QString m_unit;
    float   m_precision;
    uint16_t m_decimalPlaces;
};
class SetIndexCfgItem : public UiCfgItem
{
public:
    HNDZ_DECLARE_DYNCREATE(SetIndexCfgItem)
    SetIndexCfgItem() :m_setSize(0), m_setCnt(0){ 	}
    virtual ~SetIndexCfgItem() {}

    virtual bool initFromDomElement(QDomElement element);
    virtual UiCfgItem* createMyself();
    virtual bool initUi(unsigned short* pStAddr);//init data

    virtual QString previewInfo();
    virtual void setDefaultVal();
    virtual QString getFullName(int idx);
    virtual QString getDataValue(uint16_t *pVal, int *dataCnt);//return data value in string; and data count, u16
protected:
    uint16_t m_setSize;
    uint16_t m_setCnt;
    QString  m_previewCfg;
    QString  m_dataNameCfg;
};
class ComboboxCfgItem : public UiCfgItem
{
public:
    HNDZ_DECLARE_DYNCREATE(ComboboxCfgItem)
    ComboboxCfgItem() { m_type = UiCfgItem::strTypeCombobox;	}
    virtual ~ComboboxCfgItem() {}

    virtual bool initFromDomElement(QDomElement element);
    virtual UiCfgItem* createMyself();
    virtual void copyTo(UiCfgItem* destItem);

    //virtual QWidget* create();
    //virtual bool init();
    virtual bool initUi(unsigned short* pStAddr);//init data
    QStringList params() { return m_params; }

protected:
    QStringList m_params;
};
class ComboboxSetCfgItem : public ComboboxCfgItem
{
public:
    HNDZ_DECLARE_DYNCREATE(ComboboxSetCfgItem)
    ComboboxSetCfgItem() { m_type = UiCfgItem::strTypeComboboxSet; pSetIndexSource = nullptr;	}
    virtual ~ComboboxSetCfgItem() {}

    virtual bool initFromDomElement(QDomElement element);
    virtual UiCfgItem* createMyself();

    virtual void create(QWidget* parent);
    virtual bool initUi(unsigned short* pStAddr);//init data

    virtual QString previewInfo();
    virtual void setDefaultVal();
    virtual QString getFullName(int idx);
    virtual QString getDataValue(uint16_t *pVal, int *dataCnt);//return data value in string; and data count, u16

    virtual bool onMaxValChanged(uint32_t max);
protected:
    QString    m_setIndexSource;
    QString    m_previewCfg;
    QString    m_setSize;
    UiCfgItem *pSetIndexSource;
};
#endif

