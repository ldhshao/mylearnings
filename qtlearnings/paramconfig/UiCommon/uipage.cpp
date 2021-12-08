#include "uipage.h"
#include "ckeydnedit.h"
#include "ckeydncombobox.h"
#include <QKeyEvent>
#include <QDebug>

UiPage::UiPage(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(/*Qt::Tool |*/ Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
}

UiPage::~UiPage()
{
    //deleteAll();
}

void UiPage::deleteAll()
{
    list<QWidget*>::iterator it = ctlList.begin();
    for (; it != ctlList.end(); it++) {
        delete *it;
    }

    ctlList.clear();
}

void UiPage::addWidget(QWidget *w)
{
    ctlList.push_back(w);
    //CStateCtl *pCtl = dynamic_cast<CStateCtl*>(w);
    //if (nullptr != pCtl)
    //    pCtl->setManager(&ctlMgr);
}

QWidget* UiPage::getHead()
{
    m_it = ctlList.begin();
    if (ctlList.end() != m_it)
        return *m_it;

    return nullptr;
}
QWidget* UiPage::getNext()
{
    if (ctlList.end() != m_it){
        m_it++;
        if (ctlList.end() != m_it){
            return *m_it;
        }
    }

    return nullptr;
}

void UiPage::initTabOrder()
{
    map<int, map<int,QWidget*>> widMap;//setting tab order
    list<QWidget*>::iterator it = ctlList.begin();
    for (; it != ctlList.end(); it++) {
        //CKeyDnEdit* pEdit = dynamic_cast<CKeyDnEdit*>(*it);
        //CKeyDnComboBox* pCmb = dynamic_cast<CKeyDnComboBox*>(*it);
        QLineEdit* pEdit = dynamic_cast<QLineEdit*>(*it);
        QComboBox* pCmb = dynamic_cast<QComboBox*>(*it);
        if (nullptr == pEdit && nullptr == pCmb){
            continue;
        }

        widMap[(*it)->pos().rx()][(*it)->pos().ry()] = *it;
    }
    //set control tab order
    QWidget *wid = nullptr;
    for(auto itCol = widMap.begin(); itCol != widMap.end(); itCol++){
        auto itRow = itCol->second.begin();
        if (nullptr == wid){
            wid = itRow->second;
            itRow++;
        }
        for(;itRow != itCol->second.end(); itRow++){
            QWidget::setTabOrder(wid, itRow->second);
            wid = itRow->second;
            qDebug()<<"col"<<itCol->first<<"row"<<itRow->first;
        }
    }
}

void UiPage::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<focusWidget();
    switch (event->key()) {
    case Qt::Key_Down:
        focusNextPrevChild(true);
        event->setAccepted(true);
        break;
    case Qt::Key_Up:
        focusNextPrevChild(false);
        event->setAccepted(true);
        break;
    case Qt::Key_Return:
        hide();
        emit sig_configFinished();
        break;
    }
    qDebug()<<focusWidget();
}

void UiPage::focusInEvent(QFocusEvent *event)
{
    qDebug()<<"UiPage::focusInEvent";
    //qDebug()<<"CMachineSelector active "<<CMachineSelector::instance()->isActiveWindow();
    //qDebug()<<"CPortSelector active "<<CPortSelector::instance()->isActiveWindow();
    //qDebug()<<"Application focus widget "<<QApplication::focusWidget();
    //qDebug()<<"Application activeWindow "<<QApplication::activeWindow();
    //if (!CMachineSelector::instance()->isActiveWindow() && !CPortSelector::instance()->isActiveWindow()){
    //    prevSelector();
    //}
}
