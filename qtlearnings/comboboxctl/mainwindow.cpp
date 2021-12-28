#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ckeydncomboboxset.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    nameLbl = new QLabel(this);
    nameLbl->setText("index 0");
    cmbSet = new CKeyDnComboBoxEx(this);
    QStringList params;
    params<<"item 0"<<"item 1"<<"item 2"<<"item 3";
    for (int i=0; i<TEST_DATA_COUNT; i++) {
        data[i] = 0;
    }
    CKeyDnSetIndexEdit *pEdit = new CKeyDnSetIndexEdit(this);
    pEdit->initData(data, TEST_DATA_COUNT, TEST_DATA_SETSIZE);
    cmbSet->initComboBox(params);
    //cmbSet->initData(data, TEST_DATA_COUNT);
    connect(cmbSet, SIGNAL(sig_dataIndexChanged(int)), this, SLOT(slot_dataIndexChanged(int)));
    connect(pEdit, SIGNAL(sig_dataSetChanged(uint16_t*, int)), cmbSet, SLOT(slot_dataSetChanged(uint16_t*, int)));
    nameLbl->move(100, 80);
    cmbSet->move(160, 80);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_dataIndexChanged(int dataIdx)
{
    nameLbl->setText(QString::asprintf("index %d", dataIdx));
}

void MainWindow::on_pushButton_clicked()
{
    cmbSet->dump();
}
