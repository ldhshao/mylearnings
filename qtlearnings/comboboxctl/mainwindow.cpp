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
    cmbSet = new CKeyDnComboBoxSet(this);
    QStringList params;
    params<<"item 0"<<"item 1"<<"item 2"<<"item 3";
    for (int i=0; i<5; i++) {
        data[i] = 0;
    }
    cmbSet->initComboBox(params);
    cmbSet->initData(data, 5);
    connect(cmbSet, SIGNAL(sig_dataIndexChanged(int)), this, SLOT(slot_dataIndexChanged(int)));
    nameLbl->move(100, 100);
    cmbSet->move(160, 100);
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
