#include "cparamtablemngrwid.h"
#include "ui_cparamtablemngrwid.h"
#include <QResizeEvent>
#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>
#include <fstream>
#include <QDebug>

#define WIDGET_BKCOLOR         "background-color:rgba(255,255,255,100%);"
#define PROPERTY_WORKDIR "workdir"
#define PROPERTY_PARCNT  "paramcount"
#define BAKEUP_WORKDIR   "bakeup"
CParamTableMngrWid::CParamTableMngrWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CParamTableMngrWid)
{
    setWindowFlags(Qt::FramelessWindowHint);
    bkLbl = new QLabel(this);
    bkLbl->setStyleSheet(WIDGET_BKCOLOR);
    paramSrvAddr = nullptr;
    paramLclAddr = nullptr;
    fileNameMax = 20;
    fileNumMax = 20;
    ui->setupUi(this);
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
}

CParamTableMngrWid::~CParamTableMngrWid()
{
    delete ui;
}

void CParamTableMngrWid::showUi()
{
    workDir = QCoreApplication::instance()->property(PROPERTY_WORKDIR).toString() + "/" + BAKEUP_WORKDIR;
    QDir dir(workDir);
    if (!dir.exists())
        dir.mkdir(dir.absolutePath());

    showParamFiles();
}

void CParamTableMngrWid::autoSetFocus()
{
    ui->lineEdit_fileNo->setFocus();
}

void CParamTableMngrWid::showParamFiles()
{
    ui->tableWidget->clear();

    QStringList headList;
    headList<<"参数表文件";
    ui->tableWidget->setColumnCount(headList.count());
    ui->tableWidget->setHorizontalHeaderLabels(headList);
    QDir dir(workDir);
    QFileInfoList list = dir.entryInfoList(QDir::Files| QDir::NoDot | QDir::NoDotDot);
    ui->tableWidget->setRowCount(list.count());
    int row = 0;
    foreach (QFileInfo fileInfo, list){
        QTableWidgetItem *item = new QTableWidgetItem(fileInfo.fileName());
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget->setItem(row++, 0, item);
    }
}

void CParamTableMngrWid::showMessage(QString msg)
{
    qDebug()<<msg;
    ui->label_message->setText(msg);
}

void CParamTableMngrWid::resizeEvent(QResizeEvent *event)
{
    int m = 16, s = 8;
    int w = event->size().width(), h = event->size().height();
    int btnW = ui->pushButton_bakeup->width();
    int btnH = ui->pushButton_bakeup->height();
    int l0 = m, l1 = w - m;

    bkLbl->resize(w, h);
    bkLbl->move(0, 0);
    //int tnW = QFontMetrics(font()).width(ui->label_tableName->text());
    //ui->label_tableName->resize(tnW, btnH);
    //ui->label_tableName->move(m, m);
    l1 = l1 - ui->pushButton_preview->width();
    ui->pushButton_preview->move(l1, m);
    l1 -= (3*s + btnW);
    ui->pushButton_delete->move(l1, m);
    l1 -= (s + btnW);
    ui->pushButton_restore->move(l1, m);
    l1 -= (s + btnW);
    ui->pushButton_bakeup->move(l1, m);
    l1 -= s;
    ui->label_fileNo->move(l0, m);
    l0 += (ui->label_fileNo->width() + s);
    ui->lineEdit_fileNo->resize(l1 - l0, ui->lineEdit_fileNo->height());
    ui->lineEdit_fileNo->move(l0, m);
    ui->tableWidget->resize(w - 2*m, h - 2*m- s - 2*btnH);
    ui->tableWidget->setColumnWidth(0, ui->tableWidget->width() - 80);
    ui->tableWidget->move(m, m + btnH + s);
    ui->label_message->move(m, h -btnH);
}

void CParamTableMngrWid::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<"CParamTableMngrWid::keyPressEvent"<<focusWidget();
    if (focusWidget() == this){
        autoSetFocus();
    }
    switch (event->key()) {
        case Qt::Key_Left:
            return ;
        case Qt::Key_Right:
            return ;
        case Qt::Key_Escape:
            setEnabled(false);
            return ;
        case Qt::Key_Up:
        case Qt::Key_Down:
            return ;
        default:
            QWidget::keyPressEvent(event);
    }
}
void CParamTableMngrWid::changeEvent(QEvent *event)
{
    switch (event->type()) {
    case QEvent::EnabledChange:
        if (isEnabled()) autoSetFocus();
        break;
    default:
        QWidget::changeEvent(event);
    }
}

void CParamTableMngrWid::on_pushButton_bakeup_clicked()
{
    if(ui->lineEdit_fileNo->text().length() > fileNameMax){
        showMessage(QString("备份文件名过长!最大长度为").append(QString::number(fileNameMax)));
        return ;
    }
    else if(ui->lineEdit_fileNo->text().length() == 0){
        showMessage("请输入备份文件名!");
        return ;
    }
    if(ui->tableWidget->rowCount() >= fileNumMax){
        showMessage("参数备份文件数量已达上限,请先删除部分参数文件!");
        return ;
    }
    QDir destDir(workDir);
    /* 备份文件已存在 */
    if(destDir.exists(ui->lineEdit_fileNo->text())){
        QMessageBox message(QMessageBox::Question,"提示","备份参数文件已存在,是否覆盖?",QMessageBox::Yes|QMessageBox::No,NULL);
        message.setDefaultButton(QMessageBox::No);
        if (message.exec()==QMessageBox::No){
            return;
        }

    }
    ofstream outFile;
    QString strDisPath = workDir + "/" + ui->lineEdit_fileNo->text();
    int paramCnt = QCoreApplication::instance()->property(PROPERTY_PARCNT).toInt();
    outFile.open(strDisPath.toStdString(), ios_base::out|ios_base::binary);
    outFile.write(reinterpret_cast<char*>(paramLclAddr), paramCnt*sizeof (uint16_t));
    outFile.close();

    showParamFiles();
}

void CParamTableMngrWid::on_pushButton_restore_clicked()
{
    bool ok = false;
    int index = ui->lineEdit_fileNo->text().toInt(&ok, 10);
    if(false == ok || ui->tableWidget->rowCount() < index || 0 >= index){
        showMessage("请输入合理范围内的备份参数文件序号!");
        return;
    }
    QMessageBox message(QMessageBox::Question,"提示","确定恢复此备份文件?",QMessageBox::Yes|QMessageBox::No,NULL);
    message.setDefaultButton(QMessageBox::No);
    if (message.exec()==QMessageBox::Yes){
        ifstream inFile;
        QString strParam = workDir + "/" + ui->tableWidget->item(index-1, 0)->text();
        inFile.open(strParam.toStdString(), ios_base::in | ios_base::binary);
        inFile.seekg(0, inFile.end);
        int paramSize = inFile.tellg();
        inFile.seekg(0, inFile.beg);
        inFile.read(reinterpret_cast<char*>(paramLclAddr), paramSize);
        inFile.close();

        memcpy(paramSrvAddr, paramLclAddr, paramSize);
        showMessage(QString("%1").arg(index) + "号备份文件恢复成功");
    }
}

void CParamTableMngrWid::on_pushButton_delete_clicked()
{
    bool ok = false;
    int index = ui->lineEdit_fileNo->text().toInt(&ok, 10);
    if(false == ok || ui->tableWidget->rowCount() < index || 0 >= index){
        showMessage("请输入合理范围内的备份参数文件序号!");
        return;
    }
    QMessageBox message(QMessageBox::Question,"提示","确定删除备份文件?",QMessageBox::Yes|QMessageBox::No, NULL);
    message.setDefaultButton(QMessageBox::No);
    if (message.exec()==QMessageBox::Yes){
        QDir dir(workDir);
        QString  strDisPath = ui->tableWidget->item(index-1, 0)->text();
        if (dir.exists(strDisPath))
            ok = dir.remove(strDisPath);
        //QString  strDisPath = workDir + "/" + ui->tableWidget->item(index-1, 0)->text();
        if (ok)
            showParamFiles();
    }
}

void CParamTableMngrWid::on_pushButton_preview_clicked()
{
}
