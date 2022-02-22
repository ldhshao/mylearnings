#include "uipage.h"
#include "ckeydnedit.h"
#include "ckeydncombobox.h"
#include "cdevpointedit.h"
#include "PageCfg.h"
#include <QLabel>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QToolTip>
#include <QDebug>

#define TITLE_STYLE      "font-size:20px;color:rgba(255,255,255,100%);"
#define LABEL_STYLE      "color:rgba(255,255,255,100%);"
#define EDIT_STYLE       "color:rgba(38,38,38,100%);"
//#define WIDGET_STYLE     "background-color:rgba(0,151,255,100%);"
#define WIDGET_STYLE     "background-color:rgba(80,80,100,100%);"

void addWidgetStyleSheet(QWidget* w, const QString& strStyle)
{
    QStringList oldList = w->styleSheet().split(';');
    QString dstStyle;
    for (int i = 0; i < oldList.count(); i++) {
        QStringList keyVal = oldList[i].split(':');
        if (2 == keyVal.count() && -1 == strStyle.indexOf(keyVal[0])){
            dstStyle.append(oldList[i] + ";");
        }
    }
    dstStyle.append(strStyle);
    w->setStyleSheet(dstStyle);
    qDebug()<<dstStyle<<strStyle;
}

UiPage::UiPage(QWidget *parent) :
    QWidget(parent)
{
    titleHeight = 50;
    initWidth = 200;
    initHeight = 200;
    init = false;
    setWindowFlags(/*Qt::Tool | Qt::WindowStaysOnTopHint |*/ Qt::FramelessWindowHint);
    bkLbl = new QLabel(this);
    bkLbl->setStyleSheet(WIDGET_STYLE);
    title = new QLabel(this);
    title->setAlignment(Qt::AlignCenter);
}

UiPage::~UiPage()
{
    //deleteAll();
}

void UiPage::deleteAll()
{
    delete title;
}

void UiPage::setTitle(QString strTitle)
{
    title->setText(strTitle);
}

void UiPage::fillColList(int col, QWidget* w)
{
    if (nullptr == w) return ;

    if (col <0 || col >= UIPAGE_COL_NUM) qDebug()<<"wrong pos "<<col;
    if(0 <= col && UIPAGE_COL_NUM > col)
        colList[col].push_back(w);
    //qDebug()<<"col "<<col<<" wid "<<w;
}

void UiPage::updateUi()
{
    QObjectList oList = children();
    for(auto it = oList.begin(); it != oList.end(); it++){
        CMyCtl* ctl = dynamic_cast<CMyCtl*>(*it);
        if (nullptr != ctl)
            ctl->updateText();
    }
}

void UiPage::initPage()
{
    addWidgetStyleSheet(title, TITLE_STYLE);
    for(int i = 0; i < UIPAGE_COL_NUM; i++){
        for(auto itRow = colList[i].begin(); itRow != colList[i].end(); itRow++){
            QLabel* pLbl = dynamic_cast<QLabel*>(*itRow);
            if (nullptr != pLbl)
                addWidgetStyleSheet(pLbl, LABEL_STYLE);
            else
                addWidgetStyleSheet(*itRow, EDIT_STYLE);
            //qDebug()<<"UiPage::initPage"<<(*itRow)->font().family()<<(*itRow)->font().pointSize()<<(*itRow)->font().weight();
        }
    }

    QWidget *wid = nullptr;
    for(int i = 0; i < UIPAGE_COL_NUM; i++){
        auto itRow = colList[i].begin();
        if (nullptr == wid){
            int j = 0;
            while (nullptr == wid && itRow != colList[i].end() && j < 3){
                QLineEdit* edit = dynamic_cast<QLineEdit*>(*itRow);
                if (nullptr != dynamic_cast<QLineEdit*>(*itRow) ||
                        nullptr != dynamic_cast<QComboBox*>(*itRow)){
                    wid = *itRow;
                    break;
                }
                itRow++;j++;
            }
            if (nullptr == wid) continue;
            itRow++;
        }
        for(;itRow != colList[i].end(); itRow++){
            if (nullptr != dynamic_cast<QLineEdit*>(*itRow) ||
                    nullptr != dynamic_cast<QComboBox*>(*itRow)){
                QWidget::setTabOrder(wid, *itRow);
                wid = *itRow;
            }
        }
    }
}

void UiPage::showPage()
{
    bool bFocus = false;
    updateUi();

    for(int i = 0; i < UIPAGE_COL_NUM; i++){
        for (auto it = colList[i].begin(); it != colList[i].end(); it++) {
            if (!bFocus && (nullptr != dynamic_cast<QLineEdit*>(*it) ||
                    nullptr != dynamic_cast<QComboBox*>(*it))){
                (*it)->setFocus();
                bFocus = true;
            }
            CKeyDnSetIndexEdit *pEdit = dynamic_cast<CKeyDnSetIndexEdit*>(*it);
            if (nullptr != pEdit){
                pEdit->setEditText("1");
            }
            CKeyDnComboBoxSet *pCmb = dynamic_cast<CKeyDnComboBoxSet*>(*it);
            if (nullptr != pCmb){
                pCmb->initUi();
            }
        }
    }
    show();
}

void UiPage::keyPressEvent(QKeyEvent *event)
{
    bool bEsc = false;
    //qDebug()<<focusWidget();
    switch (event->key()) {
    case Qt::Key_Down:
        focusNextPrevChild(true);
        event->setAccepted(true);
        if (nullptr != dynamic_cast<CDevPointEdit*>(focusWidget())){
            CDevPointEdit* pEdit = dynamic_cast<CDevPointEdit*>(focusWidget());
            showTipInfo(pEdit->mapToGlobal(QPoint(0, pEdit->height()/2)), pEdit->tipInfo());
        }else if (nullptr != dynamic_cast<CKeyDnSetEdit*>(focusWidget())){
            CKeyDnSetEdit* pEdit = dynamic_cast<CKeyDnSetEdit*>(focusWidget());
            showTipInfo(pEdit->mapToGlobal(QPoint(0, pEdit->height()/2)), pEdit->tipInfo());
        }
        break;
    case Qt::Key_Up:
        focusNextPrevChild(false);
        event->setAccepted(true);
        if (nullptr != dynamic_cast<CDevPointEdit*>(focusWidget())){
            CDevPointEdit* pEdit = dynamic_cast<CDevPointEdit*>(focusWidget());
            showTipInfo(pEdit->mapToGlobal(QPoint(0, pEdit->height()/2)), pEdit->tipInfo());
        }else if (nullptr != dynamic_cast<CKeyDnSetEdit*>(focusWidget())){
            CKeyDnSetEdit* pEdit = dynamic_cast<CKeyDnSetEdit*>(focusWidget());
            showTipInfo(pEdit->mapToGlobal(QPoint(0, pEdit->height()/2)), pEdit->tipInfo());
        }
        break;
    case Qt::Key_Escape:
        if (0 < mparamAddrList.size()){
            QMessageBox msgBox(QMessageBox::Question, "参数设置","参数已被修改， 是否保存?");
            QAbstractButton* cancelBtn = dynamic_cast<QAbstractButton*>(msgBox.addButton("不保存", QMessageBox::NoRole));
            msgBox.addButton("保存", QMessageBox::YesRole);
            msgBox.exec();
            if (!mparamAddrList.empty() && msgBox.clickedButton() == cancelBtn){
                qDebug()<<"don't save";
                emit sig_rollBack_paramAddrList(&mparamAddrList);
                updateUi();
                mparamAddrList.clear();
            }
        }
        bEsc = true;
    case Qt::Key_Return:
        if (!bEsc && !checkRequiredData())
            return ;
        //save modified param address to global modified param address list
        if (!mparamAddrList.empty()){
            emit sig_modifiedParamAddrList(&mparamAddrList);
        }
        mparamAddrList.clear();
        hide();
        emit sig_configFinished();
        //qDebug()<<"emit sig_configFinished()"<<this;
        break;
    }
    //qDebug()<<focusWidget();
}

void UiPage::focusInEvent(QFocusEvent *event)
{
    //qDebug()<<"UiPage::focusInEvent";
}

void UiPage::resizeEvent(QResizeEvent *event)
{
    if (init) return ;

    bkLbl->resize(event->size().width(), event->size().height());
    bkLbl->move(0, 0);

    initPage();

    QSize s0 = event->oldSize();
    QSize s1 = event->size();
    //qDebug()<<__FUNCTION__<<this<<" old "<<s0<<" new "<<s1;
    int cols = 0;
    for (int i = 0; i < UIPAGE_COL_NUM; i++) {
        if (0 == colList[i].size()){
            break;
        }
        cols++;
    }
    int deltX = (s1.width() - initWidth)/(cols+1);
    title->resize(s1.width(), titleHeight);
    title->move(0, 0);
    if (s0.width() > 0){
        deltX = (s1.width() - s0.width())/(cols+1);
    }
    //qDebug()<<"deltX "<<deltX<<" initSize() "<<initWidth<<" "<<initHeight;
    if (s1.width() > initWidth && s1.height() > initHeight){
        for (int i = 0; i < UIPAGE_COL_NUM; i++) {
            for (auto it = colList[i].begin(); it != colList[i].end(); it++) {
                int x = (*it)->pos().x();
                int y = (*it)->pos().y();
                (*it)->move(x + deltX*(i+1), y);
                //qDebug()<<*it<<" pos "<<(*it)->pos();
            }
        }
    }
    init = true;
}

void UiPage::slot_valueChanged(uint16_t* pVal, uint32_t newVal)
{
    bool bFind = false;
    for (auto it = mparamAddrList.begin(); it != mparamAddrList.end(); it++) {
        if( pVal == (*it)){
            bFind = true;
            break;
        }
    }
    if (!bFind){
        mparamAddrList.push_back(pVal);
        if (newVal > 0xFFFF) mparamAddrList.push_back(pVal+1);
    }
}

void UiPage::showTipInfo(QPoint pt, const QString& strTip)
{
    QToolTip::showText(pt, strTip);
}

bool UiPage::checkRequiredData()
{
    GroupCfgItem* grp = static_cast<GroupCfgItem*>(property("device").value<void*>());
    if (nullptr != grp){
        UiCfgItem *pItem = grp->getHead();
        while (nullptr != pItem){
            QWidget *w = pItem->getWidget();
            if (!pItem->isDataOK() && w->isVisible() && w->isEnabled()){
            QMessageBox msgBox(QMessageBox::Information, "参数设置", pItem->getName() + "是必设参数，请重新设置!");
            msgBox.addButton("确定", QMessageBox::YesRole);
            msgBox.exec();
            return false;
            }
            pItem = grp->getNext();
        }
    }
    return true;
}
