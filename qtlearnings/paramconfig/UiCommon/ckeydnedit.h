#ifndef CKEYDNEDIT_H
#define CKEYDNEDIT_H

#include "uistatectl.h"
#include "ibindobj.h"


class CKeyDnEdit : public CStateLineEdit, public CMyCtl
{
    Q_OBJECT

public:
    CKeyDnEdit(QWidget *parent = 0);
    ~CKeyDnEdit(){
        if(pBind != NULL)
            delete pBind;
    }

    virtual void updateText() {	showText();	}
    void showText();
    bool setEditText(const QString& strText);
    void setAccessibleName(QString str)
    {
        m_accessiblename = str;
    }
    QString accessibleName()
    {
        return m_accessiblename;
    }

    IBindObj* pBind;
    // tips框偏移量，默认为0，界面使用容器时需要重新计算
    void SetTipOffset(int x, int y) { XOffset = x; YOffset = y; }
    int XOffset;
    int YOffset;
protected:
    void keyPressEvent(QKeyEvent* ev);
    virtual void focusInEvent(QFocusEvent *ev);
    virtual void focusOutEvent(QFocusEvent *ev);
 signals:
    void keydown(QKeyEvent* ev);
    void sig_valueChanged(uint16_t *pVal, uint32_t valNew);
protected slots:
    //void slot_textEdited(const QString& newTxt);
    //void slot_textChanged(const QString& newTxt);

private:
    QString m_accessiblename;
    bool    m_editing;
};

#endif // CKEYDNEDIT_H
