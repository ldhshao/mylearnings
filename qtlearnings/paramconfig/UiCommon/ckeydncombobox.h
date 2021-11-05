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

    //bool setComboBoxText(const QString& strText);
    uint16_t* valuePtr() { return pVal; }
    void setValuePtr(uint16_t* val) { pVal = val; }

signals:
    void sig_valueChanged(uint16_t *pVal, uint16_t valNew);

protected:
    void keyPressEvent(QKeyEvent* ev);
    //virtual void focusInEvent(QFocusEvent *event);

protected slots:
    void slot_currentIndexChanged(int index);

private:
    uint16_t *pVal;
};

#endif // CKEYDNEDIT_H
