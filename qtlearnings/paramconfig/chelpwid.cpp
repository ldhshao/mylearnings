#include "chelpwid.h"
#include "ui_chelpwid.h"
#include "cdeviceadrtblwid.h"
#include "Util/PageCfg.h"
#include <QResizeEvent>
#include <QKeyEvent>
#include <QDebug>

#define WIDGET_BKCOLOR        "border-image: url(:/images/mainback.png);"
#define TITLE_STYLE         "font-size:28px;color:rgba(255,255,255,100%);"
#define COPYRIGHT_STYLE     "color:rgba(255,255,255,100%);"
#define PROPERTY_INDEX "index"

CHelpWid::CHelpWid(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CHelpWid)
{
    ui->setupUi(this);
    bkLbl = new QLabel(this);
    bkLbl->setStyleSheet(WIDGET_BKCOLOR);
    menuWidth = 100;
    menuHeight = 40;
    ui->setupUi(this);
    devUiCfgList = nullptr;
    initMenu();
    connect(&menu2Mgr, SIGNAL(sig_button_clicked(CStateButton*)), this, SLOT(slot_menu2_clicked(CStateButton*)));
}

CHelpWid::~CHelpWid()
{
    delete ui;
}

void CHelpWid::showUi()
{
    for (auto it = menu2List.begin(); it != menu2List.end(); it++) {
        QWidget *w = (*it)->getWidget();
        if (nullptr != w){
            w->setEnabled(false);
            CDeviceAdrTblWid *pDevWid = dynamic_cast<CDeviceAdrTblWid*>(w);
            if (nullptr != pDevWid)
                pDevWid->initUi();
        }
    }
    menu2Mgr.selectButton(menu2List[0]);
    show();
}

void CHelpWid::initMenu()
{
    int i = 0;
    CDeviceAdrTblWid* widPreview = new CDeviceAdrTblWid(this);
    widPreview->hide();
    CKeyWidgetButton* pBtn = new CKeyWidgetButton(widPreview, this);
    pBtn->setText("点表信息");
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

void CHelpWid::slot_menu2_clicked(CStateButton* pBtn)
{
    int idx = pBtn->property(PROPERTY_INDEX).toInt();
    if (idx == menu2List.size() - 1){
        hide();
    }
}

void CHelpWid::onResize(int width, int height)
{
    int l = 120, t = 100, m = 4, s = 10;
    int i = 0;

    ui->label_title->setStyleSheet(TITLE_STYLE);
    ui->label_copyright->setStyleSheet(COPYRIGHT_STYLE);

    //layout background
    bkLbl->resize(width, height);
    bkLbl->move(0, 0);

    //layout title
    int tWidth = QFontMetrics(ui->label_title->font()).width(ui->label_title->text());
    int tHeight = QFontMetrics(ui->label_title->font()).height();
    ui->label_title->resize(tWidth, tHeight);
    ui->label_title->move((width - tWidth)/2, tHeight);

    //layout menu2
    int span = menuHeight + s;
    for (auto it = menu2List.begin(); it != menu2List.end(); it++) {
        (*it)->move(m, t + i * span);
        QWidget *w = (*it)->getWidget();
        if (nullptr != w){
            w->resize(width - l, height -50 - t);
            w->move(l, t);
        }
        i++;
    }

    //layout copyright
    ui->label_copyright->move((width - ui->label_copyright->width())/2, height - 50);

    update(0, 0, width, height);
}
void CHelpWid::resizeEvent(QResizeEvent *event)
{
    onResize(event->size().width(), event->size().height());
    QMainWindow::resizeEvent(event);
}

void CHelpWid::keyPressEvent(QKeyEvent *event)
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

bool CHelpWid::eventFilter(QObject * watched, QEvent * event)
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
