#ifndef MYEDIT_H
#define MYEDIT_H

#include <QLineEdit>

class MyEdit : public QLineEdit
{
    Q_OBJECT

public:
    MyEdit(QWidget *parent = nullptr);

protected:
    virtual void keyPressEvent(QKeyEvent *event);
};

#endif // MYEDIT_H
