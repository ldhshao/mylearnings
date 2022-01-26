#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidgetItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidget->setRowCount(4);
    for (int i = 0; i < 4; i++)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem(QString::asprintf("%d", i+1));
        newItem->setData(0, i);
        newItem->setData(1, i*i);
        ui->tableWidget->setItem(i, 0, newItem);
        newItem = new QTableWidgetItem(QString::asprintf("Name%03d", i+1));
        ui->tableWidget->setItem(i, 1, newItem);
    }

    for (int i = 0; i < 4; i++)
    {
        QTableWidgetItem *item = ui->tableWidget->item(i, 0);
        qDebug()<<i<<" 0:"<<item->data(0)<<" 1:"<<item->data(1);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
