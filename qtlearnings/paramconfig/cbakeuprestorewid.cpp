#include "cbakeuprestorewid.h"
#include "ui_cbakeuprestorewid.h"
#include "cparamtablemngrwid.h"
#include "Util/PageCfg.h"
#include <QResizeEvent>
#include <QKeyEvent>
#include <QDebug>

#define WIDGET_BKCOLOR        "border-image: url(:/images/mainback.png);"
#define TITLE_STYLE         "font-size:28px;color:rgba(255,255,255,100%);"
#define COPYRIGHT_STYLE     "color:rgba(255,255,255,100%);"
#define PROPERTY_INDEX "index"
CBakeupRestoreWid::CBakeupRestoreWid(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CBakeupRestoreWid)
{
    bkLbl = new QLabel(this);
    bkLbl->setStyleSheet(WIDGET_BKCOLOR);
    ui->setupUi(this);
    ui->label_title->setStyleSheet(TITLE_STYLE);
    ui->label_copyright->setStyleSheet(COPYRIGHT_STYLE);
    menuWidth = 100;
    menuHeight = 40;
    devUiCfgList = nullptr;
    paramSrvAddr = nullptr;
    paramLclAddr = nullptr;
    initMenu();
    connect(&menu2Mgr, SIGNAL(sig_button_clicked(CStateButton*)), this, SLOT(slot_menu2_clicked(CStateButton*)));
}

CBakeupRestoreWid::~CBakeupRestoreWid()
{
    delete ui;
}

void CBakeupRestoreWid::showUi()
{
    for (auto it = menu2List.begin(); it != menu2List.end(); it++) {
        QWidget *w = (*it)->getWidget();
        if (nullptr != w) w->setEnabled(false);
        CParamTableMngrWid* preview = dynamic_cast<CParamTableMngrWid*>(w);
        if (nullptr != preview)
            preview->showUi();
    }

    menu2Mgr.selectButton(menu2List[0]);
    show();
}

void CBakeupRestoreWid::setParamAddr(uint16_t* srvAddr, uint16_t* lclAddr)
{
    paramSrvAddr = srvAddr; paramLclAddr = lclAddr;
    for (auto it = menu2List.begin(); it != menu2List.end(); it++) {
        CParamTableMngrWid* preview = dynamic_cast<CParamTableMngrWid*>((*it)->getWidget());
        if (nullptr != preview){
            preview->setParamAddr(srvAddr, lclAddr);
            break;
        }
    }
}

void CBakeupRestoreWid::initMenu()
{
    int i = 0;
    CParamTableMngrWid* widPtMngr = new CParamTableMngrWid(this);
    widPtMngr->hide();
    CKeyWidgetButton* pBtn = new CKeyWidgetButton(widPtMngr, this);
    pBtn->setText("参数表管理");
    pBtn->setProperty(PROPERTY_INDEX, i++);
    pBtn->resize(menuWidth, menuHeight);
    pBtn->installEventFilter(this);
    menu2Mgr.registerButton(pBtn);
    menu2List.push_back(pBtn);

    pBtn = new CKeyWidgetButton(nullptr, this);
    pBtn->setText("返回");
    pBtn->setProperty(PROPERTY_INDEX, i++);
    pBtn->resize(menuWidth, menuHeight);
    pBtn->installEventFilter(this);
    menu2Mgr.registerButton(pBtn);
    menu2List.push_back(pBtn);
}

void CBakeupRestoreWid::slot_menu2_clicked(CStateButton* pBtn)
{
    int idx = pBtn->property(PROPERTY_INDEX).toInt();
    if (idx == menu2List.size() - 1){
        hide();
    }
}

void CBakeupRestoreWid::onResize(int width, int height)
{
    int l = 120, t = 100, m = 4, s = 10;
    int i = 0;

    //layout background
    bkLbl->resize(width, height);
    bkLbl->move(0, 0);

    //layout title
    int tWidth = QFontMetrics(ui->label_title->font()).width(ui->label_title->text());
    int tHeight = QFontMetrics(ui->label_title->font()).height();
    ui->label_title->resize(tWidth, tHeight);
    ui->label_title->move((width - tWidth)/2, 20);

    //layout menu2
    int span = menuHeight + s;
    for (auto it = menu2List.begin(); it != menu2List.end(); it++) {
        (*it)->move(m, t + i * span);
        QWidget *w = (*it)->getWidget();
        if (nullptr != w){
            w->resize(width - l, height -50 - t);
            //w->move(mapToGlobal(QPoint(l, t)));
            w->move(l, t);
        }
        i++;
    }
    qDebug()<<__FUNCTION__<<" idx "<<i;

    //layout copyright
    ui->label_copyright->move((width - ui->label_copyright->width())/2, height - 50);
    update(0, 0, width, height);
}
void CBakeupRestoreWid::resizeEvent(QResizeEvent *event)
{
    onResize(event->size().width(), event->size().height());
    QMainWindow::resizeEvent(event);
}

void CBakeupRestoreWid::keyPressEvent(QKeyEvent *event)
{
    CKeyWidgetButton* pBtn = dynamic_cast<CKeyWidgetButton*>(menu2Mgr.currentButton());
    int idx = pBtn->property(PROPERTY_INDEX).toInt();
    switch (event->key()) {
    case Qt::Key_Return:
        {
            QWidget *w = pBtn->getWidget();
            w->setEnabled(true);
            w->setFocus();
        }
        break;
    case Qt::Key_Up:
        idx = (idx - 1 + menu2List.size()) % menu2List.size();
        menu2Mgr.selectButton(menu2List[idx]);
        break;
    case Qt::Key_Down:
        idx = (idx + 1) % menu2List.size();
        menu2Mgr.selectButton(menu2List[idx]);
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }
}

bool CBakeupRestoreWid::eventFilter(QObject * watched, QEvent * event)
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
        return QMainWindow::eventFilter(watched, event);
    }
#else
    return QMainWindow::eventFilter(watched, event);
#endif
}
