#ifndef CKEYLABEL_H
#define CKEYLABEL_H

#include <QLabel>

class CKeyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit CKeyLabel(QWidget *parent=nullptr, Qt::WindowFlags f=Qt::WindowFlags());

protected:
    virtual void mouseReleaseEvent(QMouseEvent * ev);

    signals:
    void clicked(QLabel* lbl);
};

#endif // CKEYLABEL_H
