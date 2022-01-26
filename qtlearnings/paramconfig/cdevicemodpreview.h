#ifndef CDEVICEMODPREVIEW_H
#define CDEVICEMODPREVIEW_H

#include <QDialog>
#include <list>
using namespace std;

namespace Ui {
class CDeviceModPreview;
}

struct SDeviceInfoItem{
    QString name;
    QString oriVal;
    QString currVal;
    uint16_t *oriAddr;
    uint16_t *currAddr;
    int       dataCnt;
};

class CDeviceModPreview : public QDialog
{
    Q_OBJECT

public:
    explicit CDeviceModPreview(list<struct SDeviceInfoItem>* pItemList, QWidget *parent = nullptr);
    ~CDeviceModPreview();

protected:
    void initTable();
    void autoSetSize();
    QWidget* getCloseWidget(int x, int y);
    int getCloseCell(int x, int r);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();

private:
    Ui::CDeviceModPreview *ui;
    list<struct SDeviceInfoItem>* pItemList;
};

#endif // CDEVICEMODPREVIEW_H
