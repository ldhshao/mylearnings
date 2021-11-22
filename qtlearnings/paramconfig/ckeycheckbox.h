#ifndef CKEYCHECKBOX_H
#define CKEYCHECKBOX_H

#include <QCheckBox>

class CKeyCheckBox : public QCheckBox
{
public:
    explicit CKeyCheckBox(QWidget *parent = nullptr);
    explicit CKeyCheckBox(const QString &text, QWidget *parent = nullptr);

protected:
    virtual void keyPressEvent(QKeyEvent *event);
};

#endif // CKEYCHECKBOX_H
