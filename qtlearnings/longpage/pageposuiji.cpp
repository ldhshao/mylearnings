#include "pageposuiji.h"
#include "ui_pageposuiji.h"

PagePosuiji::PagePosuiji(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PagePosuiji)
{
    ui->setupUi(this);
}

PagePosuiji::~PagePosuiji()
{
    delete ui;
}
