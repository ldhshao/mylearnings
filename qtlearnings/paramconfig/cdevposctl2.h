#ifndef CDEVPOSCTL2_H
#define CDEVPOSCTL2_H
#include "cdevposmgr.h"
#include <QWidget>

namespace Ui {
class CDevPosCtl2;
}
class QLabel;
class QCheckBox;

class CDevPosCtl2 : public QWidget
{
    Q_OBJECT

public:
    explicit CDevPosCtl2(QWidget *parent = nullptr);
    ~CDevPosCtl2();

protected:
    void initAllCtls();
    void showCtlsByPortList(const list<list<bool>> &portList);

    virtual void keyPressEvent(QKeyEvent *event);

protected slots:
    void on_checkbox_stateChanged(int newState);

private slots:
    void on_comboBox_line_currentIndexChanged(int index);

    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

private:
    Ui::CDevPosCtl2 *ui;
    QLabel *lblMachines[MACHINE_MAX];
    QCheckBox *chkPorts[MACHINE_MAX*PORT_MAX];
    QCheckBox *currChk;
    bool       chkLoading;
    int        machineCount;
};

#endif // CDEVPOSCTL2_H
