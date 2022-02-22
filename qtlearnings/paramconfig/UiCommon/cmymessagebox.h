#ifndef CMYMESSAGEBOX_H
#define CMYMESSAGEBOX_H

#include <QMessageBox>

class CMyMessageBox : public QMessageBox
{
    Q_OBJECT
public:
    explicit CMyMessageBox(QWidget *parent = 0);
    CMyMessageBox(Icon icon, const QString &title, const QString &text,
                StandardButtons buttons = NoButton, QWidget *parent = 0,
                Qt::WindowFlags flags = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

protected:
    virtual void keyPressEvent(QKeyEvent *event);
};

#endif // CMYMESSAGEBOX_H
