#include "cwidgetbutton.h"
#include <QKeyEvent>
#include <QDebug>

#define MENU_NORMAL_ORIGIN "color:rgba(255,255,255,100%);background-color: rgba(80, 80, 100, 100%);"
#define MENU_NORMAL "background-color: rgba(80, 80, 100, 100%)"
#define MENU_SELECTED "background-color: rgba(200, 60, 60, 100%)"

CStateButton::CStateButton(QWidget *parent):
    QPushButton(parent)
{
    setStyleSheet(MENU_NORMAL_ORIGIN);
    setButtonState(BS_NORMAL);
    connect(this, SIGNAL(clicked(bool)), this, SLOT(slot_clicked(bool)));
}
void CStateButton::setButtonState(BUTTON_STATE state)
{
    QString strStyle = styleSheet();
    strStyle = removeStyle(strStyle, "background-color");
    if (BS_SELECTED == state)
        strStyle.append(";").append(MENU_SELECTED);
    else
        strStyle.append(";").append(MENU_NORMAL);
    setStyleSheet(strStyle);
}
void CStateButton::slot_clicked(bool)
{
    //setButtonState(BS_SELECTED);
    emit sig_button_clicked(this);
}

void CStateButton::addStyleSheet(const QString& strStyle)
{
    QStringList oldList = styleSheet().split(';');
    QString dstStyle;
    for (int i = 0; i < oldList.count(); i++) {
        QStringList keyVal = oldList[i].split(':');
        if (2 == keyVal.count() && -1 == strStyle.indexOf(keyVal[0])){
            dstStyle.append(oldList[i] + ";");
        }
    }
    dstStyle.append(strStyle);
    setStyleSheet(dstStyle);
}

QString CStateButton::removeStyle(const QString& strStyle, QString strAttr)
{
    QStringList dstList;
    QStringList styleList = strStyle.split(';');
    for (int i = 0; i < styleList.count(); i++) {
        if (-1 == styleList[i].indexOf(strAttr))
            dstList.append(styleList[i]);
    }
    return dstList.join(";");
}

CWidgetButton::CWidgetButton(QWidget *w, QWidget *parent)
    :CStateButton(parent), pWidget(w)
{

}

void CWidgetButton::setButtonState(BUTTON_STATE state)
{
    CStateButton::setButtonState(state);
    if (nullptr != pWidget){
        if (BS_SELECTED == state)
            pWidget->show();
        else
            pWidget->hide();
    }
}

CKeyStateButton::CKeyStateButton(QWidget *parent)
    :CStateButton(parent)
{

}
void CKeyStateButton::keyPressEvent(QKeyEvent *event)
{
    //qDebug()<<"CKeyStateButton "<<__FUNCTION__<<event->key();
    QWidget *focus = focusWidget();
    switch (event->key()) {
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_Return:
    case Qt::Key_Escape:
    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
        event->setAccepted(false);
        break;
    default:
        break;
    }
}

CKeyWidgetButton::CKeyWidgetButton(QWidget *w, QWidget *parent)
    :CKeyStateButton(parent), pWidget(w)
{

}

void CKeyWidgetButton::setButtonState(BUTTON_STATE state)
{
    CStateButton::setButtonState(state);
    if (nullptr != pWidget){
        if (BS_SELECTED == state)
            pWidget->show();
        else
            pWidget->hide();
    }
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
    if (nullptr != pBtn){
        //pBtn->setFocus();
        pBtn->setButtonState(CStateButton::BS_SELECTED);

        emit sig_button_clicked(pBtn);
    }
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
