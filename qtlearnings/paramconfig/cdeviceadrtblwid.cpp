#include "cdeviceadrtblwid.h"
#include "ui_cdeviceadrtblwid.h"
#include "Util/PageCfg.h"
#include "cdevposmgr.h"
#include <QCoreApplication>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QDebug>

#define WIDGET_BKCOLOR         "background-color:rgba(255,255,255,100%);"
#define PROPERTY_DEVICE "device"
CDeviceAdrTblWid::CDeviceAdrTblWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CDeviceAdrTblWid),pDevUiCfgList(nullptr), currWid(nullptr),lineIdx(0),currPrefix("> ")
{
    setWindowFlags(Qt::FramelessWindowHint);
    bkLbl = new QLabel(this);
    bkLbl->setStyleSheet(WIDGET_BKCOLOR);
    ui->setupUi(this);
    pInputWid = new CSingleDevAdrTblWid(this);
    pOutputWid = new CSingleDevAdrTblWid(this);
    pDevUiCfgList = static_cast<PageCfgList*>(QCoreApplication::instance()->property(PROPERTY_DEVICE).value<void*>());
    QString strTip("按下←, →按键切换显示, 按下K键切换输入/输出点列表, 按下↑, ↓键翻页");
    int width = QFontMetrics(font()).width(strTip);
    ui->label_tip->resize(width, ui->label_tip->height());
    ui->label_tip->setText(strTip);
}

CDeviceAdrTblWid::~CDeviceAdrTblWid()
{
    delete ui;
}

void CDeviceAdrTblWid::initUi()
{
    currWid = pInputWid;
    lineIdx = 0;
    updateWidData();
    updateWidTitle();
    //for (auto it = inputList.begin(); it != inputList.end(); it++) {
    //    qDebug()<<CDevPosMgr::instance()->makeStrDevPoint(it->first)<<it->second;
    //}
}

void CDeviceAdrTblWid::updateWidData()
{
    list<pair<uint32_t,QString>> inputList, outputList;
    pDevUiCfgList->findPortInfoByPortType(lineIdx, CDevPosMgr::PORTTYPE_IN, &inputList);
    pDevUiCfgList->findPortInfoByPortType(lineIdx, CDevPosMgr::PORTTYPE_OUT, &outputList);
    pInputWid->setPortInfo(&inputList);
    pOutputWid->setPortInfo(&outputList);
}

void CDeviceAdrTblWid::updateWidTitle()
{
    QStringList lineNames = CDevPosMgr::instance()->getLineNames();
    if (lineIdx < lineNames.count()){
        QString lineName = lineNames[lineIdx];
        if (pInputWid == currWid){
            pInputWid->setTitle(currPrefix + lineName + "输入点使用情况");
            pOutputWid->setTitle(lineName + "输出点使用情况");
        }else if (pOutputWid == currWid){
            pInputWid->setTitle(lineName + "输入点使用情况");
            pOutputWid->setTitle(currPrefix + lineName + "输出点使用情况");
        }
    }
}

void CDeviceAdrTblWid::onResize(int width, int height)
{
    int m = 6, s = 4;
    int tipHeight = 30;

    //layout background
    bkLbl->resize(width, height);
    bkLbl->move(0, 0);

    int childWidth = (width - 2*m - s)/2;
    int childHeight = height - tipHeight;
    //layout input widget
    pInputWid->resize(childWidth, childHeight);
    pInputWid->move(m, 0);
    pOutputWid->resize(childWidth,childHeight);
    pOutputWid->move(width - m - childWidth, 0);

    //layout tip info
    ui->label_tip->move(m+30, height - tipHeight);
    update(0, 0, width, height);
}
void CDeviceAdrTblWid::resizeEvent(QResizeEvent *event)
{
    onResize(event->size().width(), event->size().height());
    //QWidget::resizeEvent(event);
}

void CDeviceAdrTblWid::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        {
            QStringList lineNames = CDevPosMgr::instance()->getLineNames();
            lineIdx = (lineIdx - 1 + lineNames.count()) % lineNames.count();
            updateWidData();
            updateWidTitle();
        }
        break;
    case Qt::Key_Right:
        {
            QStringList lineNames = CDevPosMgr::instance()->getLineNames();
            lineIdx = (lineIdx + 1) % lineNames.count();
            updateWidData();
            updateWidTitle();
        }
        break;
    case Qt::Key_K:
        if (currWid == pInputWid) {
            currWid = pOutputWid;
            updateWidTitle();
        }else if (currWid == pOutputWid) {
            currWid = pInputWid;
            updateWidTitle();
        }
        break;
    case Qt::Key_Up:
        if (nullptr != currWid) {
            CSingleDevAdrTblWid* pWid = dynamic_cast<CSingleDevAdrTblWid*>(currWid);
            pWid->pageAction(CSingleDevAdrTblWid::ACTION_DES);
        }
        break;
    case Qt::Key_Down:
        if (nullptr != currWid) {
            CSingleDevAdrTblWid* pWid = dynamic_cast<CSingleDevAdrTblWid*>(currWid);
            pWid->pageAction(CSingleDevAdrTblWid::ACTION_INC);
        }
        break;
    case Qt::Key_Escape:
        setEnabled(false);
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

bool CDeviceAdrTblWid::eventFilter(QObject * watched, QEvent * event)
{
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
    if (nullptr != dynamic_cast<QWidget*>(watched)) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            keyPressEvent(keyEvent);
            return true;
        } else {
            return false;
        }
    } else {
        return QWidget::eventFilter(watched, event);
    }
#else
    return QWidget::eventFilter(watched, event);
#endif
}
