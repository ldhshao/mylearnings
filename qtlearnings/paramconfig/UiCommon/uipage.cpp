#include "uipage.h"
#include "ckeydnedit.h"
#include "ckeydncombobox.h"
#include <QLabel>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QDebug>

UiPage::UiPage(QWidget *parent) :
    QWidget(parent)
{
    titleHeight = 80;
    initWidth = 200;
    initHeight = 200;
    init = false;
    setWindowFlags(/*Qt::Tool | Qt::WindowStaysOnTopHint |*/ Qt::FramelessWindowHint);
    title = new QLabel(this);
    title->setAlignment(Qt::AlignCenter);
    title->move(0, 0);
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

void UiPage::fillColList()
{
    QObjectList oList = children();
    for(auto it = oList.begin(); it != oList.end(); it++){
        if (title == *it) continue;
        fillColList(dynamic_cast<QWidget*>(*it));
    }
}
void UiPage::fillColList(QWidget* w)
{
    if (nullptr == w) return ;

    int col = w->pos().x();
    if (col <=0 || col > UIPAGE_COL_NUM) qDebug()<<"wrong pos "<<w->pos()<<" widget "<<w;
    if(0 < col && UIPAGE_COL_NUM >= col)
        colList[col-1].push_back(w);
    qDebug()<<"col "<<col<<" wid "<<w;
}

void UiPage::initTabOrder()
{
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
    case Qt::Key_Escape:
        if (0 < mparamAddrList.size()){
            QMessageBox msgBox(QMessageBox::Question, "参数设置","参数已被修改， 是否保存?");
            QAbstractButton* cancelBtn = dynamic_cast<QAbstractButton*>(msgBox.addButton("不保存", QMessageBox::NoRole));
            msgBox.addButton("保存", QMessageBox::YesRole);
            msgBox.exec();
            if (msgBox.clickedButton() == cancelBtn){
                qDebug()<<"don't save";
                //reback
                mparamAddrList.clear();
            }
        }
    case Qt::Key_Return:
        //save modified param address to global modified param address list
        mparamAddrList.clear();
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

void UiPage::resizeEvent(QResizeEvent *event)
{
    if (init) return ;

    QSize s0 = event->oldSize();
    QSize s1 = event->size();
    qDebug()<<__FUNCTION__<<this<<" old "<<s0<<" new "<<s1;
    int deltX = (s1.width() - initWidth)/(UIPAGE_COL_NUM+1);
    title->resize(s1.width(), titleHeight);
    if (s0.width() > 0){
        deltX = (s1.width() - s0.width())/(UIPAGE_COL_NUM+1);
    }
    qDebug()<<"deltX "<<deltX<<" initSize() "<<initWidth<<" "<<initHeight;
    if (s1.width() > initWidth && s1.height() > initHeight){
        for (int i = 0; i < UIPAGE_COL_NUM; i++) {
            for (auto it = colList[i].begin(); it != colList[i].end(); it++) {
                int x = (*it)->pos().x();
                int y = (*it)->pos().y();
                (*it)->move(x + deltX*(i+1), y);
                qDebug()<<*it<<" pos "<<(*it)->pos();
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

