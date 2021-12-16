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

signals:
    void sig_valueChanged(uint16_t *pVal, uint32_t valNew);

protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void focusInEvent(QFocusEvent *event);
    virtual void focusOutEvent(QFocusEvent *event);
    uint16_t *pVal;
};

#endif // CDEVPOINTEDIT_H
