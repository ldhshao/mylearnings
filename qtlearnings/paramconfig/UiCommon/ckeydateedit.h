#ifndef CKEYDATEEDIT_H
#define CKEYDATEEDIT_H

#include <QDateEdit>

class CKeyDateEdit : public QDateEdit
{
    Q_OBJECT
public:
    explicit CKeyDateEdit(QWidget *parent = nullptr);

protected:
    virtual void keyPressEvent(QKeyEvent *event);
};

#endif // CKEYDATEEDIT_H
