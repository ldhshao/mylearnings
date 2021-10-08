#include "pagecontainer.h"
#include "ui_pagecontainer.h"
#include "uipage.h"
#include <QLayout>
#include <QLayoutItem>
#include <QGroupBox>
#include <qevent.h>
#include <QScrollBar>
#include <QDebug>

PageContainer::PageContainer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageContainer), pContentWidget(nullptr), bInited(false)
{
    ui->setupUi(this);
    iTagWidWidth = 100;
    iTagWidHeigth = height();
    pScrollArea = new QScrollArea (this);
    ui->tagWidget->resize(iTagWidWidth,iTagWidHeigth);
    pVLayout = new QVBoxLayout(ui->tagWidget);
    pScrollArea->verticalScrollBar()->hide();

    connect(&widButtonMgr, SIGNAL(sig_showWidget(QWidget*)), this, SLOT(slot_showWidget(QWidget*)));
    connect(this, SIGNAL(sig_selectButton(QWidget*)), &widButtonMgr, SLOT(slot_selectButton(QWidget*)));
    connect(pScrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(slot_vertScrollBarValueChanged(int)));

    bInited = true;
}

PageContainer::~PageContainer()
{
    clear();
    delete ui;
    delete  pScrollArea;
    delete  pVLayout;
}

void PageContainer::addWidgetLink(QWidget* w, QString strName)
{
    CWidgetButton* pBtn = widButtonMgr.addWidgetButton(w, strName);
    if (nullptr != pBtn){
        pVLayout->addWidget(pBtn);
    }
}
void PageContainer::setContent(UiPage *page)
{
    clear();

    pContentWidget = page;
    QWidget *w = page->getHead();
    while (nullptr != w){
        QGroupBox *pGroup = dynamic_cast<QGroupBox*>(w);
        if (nullptr != pGroup){
            addWidgetLink(pGroup, pGroup->title());
            subWidList.push_back(w);
        }
        w = page->getNext();
    }
    pScrollArea->setWidget(pContentWidget);
}
void PageContainer::clear()
{
    if (nullptr != pContentWidget){
        pScrollArea->takeWidget();
    }

    pContentWidget = nullptr;
    subWidList.clear();
    widButtonMgr.clear();
}

void PageContainer::resizeEvent(QResizeEvent *event)
{
    if (bInited){
        int w = event->size().width();
        int h = event->size().height();
        pScrollArea->resize(w - iTagWidWidth, h);
        ui->tagWidget->move(w - iTagWidWidth, 0);
        QWidget::resizeEvent(event);
    }
}

void PageContainer::slot_showWidget(QWidget* pSubWid)
{
    int y = pSubWid->pos().ry();
    int max = pScrollArea->verticalScrollBar()->maximum();
    if (y > max)
    {
        y = max;
    }

    pScrollArea->verticalScrollBar()->setValue(y);
    //qDebug()<<"subWidget pos " <<pSubWid->pos()<<" width "<<pSubWid->width()<<" height "<<pSubWid->height();
}

void PageContainer::slot_vertScrollBarValueChanged(int val)
{
    if (!subWidList.empty()){
        for (list<QWidget*>::const_iterator it = subWidList.cbegin(); it != subWidList.end(); it++){
            if ((*it)->pos().ry() >= val){
                emit sig_selectButton(*it);
                break;
            }
        }
    }
}
