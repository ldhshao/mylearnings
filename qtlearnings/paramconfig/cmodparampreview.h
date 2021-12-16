#ifndef CMODPARAMPREVIEW_H
#define CMODPARAMPREVIEW_H

#include <QDialog>
#include <list>
#include <vector>
using namespace std;

class UiCfgItem;
class QPushButton;

namespace Ui {
class CModParamPreview;
}

class CModParamPreview : public QDialog
{
    Q_OBJECT

public:
    explicit CModParamPreview(list<UiCfgItem*>* iLst, QWidget *parent = nullptr);
    ~CModParamPreview();

protected slots:
    void slot_operateParam();

protected:
    QWidget* getCloseWidget(int x, int y);
    int getCloseCell(int x, int r);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_sync_clicked();

private:
    void initTable();

    Ui::CModParamPreview *ui;
    list<UiCfgItem*>     *itemList;
    vector<QPushButton*>  btnList;
};

#endif // CMODPARAMPREVIEW_H
