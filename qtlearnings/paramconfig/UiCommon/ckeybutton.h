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
};

#endif // CKEYBUTTON_H
