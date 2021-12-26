#include "ckeydncomboboxset.h"
#include "ui_ckeydncomboboxset.h"
#include <QKeyEvent>
#include <QDebug>

CKeyDnComboBoxSet::CKeyDnComboBoxSet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CKeyDnComboBoxSet)
{
    pData = nullptr;
    dataCnt = 0;
    dataIdx = -1;
    ui->setupUi(this);
}

CKeyDnComboBoxSet::~CKeyDnComboBoxSet()
{
    delete ui;
}

void CKeyDnComboBoxSet::initComboBox(const QStringList &params)
{
    ui->comboBox->clear();
    ui->comboBox->addItems(params);
}

void CKeyDnComboBoxSet::initData(uint16_t* pAddr, int cnt)
{
    if (nullptr != pAddr && 0 < cnt){
        pData = pAddr;
        dataCnt = cnt;
        dataIdx = 0;
        setDataIndex(dataIdx);
        emit sig_dataIndexChanged(dataIdx);
    }
}

void CKeyDnComboBoxSet::setDataIndex(int idx)
{
    if (-1 < idx && idx < dataCnt){
        ui->comboBox->setCurrentIndex(pData[idx]);
        dataIdx = idx;
    }
}

void CKeyDnComboBoxSet::keyPressEvent(QKeyEvent *event)
{
    int iCurr = ui->comboBox->currentIndex();
    int iCnt = ui->comboBox->count();
    switch (event->key()) {
    case Qt::Key_Left:
        iCurr = (iCurr - 1 + iCnt) % iCnt;
        ui->comboBox->setCurrentIndex(iCurr);
        break;
    case Qt::Key_Right:
        iCurr = (iCurr + 1) % iCnt;
        ui->comboBox->setCurrentIndex(iCurr);
        break;
    case Qt::Key_T:
        on_pushButton_prev_clicked();
        break;
    case Qt::Key_K:
        on_pushButton_next_clicked();
        break;
    default:
        QWidget::keyPressEvent(event);
        break;
    }
}

void CKeyDnComboBoxSet::focusInEvent(QFocusEvent *event)
{
    //
}

void CKeyDnComboBoxSet::resizeEvent(QResizeEvent *event)
{
    //
}

void CKeyDnComboBoxSet::on_pushButton_prev_clicked()
{
    if (-1 < dataIdx){
        dataIdx = (dataIdx - 1 + dataCnt) % dataCnt;
        setDataIndex(dataIdx);
        emit sig_dataIndexChanged(dataIdx);
    }
}

void CKeyDnComboBoxSet::on_pushButton_next_clicked()
{
    if (-1 < dataIdx){
        dataIdx = (dataIdx + 1) % dataCnt;
        setDataIndex(dataIdx);
        emit sig_dataIndexChanged(dataIdx);
    }
}

void CKeyDnComboBoxSet::dump()
{
    if (0 < dataCnt){
        for (int i = 0; i < dataCnt; i++) {
            qWarning()<<i<<pData[i];
        }
    }
}
