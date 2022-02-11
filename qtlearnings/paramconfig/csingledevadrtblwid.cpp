#include "csingledevadrtblwid.h"
#include "ui_csingledevadrtblwid.h"
#include "cdevposmgr.h"
#include <QResizeEvent>
#include <QDebug>

CSingleDevAdrTblWid::CSingleDevAdrTblWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CSingleDevAdrTblWid)
{
    ui->setupUi(this);
    pageSize = 34;
    pageIdx = 0;
}

CSingleDevAdrTblWid::~CSingleDevAdrTblWid()
{
    delete ui;
}

void CSingleDevAdrTblWid::setTitle(const QString &strTitle)
{
    ui->label_title->setText(strTitle);
    onResize(width(), height());
}

void CSingleDevAdrTblWid::setPortInfo(list<pair<uint32_t, QString>> *pList)
{
    portList = *pList;
    pageIdx = 0;
    updatePage();
}

bool CSingleDevAdrTblWid::pageAction(int action)
{
    bool ret = false;
    if(ACTION_INC <= action && action < ACTION_CNT){
        int pageCnt = (portList.size() + pageSize - 1)/ pageSize;
        if (ACTION_INC == action){
            if (pageIdx + 1 < pageCnt){
                pageIdx++;
                updatePage();
                return true;
            }
        }else {
            if (0 <= pageIdx -1){
                pageIdx--;
                updatePage();
                return true;
            }
        }
    }

    return ret;
}

void CSingleDevAdrTblWid::updatePage()
{
    QString strInfo;
    int pageCnt = (portList.size() + pageSize - 1)/ pageSize;
    int i = 0, cnt = 0, begin = pageIdx * pageSize;
    for (auto it = portList.begin(); it != portList.end(); it++,i++) {
        if (begin <= i){
            strInfo.append(CDevPosMgr::instance()->makeStrDevPoint(it->first) + ":" + it->second + "\n" );
            cnt++;
            if (cnt >= pageSize) break;
        }
    }
    ui->label_content->setText(strInfo);
    if (0 == pageCnt) pageCnt++;
    ui->label_pageinfo->setText(QString("第")+QString::number(pageIdx+1)+QString("页 共")+QString::number(pageCnt)+QString("页"));
    onResize(width(), height());
}

void CSingleDevAdrTblWid::onResize(int width, int height)
{
    int m = 30, s = 6;
    int t = m, tipHeight = 30;
    int i = 0;

    qDebug()<<"CSingleDevAdrTblWid onResize"<<width<<height;
    //layout title
    int tWidth = QFontMetrics(ui->label_title->font()).width(ui->label_title->text());
    int tHeight = QFontMetrics(ui->label_title->font()).height()+2;
    ui->label_title->resize(tWidth, tHeight);
    ui->label_title->move((width - tWidth)/2, t);
    t += (ui->label_title->height() + s);

    //layout page info
    tWidth = QFontMetrics(ui->label_pageinfo->font()).width(ui->label_pageinfo->text());
    ui->label_pageinfo->resize(tWidth, tipHeight);
    ui->label_pageinfo->move((width - tWidth)/2, height - tipHeight);

    //layout line
    ui->line->resize(width - 2*m, ui->line->height());
    ui->line->move(m, height - tipHeight - ui->line->height());

    //layout content
    ui->label_content->resize(width - 2*m, height - tipHeight - s - ui->line->height() - t);
    ui->label_content->move(m, t);
    //update(0, 0, width, height);
}

void CSingleDevAdrTblWid::resizeEvent(QResizeEvent *event)
{
    onResize(event->size().width(), event->size().height());
    //QWidget::resizeEvent(event);
}

