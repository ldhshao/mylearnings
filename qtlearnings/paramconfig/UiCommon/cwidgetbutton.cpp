#include "cwidgetbutton.h"

#define MENU_NORMAL "QPushButton{color:rgba(255,255,255,100%);background-color: rgba(80, 80, 100, 100%);}"
#define MENU_SELECTED "QPushButton{color:rgba(255,255,255,100%); background-color: rgba(200, 60, 60, 100%);}"

CStateButton::CStateButton(QWidget *parent):
    QPushButton(parent)
{
    setButtonState(BS_NORMAL);
    connect(this, SIGNAL(clicked(bool)), this, SLOT(slot_clicked(bool)));
}
void CStateButton::setButtonState(BUTTON_STATE state)
{
    QString strStyle = styleSheet();
    if (BS_SELECTED == state)
        strStyle.append(MENU_SELECTED);
    else
        strStyle.append(MENU_NORMAL);
    setStyleSheet(strStyle);
}
void CStateButton::slot_clicked(bool)
{
    //setButtonState(BS_SELECTED);
    emit sig_button_clicked(this);
}

CWidgetButton::CWidgetButton(QWidget *w, QWidget *parent)
    :CStateButton(parent), pWidget(w)
{

}


CStateButtonMgr::CStateButtonMgr(QObject *parent):
    QObject(parent), pLastBtn(nullptr)
{

}

void CStateButtonMgr::registerButton(CStateButton* pBtn)
{
    connect(pBtn, SIGNAL(sig_button_clicked(CStateButton*)), this, SLOT(slot_btnClicked(CStateButton*)));
}
void CStateButtonMgr::selectButton(CStateButton* pBtn)
{
    slot_btnClicked(pBtn);
}

void CStateButtonMgr::slot_btnClicked(CStateButton* pBtn)
{
    if (nullptr != pLastBtn)
        pLastBtn->setButtonState(CStateButton::BS_NORMAL);
    pLastBtn = pBtn;
    pBtn->setButtonState(CStateButton::BS_SELECTED);

    emit sig_button_clicked(pBtn);
}

CWidgetButtonMgr::CWidgetButtonMgr(QObject *parent):
    QObject(parent), pLastBtn(nullptr)
{

}
CWidgetButtonMgr::~CWidgetButtonMgr()
{
    clear();
}

CWidgetButton* CWidgetButtonMgr::addWidgetButton(QWidget* w, QString strName)
{
    CWidgetButton* pBtn = new CWidgetButton(w);
    pBtn->setText(strName);

    mapButtons[w] = pBtn;
    connect(pBtn, SIGNAL(sig_button_clicked(CStateButton*)), this, SLOT(slot_btnClicked(CStateButton*)));
    return pBtn;
}

void CWidgetButtonMgr::clear()
{
    map<QWidget*, CWidgetButton*>::iterator it = mapButtons.begin();
    for (; it != mapButtons.end(); it++){
        delete it->second;
    }
    mapButtons.clear();
    pLastBtn = nullptr;
}

void CWidgetButtonMgr::slot_btnClicked(CStateButton* pBtn)
{
    _selectButton(pBtn);
    emit sig_showWidget((dynamic_cast<CWidgetButton*>(pBtn))->getWidget());
}

void CWidgetButtonMgr::slot_selectButton(QWidget *pSubWid)
{
    if(nullptr != pSubWid){
        map<QWidget*, CWidgetButton*>::iterator it = mapButtons.find(pSubWid);
        if (it != mapButtons.end()){
            _selectButton(it->second);
        }
    }
}

void CWidgetButtonMgr::_selectButton(CStateButton* pBtn)
{
    if (nullptr != pLastBtn)
        pLastBtn->setButtonState(CStateButton::BS_NORMAL);
    pLastBtn = dynamic_cast<CWidgetButton*>(pBtn);
    pBtn->setButtonState(CStateButton::BS_SELECTED);
}
