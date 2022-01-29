#include "clineselector.h"
#include "cdevposmgr.h"
#include "cdevpointedit.h"
#include "ckeycheckbox.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QCheckBox>
#include <QTimer>
#include <QDebug>
#include <QKeyEvent>
//#include <QMouseEvent>

#define PROPERTY_INDEX "index"
CButtonSelector::CButtonSelector(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    btnVisibleCnt = 0;
    usrKey = 0;
    timerInterval = 300;
    emitTimer = new QTimer(this);
    connect(emitTimer, SIGNAL(timeout()), this, SLOT(slot_emitTimer()));
}

void CButtonSelector::selectButtonByIndex(int idx)
{
    if (idx < btnList.size()){
        btnMgr.selectButton(btnList[idx]);
    }else {
        btnMgr.selectButton(nullptr);
    }
}

void CButtonSelector::adjustPosition(int x, int y, int w, int h)
{
    QRect deskRect = QApplication::desktop()->availableGeometry();
    qDebug()<<"x "<<x<<" y "<<y<<" w "<<w<<" h "<<h;
    int sw = deskRect.width(), sh = deskRect.height();
    qDebug()<<"sw "<<sw<<" sh "<<sh;
    if (y + h + height() > sh){//show up
        if (x + width() <= sw){//align with left
            move(x, y-height());
        }else if (width() < x + w){//align with right
            move(x + w - width(), y-height());
        }else{//align with center
            move((sw - width())/2, y-height());
        }
    }else {
        if (x + width() <= sw){//align with left
            move(x, y+h);
        }else if (width() < x + w){//align with right
            move(x + w - width(), y+h);
        }else{//align with center
            move((sw - width())/2, y+h);
        }
    }
    qDebug()<<"new pos "<<pos();
}

void CButtonSelector::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        if (nullptr != btnMgr.currentButton()){
            int idx = btnMgr.currentButton()->property(PROPERTY_INDEX).toInt();
            for (int i = 1; i < btnList.size(); i++){
                int k = (idx - i + btnList.size()) % btnList.size();
                if (btnList[k]->isVisible()){
                    btnMgr.selectButton(btnList[k]);
                    break;
                }
            }
        }else{
            selectButtonByIndex(0);
        }
        qDebug()<<"keyPressEvent KEY_LEFT";
        break;
    case Qt::Key_Right:
        if (nullptr != btnMgr.currentButton()){
            int idx = btnMgr.currentButton()->property(PROPERTY_INDEX).toInt();
            for (int i = 1; i < btnList.size(); i++){
                int k = (idx + i) % btnList.size();
                if (btnList[k]->isVisible()){
                    btnMgr.selectButton(btnList[k]);
                    break;
                }
            }
        }else{
            selectButtonByIndex(0);
        }
        qDebug()<<"keyPressEvent KEY_RIGHT";
        break;
    case Qt::Key_Up:
        prevSelector();
        qDebug()<<"keyPressEvent KEY_UP";
        break;
    case Qt::Key_Down:
    case Qt::Key_Return:
        nextSelector();
        qDebug()<<"keyPressEvent KEY_DOWN";
        break;
    case Qt::Key_Escape:
        prevSelector();
        break;
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
        if(emitTimer->isActive()) emitTimer->stop();
        usrKey = usrKey * 10 + (event->key() - Qt::Key_0);
        emitTimer->start(timerInterval);
        break;
    default:
        break;
    }
}

void CButtonSelector::hideSelector()
{
    hide();
    selectButtonByIndex(-1);
}

void CButtonSelector::slot_emitTimer()
{
    emitTimer->stop();
    qDebug()<<"slot_emitTimer "<<usrKey;
    usrKey = usrKey - 1;
    if (usrKey < btnList.size() && btnList[usrKey]->isVisible()){
        btnMgr.selectButton(btnList[usrKey]);
    }
    usrKey = 0;
}

CLineSelector::CLineSelector(QWidget *parent) : CButtonSelector(parent)
{
    int w = 60, h = 40;
    QStringList lineNames = CDevPosMgr::instance()->getLineNames();
    for (int i = 0; i < lineNames.count(); i++) {
        CStateButton* btn = new CKeyStateButton(this);
        btn->resize(w,h);
        btn->setText(lineNames[i] + "\n" + QString::number(i+1));
        btn->setProperty(PROPERTY_INDEX, i);
        btn->move(i*w, 0);
        btnMgr.registerButton(btn);
        btnList.push_back(btn);
    }
    btnVisibleCnt = lineNames.count();
    resize(lineNames.count()*w, h);
    connect(&btnMgr, SIGNAL(sig_button_clicked(CStateButton*)), this, SLOT(slot_btnClicked(CStateButton*)));
}

CLineSelector* CLineSelector::instance()
{
    static CLineSelector _ins;
    return &_ins;
}

void CLineSelector::slot_btnClicked(CStateButton* pBtn)
{
    qDebug()<<"slot_btnChicked "<<pBtn<<" pos "<<pBtn->pos();
    int idx = pBtn->property(PROPERTY_INDEX).toInt();
    QPoint pt = mapToGlobal(pBtn->pos());
    qDebug()<<"pt "<<pt;
    int w = pBtn->width(), h = pBtn->height();
    CMachineSelector::instance()->showMachines(idx);
    CMachineSelector::instance()->adjustPosition(pt.rx(), pt.ry(), w, h);
    activateWindow();
}

void CLineSelector::nextSelector()
{
    if (nullptr != btnMgr.currentButton())
        CMachineSelector::instance()->activateWindow();
}
void CLineSelector::prevSelector()
{
    CPortSelector::instance()->exitSelector();
}

CMachineSelector::CMachineSelector(QWidget *parent) : CButtonSelector(parent),currLine(-1)
{
    int w = 60, h = 40, row = 0, lineBtnCnt = 20;
    for (int i = 0; i < MACHINE_MAX; i++) {
        CStateButton* btn = new CKeyStateButton(this);
        btn->resize(w,h);
        btn->setText(QString::number(i+1) + "\n" + QString::number(i+1));
        btn->setProperty(PROPERTY_INDEX, i);
        btn->move((i % lineBtnCnt)*w, row*h);
        btnMgr.registerButton(btn);
        btnList.push_back(btn);
        if (lineBtnCnt -1 == (i % lineBtnCnt)) row++;
    }
    connect(&btnMgr, SIGNAL(sig_button_clicked(CStateButton*)), this, SLOT(slot_btnClicked(CStateButton*)));
}

CMachineSelector* CMachineSelector::instance()
{
    static CMachineSelector _ins;
    return &_ins;
}

void CMachineSelector::showMachines(int line)
{
    int w = 60, h = 40, lineBtnCnt = 20;
    //int cnt = CDevPosMgr::instance()->getMachineCount(line);
    int i = 0, r = 0;
    for (; i < MACHINE_MAX; i++) {
        btnList[i]->setVisible(false);
    }
    //for (; i < cnt; i++) {
    //    btnList[i]->setVisible(true);
    //}
    list<int> mList = CDevPosMgr::instance()->getMachines(line);
    i = 0;
    for (auto it = mList.begin(); it != mList.end(); it++) {
        btnList[*it]->setVisible(true);
        btnList[*it]->move((i%lineBtnCnt)*w, r*h);
        if (0 == (i+1)%lineBtnCnt) r++;
        i++;
    }
    btnVisibleCnt = mList.size();
    if (btnVisibleCnt <= lineBtnCnt)
        resize(btnVisibleCnt*w, h);
    else {
        resize(lineBtnCnt*w, (btnVisibleCnt + lineBtnCnt - 1)/lineBtnCnt * h);
    }
    currLine = line;
    btnMgr.selectButton(nullptr);
    qDebug()<<"showMachines";
    show();
}

void CMachineSelector::slot_btnClicked(CStateButton* pBtn)
{
    int m = btnMgr.currentButton()->property(PROPERTY_INDEX).toInt();
    QPoint pt = mapToGlobal(pBtn->pos());
    int w = pBtn->width(), h = pBtn->height();
    CPortSelector::instance()->showPorts(currLine, m);
    CPortSelector::instance()->adjustPosition(pt.rx(), pt.ry(), w, h);
    activateWindow();
}

void CMachineSelector::nextSelector()
{
    if (nullptr != btnMgr.currentButton())
        CPortSelector::instance()->activateWindow();
}
void CMachineSelector::prevSelector()
{
    hideSelector();
    CPortSelector::instance()->hide();
    CLineSelector::instance()->activateWindow();
}
//////CPortSelector/////////////////////////
CPortSelector::CPortSelector(QWidget *parent) : QWidget(parent),currLine(-1),currMachine(-1)
{
    setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    pEdit = nullptr;
    usrKey = 0;
    timerInterval = 300;
    emitTimer = new QTimer(this);
    connect(emitTimer, SIGNAL(timeout()), this, SLOT(slot_emitTimer()));

    int w = 60, h = 40, m = 6;
    for (int i = 0; i < PORT_MAX; i++) {
        QCheckBox* btn = new CKeyCheckBox(this);
        btn->resize(w,h);
        btn->setText(QString::number(i+1) + "\n" + QString::number(i+1));
        btn->setProperty(PROPERTY_INDEX, i);
        btn->move(m+i*w, 0);
        connect(btn, SIGNAL(stateChanged(int)), this, SLOT(on_checkbox_stateChanged(int)));
        btnList.push_back(btn);
    }
}

CPortSelector* CPortSelector::instance()
{
    static CPortSelector _ins;
    return &_ins;
}

void CPortSelector::showPorts(int line, int machine)
{
    int w = 60, h = 40, m=6;
    list<bool> portList = CDevPosMgr::instance()->getMachinePorts(line, machine, portType);
    int cnt = portList.size();
    int i = 0;
    uint32_t devPt = 0;
    bool devPtValid = false;
    if (nullptr != pEdit){
        devPt = pEdit->getValue();
        if (CDevPosMgr::instance()->isDevPointValid(devPt, portType)){
            devPtValid = true;
        }
    }
    chkLoading = true;
    if (!devPtValid){
        for (auto it = portList.begin(); it != portList.end(); it++,i++) {
            btnList[i]->setVisible(true);
            btnList[i]->setEnabled(!(*it));
            btnList[i]->setChecked(*it);
        }
    }else {
        for (auto it = portList.begin(); it != portList.end(); it++,i++) {
            btnList[i]->setVisible(true);
            btnList[i]->setChecked(*it);
            btnList[i]->setEnabled(false);
        }
        int l = get_line_from_dev_point(devPt) - 1;
        int m = get_machine_from_dev_point(devPt);
        if (line == l && machine == m){
            int p = get_port_from_dev_point(devPt);
            btnList[p]->setEnabled(true);
        }
    }
    for (; i < PORT_MAX; i++) {
        btnList[i]->setVisible(false);
    }
    resize(m+cnt*w, h);
    currLine = line;
    currMachine = machine;
    chkLoading = false;
    btnList[0]->setFocus();
    show();
}

void CPortSelector::adjustPosition(int x, int y, int w, int h)
{
    QRect deskRect = QApplication::desktop()->availableGeometry();
    qDebug()<<"x "<<x<<" y "<<y<<" w "<<w<<" h "<<h;
    int sw = deskRect.width(), sh = deskRect.height();
    if (y + h + height() > sh){//show up
        if (x + width() <= sw){//align with left
            move(x, y-height());
        }else if (width() < x + w){//align with right
            move(x + w - width(), y-height());
        }else{//align with center
            move(0, y-height());
        }
    }else {
        if (x + width() <= sw){//align with left
            move(x, y+h);
        }else if (width() < x + w){//align with right
            move(x + w - width(), y+h);
        }else{//align with center
            move(0, y+h);
        }
    }
    qDebug()<<"new pos "<<pos();
}

void CPortSelector::exitSelector()
{
    hide();
    CMachineSelector::instance()->hideSelector();
    CLineSelector::instance()->hideSelector();
    if (nullptr != pEdit){
        pEdit->endEdit();
    }
}

void CPortSelector::on_checkbox_stateChanged(int newState)
{
    if (!chkLoading){
        QCheckBox* chk = dynamic_cast<QCheckBox*>(sender());
        int p = chk->property(PROPERTY_INDEX).toInt();
        if (Qt::Checked == newState){
            if (nullptr != pEdit){
                uint32_t devPt = make_dev_point(currLine+1, currMachine, p);
                pEdit->setValue(devPt);
            }
            exitSelector();
        }else {
            if (nullptr != pEdit){
                pEdit->setValue(0);
            }
            for (int i = 0; i < PORT_MAX; i++) {
                if (!btnList[i]->isVisible())
                    break;
                if (!btnList[i]->isChecked()){
                    btnList[i]->setEnabled(true);
                }
            }
        }
        qDebug()<<"sender "<<chk<<"state "<<newState;
    }
}

void CPortSelector::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        hide();
        break;
    case Qt::Key_Escape:
        exitSelector();
        break;
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
        if(emitTimer->isActive()) emitTimer->stop();
        usrKey = usrKey * 10 + (event->key() - Qt::Key_0);
        emitTimer->start(timerInterval);
        break;
    default:
        QWidget::keyPressEvent(event);
        break;
    }
}

void CPortSelector::slot_emitTimer()
{
    emitTimer->stop();
    qDebug()<<"CPortSelector::slot_emitTimer "<<usrKey;
    usrKey = usrKey - 1;
    if (usrKey < btnList.size() && btnList[usrKey]->isVisible()){
        btnList[usrKey]->setFocus();//效果不明显
    }
    usrKey = 0;
}
