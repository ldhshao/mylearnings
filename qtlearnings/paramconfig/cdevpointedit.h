#ifndef CDEVPOINTEDIT_H
#define CDEVPOINTEDIT_H

#include <QLineEdit>

class CDevPointEdit : public QLineEdit
{
public:
    explicit CDevPointEdit(QWidget *parent = nullptr);
    explicit CDevPointEdit(const QString &text, QWidget *parent = nullptr);

    void setValuePtr(uint16_t* val);
    void setValue(uint32_t val);
    void showText();

protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    uint16_t *pVal;
};

#endif // CDEVPOINTEDIT_H
