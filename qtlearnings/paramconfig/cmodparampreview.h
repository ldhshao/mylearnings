#ifndef CMODPARAMPREVIEW_H
#define CMODPARAMPREVIEW_H

#include <QWidget>
#include <list>
#include <vector>
using namespace std;

class UiCfgItem;
class QPushButton;

namespace Ui {
class CModParamPreview;
}

class CModParamPreview : public QWidget
{
    Q_OBJECT

public:
    explicit CModParamPreview(list<UiCfgItem*>* iLst, QWidget *parent = nullptr);
    ~CModParamPreview();

    void updateItemList(list<UiCfgItem*>* iList);

protected slots:
    void slot_operateParam();

protected:
    QWidget* getCloseWidget(int x, int y);
    int getCloseCell(int x, int r);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void changeEvent(QEvent *event);

private slots:
    void on_pushButton_send_clicked();

private:
    void initTable();
    void autoAdjustTableColumns();

    Ui::CModParamPreview *ui;
    list<UiCfgItem*>     *itemList;
    vector<QPushButton*>  btnList;
};

#endif // CMODPARAMPREVIEW_H
