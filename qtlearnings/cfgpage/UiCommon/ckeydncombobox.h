#ifndef CKEYDNCOMBOBOX_H
#define CKEYDNCOMBOBOX_H

#include "uistatectl.h"


class CKeyDnComboBox : public CStateComboBox
{
    Q_OBJECT

public:
    CKeyDnComboBox(QWidget *parent = 0);
    virtual ~CKeyDnComboBox(){
    }

    bool setComboBoxText(const QString& strText);
    void setValuePtr(u_int16_t* val) { pVal = val; }

protected:
    void keyPressEvent(QKeyEvent* ev);
    virtual void focusInEvent(QFocusEvent *event);

private:
    uint16_t *pVal;
};

#endif // CKEYDNEDIT_H
