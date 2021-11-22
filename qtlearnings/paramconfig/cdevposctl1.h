#ifndef CDEVPOSCTL1_H
#define CDEVPOSCTL1_H

#include <QWidget>
#define MACHINE_CHECKBOX_MAX 12

class QCheckBox;
namespace Ui {
class CDevPosCtl1;
}

class CDevPosCtl1 : public QWidget
{
    Q_OBJECT

public:
    explicit CDevPosCtl1(QWidget *parent = nullptr);
    ~CDevPosCtl1();

private slots:
    void on_checkbox_stateChanged(int newState);
    void on_comboBox_line_currentIndexChanged(int index);

    void on_comboBox_machine_currentIndexChanged(int index);

    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

private:
    Ui::CDevPosCtl1 *ui;
    QCheckBox       *chks[MACHINE_CHECKBOX_MAX];
    QCheckBox       *currChk;
    bool             chkLoading;
};

#endif // CDEVPOSCTL1_H
