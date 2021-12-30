#ifndef CKEYDNCOMBOBOX_H
#define CKEYDNCOMBOBOX_H

#include "uistatectl.h"
#include <QLabel>

class CKeyDnComboBox : public CStateComboBox, public CMyCtl
{
    Q_OBJECT

public:
    CKeyDnComboBox(QWidget *parent = 0);
    virtual ~CKeyDnComboBox(){
    }

    virtual void updateText();

    bool setEditText(const QString& strText);
    uint16_t* valuePtr() { return pVal; }
    void setValuePtr(uint16_t* val) { pVal = val; }

signals:
    void sig_valueChanged(uint16_t *pVal, uint32_t valNew);

protected:
    void keyPressEvent(QKeyEvent* ev);
    virtual void focusInEvent(QFocusEvent *event);
    virtual void focusOutEvent(QFocusEvent *event);

protected slots:
    void slot_currentIndexChanged(int index);

protected:
    uint16_t *pVal;
    bool      updating;
};

class CKeyDnComboBoxSet : public CKeyDnComboBox
{
    Q_OBJECT

public:
    explicit CKeyDnComboBoxSet(QWidget *parent = nullptr);
    ~CKeyDnComboBoxSet();

    void initData(uint16_t* pAddr, uint16_t cnt);
    void initUi();
    uint16_t dataCount() { return dataCnt; }

    void onDataCountChanged(uint16_t dataCount);

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void focusInEvent(QFocusEvent *event);

    void setDataIndex(int idx);
    void onPrevData();
    void onNextData();

    signals:
    void sig_dataIndexChanged(int dataIdx);

private slots:
    void slot_dataSetChanged(uint16_t* pAddr, uint16_t setSize);
    void slot_currentIndexChanged(int index);

private:
    uint16_t       dataCnt;
    uint16_t       dataIdx;
};

class CIndexLabel : public QLabel
{
    Q_OBJECT

public:
    explicit CIndexLabel(QWidget *parent = nullptr);
    ~CIndexLabel(){}

    void setDataName(QString name) { strDataName = name; }

protected slots:
    void slot_dataIndexChanged(int dataIdx);

protected:
    QString strDataName;
};
#endif // CKEYDNEDIT_H
