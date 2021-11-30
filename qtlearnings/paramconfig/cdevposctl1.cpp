#include "cdevposctl1.h"
#include "ui_cdevposctl1.h"
#include "cdevposmgr.h"
#include "cdevpointedit.h"
#include <QKeyEvent>
#include <QDebug>

#define PROPERTY_INDEX "index"

CDevPosCtl1* CDevPosCtl1::instance(){
    static CDevPosCtl1 _ins;
    return &_ins;
}

CDevPosCtl1::CDevPosCtl1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CDevPosCtl1)
{
    ui->setupUi(this);
    currChk = nullptr;
    edit = nullptr;
    chkLoading = false;
    int i = 0;
    chks[i++] = ui->checkBox_1;
    chks[i++] = ui->checkBox_2;
    chks[i++] = ui->checkBox_3;
    chks[i++] = ui->checkBox_4;
    chks[i++] = ui->checkBox_5;
    chks[i++] = ui->checkBox_6;
    chks[i++] = ui->checkBox_7;
    chks[i++] = ui->checkBox_8;
    chks[i++] = ui->checkBox_9;
    chks[i++] = ui->checkBox_10;
    chks[i++] = ui->checkBox_11;
    chks[i++] = ui->checkBox_12;
    for (i = 0; i < MACHINE_CHECKBOX_MAX; i++) {
       chks[i]->hide();
       chks[i]->setProperty(PROPERTY_INDEX, i);
       connect(chks[i], SIGNAL(stateChanged(int)), this, SLOT(on_checkbox_stateChanged(int)));
    }

    QStringList lineNames = CDevPosMgr::instance()->getLineNames();
    ui->comboBox_line->insertItem(0, "选择沿线");
    ui->comboBox_line->insertItems(1, lineNames);
    ui->comboBox_machine->insertItem(0, "选择下位机");
}

CDevPosCtl1::~CDevPosCtl1()
{
    delete ui;
}


void CDevPosCtl1::setAttachEdit(CDevPointEdit *e)
{
    if (nullptr != e && !e->text().isEmpty()){
        QStringList ptList = e->text().split(".");
        if (3 == ptList.count()){
            ui->comboBox_line->setCurrentText(ptList[0]);
            ui->comboBox_machine->setCurrentText(ptList[1]);
            int port = ptList[2].toInt() - 1;
            if (0 <= port){
                for (int i = 0; i < MACHINE_CHECKBOX_MAX; i++) {
                    if (i != port) chks[i]->setEnabled(false);
                    else chks[i]->setEnabled(true);
                }
            }
        }
    }
    edit = e;
}

void CDevPosCtl1::keyPressEvent(QKeyEvent *event)
{
    QWidget *focus = focusWidget();
    bool dealed = false;
    switch (event->key()) {
    case Qt::Key_Up:
        focusNextPrevChild(false);
        break;
    case Qt::Key_Down:
        focusNextPrevChild(true);
        break;
    default:
        break;
    }

    if (Qt::Key_Return == event->key()){
        if (focus == ui->pushButton_cancel){
            on_pushButton_cancel_clicked();
            return ;
        }
        if (focus == ui->pushButton_ok){
            on_pushButton_ok_clicked();
            return ;
        }
    }

    event->setAccepted(dealed);
}

void CDevPosCtl1::on_checkbox_stateChanged(int newState)
{
    if (!chkLoading){
        QCheckBox* chk = dynamic_cast<QCheckBox*>(sender());
        int l = ui->comboBox_line->currentIndex() - 1;
        int m = ui->comboBox_machine->currentIndex() - 1;
        bool use = false;
        if (Qt::Checked == newState){
            use = true;
            currChk = chk;
        }else{
            currChk = nullptr;
        }
        for (int i = 0; i < MACHINE_CHECKBOX_MAX; i++) {
            if (!chks[i]->isChecked()){
                chks[i]->setEnabled(!use);
            }
            if (chk == chks[i]){
                CDevPosMgr::instance()->setPortValue(l, m, i, use);
            }
        }
        ui->comboBox_line->setEnabled(!use);
        ui->comboBox_machine->setEnabled(!use);
        qDebug()<<"sender "<<chk<<"state "<<newState;
    }
}

void CDevPosCtl1::on_comboBox_line_currentIndexChanged(int index)
{
    if (0 < index){
        int line = index - 1;
        int mCnt = CDevPosMgr::instance()->getMachineCount(line);
        int cmbCnt = ui->comboBox_machine->count();
        ui->comboBox_machine->setCurrentIndex(0);
        for (int i = 0; i < cmbCnt -1; i++)
            ui->comboBox_machine->removeItem(1);
        for (int i = 0; i < mCnt; i++) {
            ui->comboBox_machine->insertItem(i+1, QString::asprintf("%d", i+1));
        }
    }
}

void CDevPosCtl1::on_comboBox_machine_currentIndexChanged(int index)
{
    int line = ui->comboBox_line->currentIndex() -1;
    if (0 <= line){
        int m = index -1;
        list<bool> portList = CDevPosMgr::instance()->getMachinePorts(line, m);
        int i = 0;
        chkLoading = true;
        list<bool>::iterator it = portList.begin();
        for (; it!= portList.end(); it++,i++) {
            chks[i]->setChecked(*it);
            chks[i]->setEnabled(!(*it));
            chks[i]->show();
        }
        for (; i < MACHINE_CHECKBOX_MAX; i++) {
            chks[i]->hide();
        }
        chkLoading = false;
    }

}

void CDevPosCtl1::on_pushButton_cancel_clicked()
{
    if (nullptr != currChk && currChk->isChecked()){
        int l = ui->comboBox_line->currentIndex() - 1;
        int m = ui->comboBox_machine->currentIndex() - 1;

        CDevPosMgr::instance()->setPortValue(l, m, currChk->property(PROPERTY_INDEX).toInt(), false);
    }
    hide();
}

void CDevPosCtl1::on_pushButton_ok_clicked()
{
    if (nullptr != edit && nullptr != currChk){
        int l = ui->comboBox_line->currentIndex();
        int m = ui->comboBox_machine->currentIndex() - 1;
        int p = currChk->property(PROPERTY_INDEX).toInt();
        edit->setValue(make_dev_point(l, m, p));
    }
    hide();
}
