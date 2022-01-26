#ifndef CKEYLABEL_H
#define CKEYLABEL_H

#include <QLabel>

class CKeyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit CKeyLabel(QWidget *parent=nullptr, Qt::WindowFlags f=Qt::WindowFlags());

protected:
    virtual void mousePressEvent(QMouseEvent * ev);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void focusInEvent(QFocusEvent *event);
    virtual void focusOutEvent(QFocusEvent *event);

    signals:
    void clicked(QLabel* lbl);
};

#endif // CKEYLABEL_H
