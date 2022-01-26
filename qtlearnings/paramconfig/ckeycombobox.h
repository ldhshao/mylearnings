#ifndef CKEYCOMBOBOX_H
#define CKEYCOMBOBOX_H

#include <QComboBox>

class CKeyComboBox : public QComboBox
{
public:
    explicit CKeyComboBox(QWidget *parent = nullptr);

protected:
    virtual void keyPressEvent(QKeyEvent *event);
};

#endif // CKEYCOMBOBOX_H
