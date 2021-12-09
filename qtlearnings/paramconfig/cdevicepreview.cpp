#include "cdevicepreview.h"
#include "ui_cdevicepreview.h"
#include "Util/PageCfg.h"
#include "UiCommon/uipage.h"
#include <QDebug>

CDevicePreview::CDevicePreview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CDevicePreview), pageCfg(nullptr)
{
    ui->setupUi(this);
}

CDevicePreview::~CDevicePreview()
{
    delete ui;
}

void CDevicePreview::updateByDevice(GroupCfgItem* dev)
{
    if (nullptr != dev){
        ui->label_title->setText(dev->getName());
        ui->label_content->setText(dev->previewInfo());
        devUiCfg = dev;
    }
}

void CDevicePreview::showConfigPage(GroupCfgItem *dev)
{
    clearConfigPage();

    if (nullptr != dev){
        UiPage* page = dynamic_cast<UiPage*>(dev->getWidget());
        connect(page, SIGNAL(sig_configFinished), this, SLOT());
        page->resize(width(), height());
        page->move(mapToGlobal(QPoint(0, 0)));
        page->show();
        pageCfg = page;
    }
}
void CDevicePreview::clearConfigPage()
{
    if (nullptr != pageCfg){
        pageCfg->hide();
        pageCfg = nullptr;
    }
}

void CDevicePreview::keyReleaseEvent(QKeyEvent *event)
{
    qDebug()<<"preview "<<__FUNCTION__;
}
void CDevicePreview::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug()<<"preview "<<__FUNCTION__;
    showConfigPage(devUiCfg);
}

void CDevicePreview::slot_configFinished()
{
    qDebug()<<"preview "<<__FUNCTION__;
    updateByDevice(devUiCfg);
    pageCfg = nullptr;
}
