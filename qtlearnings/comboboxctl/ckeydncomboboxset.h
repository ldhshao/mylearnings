#ifndef CKEYDNCOMBOBOXSET_H
#define CKEYDNCOMBOBOXSET_H

#include <QWidget>

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

#endif // CKEYDNCOMBOBOXSET_H
