#include "cdevicepreview.h"
#include "ui_cdevicepreview.h"
#include "Util/PageCfg.h"
#include "UiCommon/uipage.h"
#include <QMessageBox>
#include <QPushButton>
#include <QAbstractButton>
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
        ui->label_title->setText(dev->getNamePath(dev->titleDepth() - 1));
        ui->label_content1->setText(dev->previewInfo(1));
        ui->label_content2->setText(dev->previewInfo(2));
        devUiCfg = dev;
    }
}

void CDevicePreview::showConfigPage(GroupCfgItem *dev)
{
    clearConfigPage();

    if (nullptr != dev){
        UiPage* page = dynamic_cast<UiPage*>(dev->getWidget());
        if (!page->isEnabled()){
            QMessageBox msgBox(QMessageBox::Information, dev->getNamePath(dev->titleDepth() - 1).append("不可编辑"),
                               dev->enableReason());
            QAbstractButton* okBtn = dynamic_cast<QAbstractButton*>(msgBox.addButton("确定", QMessageBox::AcceptRole));
            msgBox.exec();
            return ;
        }
        page->resize(width(), height());
        page->move(mapToGlobal(QPoint(0, 0)));
        qDebug()<<"width "<<width()<<" height "<<height()<<" pagepos "<<mapToGlobal(QPoint(0,0));
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
