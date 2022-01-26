#ifndef CKEYDNCOMBOBOXSET_H
#define CKEYDNCOMBOBOXSET_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>

namespace Ui {
class CKeyDnComboBoxSet;
}

class CKeyDnComboBoxSet : public QWidget
{
    Q_OBJECT

public:
    explicit CKeyDnComboBoxSet(QWidget *parent = nullptr);
    ~CKeyDnComboBoxSet();

    void initComboBox(const QStringList &params);
    void initData(uint16_t* pAddr, int cnt);
    void dump();

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void focusInEvent(QFocusEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

    void setDataIndex(int idx);

    signals:
    void sig_dataIndexChanged(int dataIdx);

private slots:
    void on_pushButton_prev_clicked();
    
    void on_pushButton_next_clicked();
    
private:
    Ui::CKeyDnComboBoxSet *ui;
    uint16_t *pData;
    int       dataCnt;
    int       dataIdx;
};

class CKeyDnComboBoxEx : public QComboBox
{
    Q_OBJECT

public:
    explicit CKeyDnComboBoxEx(QWidget *parent = nullptr);
    ~CKeyDnComboBoxEx();

    void initComboBox(const QStringList &params);
    void initData(uint16_t* pAddr, int cnt);
    void dump();

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void focusInEvent(QFocusEvent *event);

    void setDataIndex(int idx);

    signals:
    void sig_dataIndexChanged(int dataIdx);

private slots:
    void slot_dataSetChanged(uint16_t* pAddr, int setSize);
    void slot_currentIndexChanged(int index);

    void on_pushButton_prev_clicked();

    void on_pushButton_next_clicked();

private:
    Ui::CKeyDnComboBoxSet *ui;
    uint16_t *pData;
    int       dataCnt;
    int       dataIdx;
};

class CKeyDnSetIndexEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit CKeyDnSetIndexEdit(QWidget *parent = nullptr);
    ~CKeyDnSetIndexEdit();

    void initData(uint16_t* pAddr, int cnt, int setSize) {
        pData = pAddr;
        dataCnt = cnt;
        this->setSize = setSize;
    }

protected:
    signals:
    void sig_dataSetChanged(uint16_t* pAddr, int setSize);

protected:
    virtual void focusOutEvent(QFocusEvent *event);

    uint16_t *pData;
    int       dataCnt;
    int       setSize;
};
#endif // CKEYDNCOMBOBOXSET_H
