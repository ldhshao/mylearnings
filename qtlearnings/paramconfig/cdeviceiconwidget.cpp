#include "cdeviceiconwidget.h"
#include "ui_cdeviceiconwidget.h"
#include <QResizeEvent>
#include <QKeyEvent>
#include <QDebug>

#define PROPERTY_IMAGE "image"
#define TITLE_STYLE_LITTLE           "font-size:20px;color:rgba(255,255,255,100%);"
#define TITLE_STYLE_NORMAL           "font-size:18px;color:rgba(255,255,255,100%);"
#define SELECT_STYLE           "background-color:rgba(200,60,60,100%);"
#define NORMAL_STYLE           ""
CDeviceIconWidget::CDeviceIconWidget(QWidget *parent, uint8_t m) :
    QWidget(parent),
    ui(new Ui::CDeviceIconWidget), mode(m)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
    ui->label_title->setAlignment(Qt::AlignCenter);
    if (0 == mode) ui->label_title->setStyleSheet(TITLE_STYLE_LITTLE);
    if (1 == mode) ui->label_title->setStyleSheet(TITLE_STYLE_NORMAL);
    setStyleSheet(NORMAL_STYLE);
}

CDeviceIconWidget::~CDeviceIconWidget()
{
    delete ui;
}

void CDeviceIconWidget::setTitle(QString strTitle)
{
    ui->label_title->setText(strTitle);
}

void CDeviceIconWidget::setImagePath(QString strImgPath)
{
    setProperty(PROPERTY_IMAGE, strImgPath);
}

void CDeviceIconWidget::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<"CDeviceIconWidget "<<__FUNCTION__;
    switch (event->key()) {
        case Qt::Key_Return:
            emit clicked(this);
            return ;
        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Left:
        case Qt::Key_Right:
        case Qt::Key_Escape:
            event->setAccepted(false);
            return ;
    }
    QWidget::keyPressEvent(event);
}
void CDeviceIconWidget::resizeEvent(QResizeEvent *event)
{
    int w = event->size().width(), h = event->size().height(), hm = 20;
    int tH = 40;
    int iW = w - 2 * hm, iH = h - tH;
    QString strImg = property(PROPERTY_IMAGE).toString();
    if (!strImg.isEmpty()){
        QPixmap pixmap(strImg);
        ui->label_img->setPixmap(pixmap.scaled(iW, iH));
    }
    ui->label_img->resize(iW, iH);
    ui->label_img->move(hm, 0);
    ui->label_title->resize(iW, tH);
    ui->label_title->move(hm, h - tH);
}

void CDeviceIconWidget::focusInEvent(QFocusEvent *event)
{
    setStyleSheet(SELECT_STYLE);
}
void CDeviceIconWidget::focusOutEvent(QFocusEvent *event)
{
    setStyleSheet(NORMAL_STYLE);
}
