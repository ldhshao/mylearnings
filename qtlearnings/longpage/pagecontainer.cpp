#include "pagecontainer.h"
#include "ui_pagecontainer.h"
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

    connect(&widButtonMgr, SIGNAL(sig_showWidget(QWidget*)), this, SLOT(slot_showWidget(QWidget*)));
    connect(this, SIGNAL(sig_selectButton(QWidget*)), &widButtonMgr, SLOT(slot_selectButton(QWidget*)));
    connect(pScrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(slot_vertScrollBarValueChanged(int)));

    bInited = true;
}

PageContainer::~PageContainer()
{
    delete ui;
    delete  pScrollArea;
    delete  pVLayout;
    clear();
}

void PageContainer::addWidgetLink(QWidget* w, QString strName)
{
    CWidgetButton* pBtn = widButtonMgr.addWidgetButton(w, strName);
    if (nullptr != pBtn){
        pVLayout->addWidget(pBtn);
    }
}
void PageContainer::setContent(QWidget *w)
{
    if (w != pContentWidget && nullptr != pContentWidget){
        clear();
    }

    pContentWidget = w;
    QLayout *layout = w->layout();
    for (int i = 0; i < layout->count(); i++) {
        QWidget *pSubWid = layout->itemAt(i)->widget();
        QGroupBox *pGroup = dynamic_cast<QGroupBox*>(pSubWid);
        if (nullptr != pGroup){
            addWidgetLink(pGroup, pGroup->title());
            subWidList.push_back(pSubWid);
        }
    }
    pScrollArea->setWidget(pContentWidget);
}
void PageContainer::clear()
{
    delete pContentWidget;
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
