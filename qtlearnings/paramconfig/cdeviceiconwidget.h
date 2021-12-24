#ifndef CDEVICEICONWIDGET_H
#define CDEVICEICONWIDGET_H

#include <QWidget>

namespace Ui {
class CDeviceIconWidget;
}

class CDeviceIconWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CDeviceIconWidget(QWidget *parent = nullptr, uint8_t m = 1);
    ~CDeviceIconWidget();

    void setTitle(QString strTitle);
    void setImagePath(QString strImgPath);

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void focusInEvent(QFocusEvent *ev);
    virtual void focusOutEvent(QFocusEvent *ev);

    signals:
    void clicked(QWidget* w);

private:
    Ui::CDeviceIconWidget *ui;
    uint8_t                mode;//0 little; 1 normal;
};

#endif // CDEVICEICONWIDGET_H
