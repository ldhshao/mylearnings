#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pForm = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<focusWidget();
    switch (event->key()) {
    case Qt::Key_Down:
        focusNextPrevChild(true);
        break;
    case Qt::Key_Up:
        focusNextPrevChild(false);
        break;
    }

    event->setAccepted(true);
    qDebug()<<focusWidget();
    //QWidget::keyPressEvent(event);
}

void MainWindow::on_pushButton_clicked()
{
    if(nullptr == pForm)
        pForm = new Form (this);
    pForm->move(10, 20);
    pForm->show();
}
