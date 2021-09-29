#include "splitertest.h"
#include "ui_splitertest.h"
#include <QSplitter>
#include <QScrollArea>
#include <QTextEdit>
#include "pageposuiji.h"

SpliterTest::SpliterTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpliterTest)
{
    ui->setupUi(this);

    int w = width(), h = height();
    //QSplitter *splitterMain=new QSplitter(Qt::Horizontal,this);
    //QScrollArea *scrollArea1 = new QScrollArea (splitterMain);
    //PagePosuiji *page = new PagePosuiji(splitterMain);
    //scrollArea1->setWidget(page);
    //QTextEdit *textLeft=new QTextEdit(QObject::tr("Left Widget"),splitterMain);
    //textLeft->setAlignment(Qt::AlignCenter);
    ////textLeft->resize(w*0.7, h);
    QScrollArea *scrollArea1 = new QScrollArea (this);
    PagePosuiji *page = new PagePosuiji();
    scrollArea1->setWidget(page);
    //QTextEdit *textLeft=new QTextEdit(QObject::tr("Left Widget"));
    //textLeft->setAlignment(Qt::AlignCenter);
    //splitterMain->addWidget(scrollArea1);
    //splitterMain->addWidget(textLeft);

    ////设置主splitter
    //splitterMain->setStretchFactor(1,1);
}

SpliterTest::~SpliterTest()
{
    delete ui;
}
