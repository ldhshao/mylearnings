#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Util/ItemBase.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    XmlList list;
    list.readXmlFile("/home/test/workspace/menu.txt");
    list.dump();
}
