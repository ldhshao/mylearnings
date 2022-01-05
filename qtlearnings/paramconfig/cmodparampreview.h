#ifndef CMODPARAMPREVIEW_H
#define CMODPARAMPREVIEW_H

#include <QWidget>
#include <list>
#include <vector>
using namespace std;

class UiCfgItem;
class QPushButton;
class QLabel;

namespace Ui {
class CModParamPreview;
}

struct SModParamInfoItem{
    UiCfgItem *item;
    int        idx;
    int       dataCnt;
};
class CModParamPreview : public QWidget
{
    Q_OBJECT

public:
    explicit CModParamPreview(list<SModParamInfoItem>* iLst, QWidget *parent = nullptr);
    ~CModParamPreview();

    void updateItemList(list<SModParamInfoItem>* iList);
    void setParamAddr(uint16_t* srvAddr, uint16_t* lclAddr) { paramSrvAddr = srvAddr; paramLclAddr = lclAddr; }

protected slots:
    void slot_operateParam();

protected:
    QWidget* getCloseWidget(int x, int y);
    int getCloseCell(int x, int r);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void changeEvent(QEvent *event);

private slots:
    void on_pushButton_upload_clicked();

private:
    void initTable();
    void autoAdjustTableColumns();
    void autoSetFocus();
    void dealUnuploadData();
    void saveModifiedParams();

    Ui::CModParamPreview *ui;
    QLabel             *bkLbl;
    list<SModParamInfoItem>     *itemList;
    vector<QPushButton*>  btnList;
    uint16_t           *paramSrvAddr;
    uint16_t           *paramLclAddr;
};

#endif // CMODPARAMPREVIEW_H
