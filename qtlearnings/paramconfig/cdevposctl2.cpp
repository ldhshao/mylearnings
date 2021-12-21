#include "cdevposctl2.h"
#include "ui_cdevposctl2.h"
#include "ckeycheckbox.h"
#include <QLabel>
#include <QCheckBox>
#include <QKeyEvent>
#include <QDebug>

#define PROPERTY_INDEX "index"
#define make_pos(m, p) ((m)<<16) + (p)
#define get_machine(pos) ((pos)>>16)
#define get_port(pos)    ((pos) & 0xFFFF)

CDevPosCtl2::CDevPosCtl2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CDevPosCtl2)
{
    ui->setupUi(this);
    currChk = nullptr;
    initAllCtls();
    QStringList lineNames = CDevPosMgr::instance()->getLineNames();
    ui->comboBox_line->insertItem(0, "选择沿线");
    ui->comboBox_line->insertItems(1, lineNames);
}

CDevPosCtl2::~CDevPosCtl2()
{
    delete ui;
}

void CDevPosCtl2::initAllCtls()
{
    int iRows = MACHINE_MAX;
    int iCols = PORT_MAX;
    int iX = ui->comboBox_line->pos().rx() + ui->comboBox_line->width();
    int iY = ui->comboBox_line->pos().ry();
    int iItemSpan = 10;
    int iLableWidth = 60, iLablHeight = 24;
    int iChkWidth = 40, iChkHeight = 20;

    for(int i = 0; i < iRows; i++){
        int iX0 = iX + iItemSpan;
        int iY0 = iY + i * (iChkHeight + 4);
        QLabel *lblName = new QLabel(QString::asprintf("下位机%d", i+1), this);
        lblName->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        lblName->resize(iLableWidth, iLablHeight);
        lblName->move(iX0, iY0);
        //lblName->hide();
        lblMachines[i] = lblName;
        iX0 += (lblName->width() + iItemSpan);
        for (int j = 0; j < iCols; j++) {
            QCheckBox* pChk = new CKeyCheckBox(QString::asprintf("%d", j+1), this);
            pChk->resize(iChkWidth, iChkHeight);
            pChk->move(iX0, iY0);
            //pChk->hide();
            chkPorts[i*PORT_MAX + j] = pChk;
            pChk->setProperty(PROPERTY_INDEX, make_pos(i, j));
            iX0 += (pChk->width() + 4);
            connect(pChk, SIGNAL(stateChanged(int)), this, SLOT(on_checkbox_stateChanged(int)));
            qDebug()<<pChk<<" chkable "<<pChk->isCheckable();
        }
    }

    QWidget::setTabOrder(ui->comboBox_line, chkPorts[0]);
    for (int i = 1; i < MACHINE_MAX*PORT_MAX; i++){
        QWidget::setTabOrder(chkPorts[i-1], chkPorts[i]);
    }
    QWidget::setTabOrder(chkPorts[MACHINE_MAX*PORT_MAX-1], ui->pushButton_cancel);
    QWidget::setTabOrder(ui->pushButton_cancel, ui->pushButton_ok);

}
void CDevPosCtl2::showCtlsByPortList(const list<list<bool>> &portList)
{
    int iX0 = ui->comboBox_line->x() + ui->comboBox_line->width() + 20 + ui->pushButton_cancel->width() + ui->pushButton_ok->width() + 10;
    int iY0 = ui->comboBox_line->y();
    int iRow = 0, iCol = 0;
    machineCount = portList.size();
    if (!portList.empty()){
        auto itRow = portList.begin();
        for (; itRow != portList.end(); itRow++) {
            int j = 0;
            for (auto itCol = itRow->begin(); itCol != itRow->end(); itCol++) {
                chkPorts[iRow*PORT_MAX + j]->setChecked(*itCol);
                chkPorts[iRow*PORT_MAX + j]->setEnabled(!(*itCol));
                chkPorts[iRow*PORT_MAX + j]->show();
                j++;
            }
            if (0 == iRow){
                QCheckBox* pBox = chkPorts[iRow * PORT_MAX + j - 1];
                iX0 = pBox->x() + pBox->width();
            }
            for (; j < PORT_MAX; j++) {
                chkPorts[iRow*PORT_MAX + j]->hide();
            }
            lblMachines[iRow]->show();
            iRow++;
        }
        iY0 = chkPorts[(iRow-1)*PORT_MAX]->y() + chkPorts[(iRow-1)*PORT_MAX]->height() + 20;
    }

    for (; iRow < MACHINE_MAX; iRow++) {
        lblMachines[iRow]->hide();
        for (int j=0; j<PORT_MAX; j++) {
            chkPorts[iRow*PORT_MAX + j]->hide();
        }
    }
    resize(iX0 + 10, iY0 + ui->pushButton_ok->height() + 10);

    iX0 = iX0 - ui->pushButton_ok->width() - ui->pushButton_cancel->width() - 10;
    ui->pushButton_cancel->move(iX0, iY0);
    ui->pushButton_ok->move(iX0 + ui->pushButton_cancel->width() + 10, iY0);
}

void CDevPosCtl2::keyPressEvent(QKeyEvent *event)
{
    QWidget *focus = focusWidget();
    bool dealed = false;
    switch (event->key()) {
    case Qt::Key_Up:
        if (focus->inherits("QCheckBox")){
            QCheckBox *pBox = dynamic_cast<QCheckBox*>(focus);
            unsigned int pos = pBox->property(PROPERTY_INDEX).toUInt();
            int m = get_machine(pos);
            int i = 0;
            do {
                m = ((m-1 +machineCount) % machineCount);
                i++;
            }while(!chkPorts[m*PORT_MAX + get_port(pos)]->isEnabled() && i < machineCount);
            if (i < machineCount) chkPorts[m*PORT_MAX + get_port(pos)]->setFocus();
            dealed = true;
        }else {
            focusNextPrevChild(false);
        }
        break;
    case Qt::Key_Down:
        if (focus->inherits("QCheckBox")){
            QCheckBox *pBox = dynamic_cast<QCheckBox*>(focus);
            unsigned int pos = pBox->property(PROPERTY_INDEX).toUInt();
            int m = get_machine(pos);
            int i = 0;
            do {
                m = ((m+1) % machineCount);
                i++;
            }while(!chkPorts[m*PORT_MAX + get_port(pos)]->isEnabled() && i < machineCount);
            if (i < machineCount) chkPorts[m*PORT_MAX + get_port(pos)]->setFocus();
            dealed = true;
        }else {
            focusNextPrevChild(true);
        }
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

void CDevPosCtl2::on_checkbox_stateChanged(int newState)
{
    if (!chkLoading){
        QCheckBox* chk = dynamic_cast<QCheckBox*>(sender());
        int l = ui->comboBox_line->currentIndex() - 1;
        bool use = false;
        if (Qt::Checked == newState){
            use = true;
            currChk = chk;
        }else{
            currChk = nullptr;
        }
        unsigned int pos = chk->property(PROPERTY_INDEX).toUInt();
        //CDevPosMgr::instance()->setPortValue(l, get_machine(pos), get_port(pos), use);
        //for (int i = 0; i < MACHINE_MAX*PORT_MAX; i++) {
        //    if (!chkPorts[i]->isChecked()){
        //        chkPorts[i]->setEnabled(!use);
        //    }
        //}
        //ui->comboBox_line->setEnabled(!use);
        qDebug()<<"sender "<<chk<<"state "<<newState;
    }
}

void CDevPosCtl2::on_comboBox_line_currentIndexChanged(int index)
{
    int line = index - 1;
    chkLoading = true;
    //list<list<bool>> portList = CDevPosMgr::instance()->getLinePorts(line);
    //showCtlsByPortList(portList);
    //chkLoading = false;
}

void CDevPosCtl2::on_pushButton_cancel_clicked()
{
    if(nullptr != currChk && currChk->isChecked()){
        int l = ui->comboBox_line->currentIndex() - 1;
        unsigned int pos = currChk->property(PROPERTY_INDEX).toUInt();
        //CDevPosMgr::instance()->setPortValue(l, get_machine(pos), get_port(pos), false);
    }
    close();
}

void CDevPosCtl2::on_pushButton_ok_clicked()
{
    close();
}
