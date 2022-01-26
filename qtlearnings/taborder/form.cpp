#include "form.h"
#include "ui_form.h"
#include <myedit.h>
#include <QKeyEvent>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    int i = 0;
    MyEdit *pEdit1 =new MyEdit(this), *pEdit2;
    pEdit1->move(10, 30*i++);
    pEdit2 = new MyEdit(this);
    pEdit2->move(10, 30*i++);
    QWidget::setTabOrder(pEdit1, pEdit2);
    pEdit1 = pEdit2;
    pEdit2 = new MyEdit(this);
    pEdit2->move(10, 30*i++);
    QWidget::setTabOrder(pEdit1, pEdit2);
    pEdit1 = pEdit2;
}

Form::~Form()
{
    delete ui;
}

void Form::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Down:
        focusNextPrevChild(true);
        //event->setAccepted(true);
        break;
    case Qt::Key_Up:
        focusNextPrevChild(false);
        //event->setAccepted(true);
        break;
    }
}
