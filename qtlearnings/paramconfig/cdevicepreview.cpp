#include "cdevicepreview.h"
#include "ui_cdevicepreview.h"
#include "Util/PageCfg.h"
#include "UiCommon/uipage.h"
#include <QMessageBox>
#include <QPushButton>
#include <QAbstractButton>
#include <QResizeEvent>
#include <QDebug>

#define WIDGET_STYLE     "background-color:rgba(80,80,100,100%);"
#define TITLE_STYLE      "font-size:20px;color:rgba(255,255,255,100%);"
#define CONTENT_STYLE    "color:rgba(255,255,255,100%);"

CDevicePreview::CDevicePreview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CDevicePreview), pageCfg(nullptr)
{
    ui->setupUi(this);
    setStyleSheet(WIDGET_STYLE);
    ui->label_title->setStyleSheet(TITLE_STYLE);
    ui->label_content1->setStyleSheet(CONTENT_STYLE);
    ui->label_content2->setStyleSheet(CONTENT_STYLE);
}

CDevicePreview::~CDevicePreview()
{
    delete ui;
}

void CDevicePreview::updateByDevice(GroupCfgItem* dev)
{
    qDebug()<<"preview "<<__FUNCTION__;
    if (nullptr != dev){
        ui->label_title->setText(dev->getNamePath(dev->titleDepth() - 1));
        ui->label_content1->setText(dev->previewInfo(0));
        ui->label_content2->setText(dev->previewInfo(1));
        devUiCfg = dev;
        qDebug()<<"preview update";
        onResize(width(), height());
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
        page->showPage();
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

void CDevicePreview::onResize(int width, int height)
{
    int t = 60, m = 10, s = 10;

    //layout title
    int tWidth = QFontMetrics(ui->label_title->font()).width(ui->label_title->text());
    int tHeight = QFontMetrics(ui->label_title->font()).height();
    ui->label_title->resize(tWidth, tHeight);
    ui->label_title->move((width - tWidth)/2, 10);

    //layout content
    int lblW = (width - 2 * m - s) /2;
    int lblH = height - t - m;
    if (ui->label_content2->text().isEmpty()){
        ui->label_content1->resize(lblW, lblH);
        ui->label_content2->resize(0, 0);
        ui->label_content1->move(m + lblW/2, t);
    }else {
        ui->label_content1->resize(lblW, lblH);
        ui->label_content2->resize(lblW, lblH);
        ui->label_content1->move(m, t);
        ui->label_content2->move(m + lblW + s, t);
    }
    setAutoFillBackground(true);
}
void CDevicePreview::resizeEvent(QResizeEvent *event)
{
    onResize(event->size().width(), event->size().height());
    //QWidget::resizeEvent(event);
}
