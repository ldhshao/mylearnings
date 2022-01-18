#ifndef CDEVPOINTEDIT_H
#define CDEVPOINTEDIT_H
#include"UiCommon/uistatectl.h"
#include <QLineEdit>

class CDevPointEdit : public QLineEdit, public CMyCtl
{
    Q_OBJECT

public:
    explicit CDevPointEdit(QWidget *parent = nullptr);
    explicit CDevPointEdit(const QString &text, QWidget *parent = nullptr);

    virtual void updateText();
    void setValuePtr(uint16_t* val);
    void setValue(uint32_t val);
    bool setEditText(const QString& strText);
    uint32_t getValue();
    void showText();
    void setPortType(int type) { portType = type; }
    uint8_t getPortType() { return portType; }
    void endEdit();
    QString tipInfo();
    //set item init
    void setItemIndex(uint8_t idx) { m_itemIdx = idx; }

    enum {
      DPES_IDLE = 0,
      DPES_EDITING,
      DPES_CONFIRM
    };

signals:
    void sig_valueChanged(uint16_t *pVal, uint32_t valNew);

protected slots:
    void slot_dataSetChanged(uint16_t* pAddr, uint16_t setSize);

protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void focusInEvent(QFocusEvent *event);
    virtual void focusOutEvent(QFocusEvent *event);
    uint16_t *pVal;
    uint8_t   portType;
    uint8_t   state;
    uint8_t   step;
    uint8_t m_itemIdx;//for set item
};

#endif // CDEVPOINTEDIT_H
