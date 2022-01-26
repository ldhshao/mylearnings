#ifndef CPARAMTABLEMNGRWID_H
#define CPARAMTABLEMNGRWID_H

#include <QWidget>
#include <vector>
using namespace std;

//class UiCfgItem;
class QPushButton;
class QLabel;


namespace Ui {
class CParamTableMngrWid;
}

class CParamTableMngrWid : public QWidget
{
    Q_OBJECT

public:
    explicit CParamTableMngrWid(QWidget *parent = nullptr);
    ~CParamTableMngrWid();

    void showUi();
    void setParamAddr(uint16_t* srvAddr, uint16_t* lclAddr) { paramSrvAddr = srvAddr; paramLclAddr = lclAddr; }

protected:
    void autoSetFocus();
    void showParamFiles();
    void showMessage(QString msg);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void changeEvent(QEvent *event);

private slots:
    void on_pushButton_bakeup_clicked();
    void on_pushButton_restore_clicked();
    void on_pushButton_delete_clicked();
    void on_pushButton_preview_clicked();

private:
    Ui::CParamTableMngrWid *ui;
    QLabel             *bkLbl;
    vector<QPushButton*>  btnList;
    uint16_t           *paramSrvAddr;
    uint16_t           *paramLclAddr;
    QString             workDir;
    int                 fileNameMax;
    int                 fileNumMax;
};

#endif // CPARAMTABLEMNGRWID_H
