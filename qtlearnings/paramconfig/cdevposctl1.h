#ifndef CDEVPOSCTL1_H
#define CDEVPOSCTL1_H

#include <QWidget>
#define MACHINE_CHECKBOX_MAX 12

class QCheckBox;
class CDevPointEdit;
namespace Ui {
class CDevPosCtl1;
}

class CDevPosCtl1 : public QWidget
{
    Q_OBJECT

public:
    static CDevPosCtl1* instance();
    void setAttachEdit(CDevPointEdit* e);

protected:
    explicit CDevPosCtl1(QWidget *parent = nullptr);
    ~CDevPosCtl1();
    virtual void keyPressEvent(QKeyEvent *event);

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
    CDevPointEdit   *edit;
};

#endif // CDEVPOSCTL1_H
