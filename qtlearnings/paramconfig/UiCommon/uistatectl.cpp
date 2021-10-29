#include "uistatectl.h"
#include <QDebug>
#define MENU_NORMAL "QPushButton{background-color: rgba(255, 255, 255, 100%);}"
#define MENU_SELECTED "QPushButton{background-color: rgba(200, 60, 60, 100%);}"
#define EDIT_FOCUSED "QLineEdit {  border: 2px solid rgba(200,60,60,100%);}"
#define EDIT_UNFOCUS "QLineEdit { }"
#define COMBOBOX_FOCUSED "QComboBox { border: 2px solid rgba(200,60,60,100%); }"
#define COMBOBOX_UNFOCUS "QComboBox { }"
//#define CHECKBOX_FOCUSED "QCheckBox::indicator { color: rgba(200,60,60,100%);}"
#define CHECKBOX_FOCUSED "QCheckBox { }"
#define CHECKBOX_UNFOCUS "QCheckBox { }"
/*
QCheckBox::indicator:focus{
    border:-10px;
}
QCheckBox::indicator{
    position: absolute;
    top: 0px;
    left: 0px;
    color: #758794;
    width: 100%;
    padding: 1px 0 0 0;
}
QCheckBox{
     spacing:10px;
}*/

void CStateCtlMgr::selectCtl(CStateCtl *pCtl)
{
    if (nullptr != pLastCtl)
        pLastCtl->setState(CStateCtl::CTLS_UNSELECT);
    pLastCtl = pCtl;
    pCtl->setState(CStateCtl::CTLS_SELECTED);
}

//CStateLineEdit
CStateLineEdit::CStateLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
}

void CStateLineEdit::setState(CTL_STATE state)
{
    switch (state) {
    case CTLS_SELECTED:
        setStyleSheet(EDIT_FOCUSED);
        break;
    case CTLS_UNSELECT:
        setStyleSheet(EDIT_UNFOCUS);
        break;
    default:
        ;
    }
    qDebug()<<styleSheet();
}
void CStateLineEdit::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event)
    if (nullptr != pManager){
        pManager->selectCtl(this);
    }
}


//CStateComboBox
CStateComboBox::CStateComboBox(QWidget *parent)
    : QComboBox(parent)
{
}

void CStateComboBox::setState(CTL_STATE state)
{
    switch (state) {
    case CTLS_SELECTED:
        setStyleSheet(COMBOBOX_FOCUSED);
        break;
    case CTLS_UNSELECT:
        setStyleSheet(COMBOBOX_UNFOCUS);
        break;
    default:
        ;
    }
}
void CStateComboBox::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event)
    if (nullptr != pManager){
        pManager->selectCtl(this);
    }
}


//CStateCheckBox
CStateCheckBox::CStateCheckBox(QWidget *parent)
    : QCheckBox(parent)
{
}

void CStateCheckBox::setState(CTL_STATE state)
{
    switch (state) {
    case CTLS_SELECTED:
        setStyleSheet(CHECKBOX_FOCUSED);
        break;
    case CTLS_UNSELECT:
        setStyleSheet(CHECKBOX_UNFOCUS);
        break;
    default:
        ;
    }
}
void CStateCheckBox::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event)
    if (nullptr != pManager){
        pManager->selectCtl(this);
    }
}
