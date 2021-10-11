#include "uistatectl.h"
#include <QDebug>
#define MENU_NORMAL "QPushButton{background-color: rgba(255, 255, 255, 100%);}"
#define MENU_SELECTED "QPushButton{background-color: rgba(200, 60, 60, 100%);}"
#define EDIT_FOCUSED "QLineEdit {  border: 2px solid gray; background: yellow; selection-background-color: darkgray;}"
#define EDIT_UNFOCUS "QLineEdit { }"
#define COMBOBOX_FOCUSED "QComboBox {  border: 2px solid gray; background: yellow; }"
#define COMBOBOX_UNFOCUS "QComboBox { }"
#define CHECKBOX_FOCUSED "QCheckBox {  border: 2px solid gray; background: yellow; }"
#define CHECKBOX_UNFOCUS "QCheckBox { }"

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
