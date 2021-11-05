#ifndef CKEYDNEDIT_H
#define CKEYDNEDIT_H

#include "uistatectl.h"
#include "ibindobj.h"


class CKeyDnEdit : public CStateLineEdit
{
    Q_OBJECT

public:
    CKeyDnEdit(QWidget *parent = 0);
    ~CKeyDnEdit(){
        if(pBind != NULL)
            delete pBind;
    }

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
    /* 需要显示在编辑框 */
    //void needDisplay(QKeyEvent *ev);
protected:
    void keyPressEvent(QKeyEvent* ev);
    //virtual void focusInEvent(QFocusEvent* ev);
    //bool event(QEvent *ev);
 signals:
    void keydown(QKeyEvent* ev);
protected slots:
    void slot_textEdited(const QString& newTxt);
    void slot_textChanged(const QString& newTxt);

private:
    QString m_accessiblename;
};

#endif // CKEYDNEDIT_H
