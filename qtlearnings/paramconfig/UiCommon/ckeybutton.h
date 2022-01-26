#ifndef CKEYBUTTON_H
#define CKEYBUTTON_H

#include <QPushButton>

class CKeyButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CKeyButton(QWidget *parent = nullptr);

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void focusInEvent(QFocusEvent *ev);
    virtual void focusOutEvent(QFocusEvent *ev);
};

class CKeyButtonEx : public CKeyButton
{
    Q_OBJECT
public:
    explicit CKeyButtonEx(QWidget *parent = nullptr);

protected:
    virtual void focusInEvent(QFocusEvent *ev);
    virtual void focusOutEvent(QFocusEvent *ev);

    QString removeStyle(const QString& strStyle, QString strAttr);

    QString m_preStyle;
};
#endif // CKEYBUTTON_H
