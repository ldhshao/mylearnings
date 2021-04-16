#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "comboboxdelegate.h"
//#include "cmbdelegate.h"

#include <QStandardItemModel>
#include <QVector>
#include <QPair>
#include <QVariant>
#include <QComboBox>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTextCodec>
#include <windows.h>
#include <QDebug>

//#include "cbdelegate.h"
CKeyPressFilter::CKeyPressFilter(QObject *parent)
    :QObject (parent)
{

}
bool CKeyPressFilter::eventFilter(QObject *obj, QEvent *event)
{
    QEvent::Type tpCurr = event->type();
    if (tpCurr == QEvent::KeyPress || tpCurr == QEvent::KeyRelease) {
        static QString strModSn;
        static QString strTest;
        static QEvent::Type tpPre = QEvent::None;
        static char ch;
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (!keyEvent->text().isEmpty() && tpPre != tpCurr)
        {
            if (QEvent::None != tpPre)
            {
                if (QEvent::KeyRelease == tpCurr)
                {
                    ch = static_cast<char>(keyEvent->key());
                    strTest += keyEvent->text();
                }
            }
            tpPre = tpCurr;
        }
        strModSn += keyEvent->text();
        qDebug()<<tpCurr<<" "<<keyEvent->text();

        if (Qt::Key_Return == keyEvent->key())
        {
            qDebug()<<strModSn;
            qDebug()<<strTest;
            strModSn.clear();
            strTest.clear();
            tpPre = QEvent::None;
        }
    }

    return QObject::eventFilter(obj, event);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _model(nullptr)
{
    ui->setupUi(this);

    _model = new QStandardItemModel(this);
    ui->tableView->setModel( _model );

    CKeyPressFilter *pKeyPressFlt = new CKeyPressFilter(this);
    QCoreApplication::instance()->installEventFilter(pKeyPressFlt);

    connect(ui->pushButtonInit, SIGNAL(clicked()), this, SLOT(initModel()));
    connect( ui->pushButtonFirst, SIGNAL(clicked()), this, SLOT(firstType()));
    connect(ui->pushButtonPrintModel, SIGNAL(clicked()), this, SLOT(printModel()));
    QString string = "D:\\cashway\\workspace\\svnchanyehua\\mcrscoretest\\config\\CC前维护整机外观作业指导书.xlsx";
    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    QByteArray encodedString = codec->fromUnicode(string);
    ShellExecuteA(0, "open", "excel.exe", encodedString.data(), 0, SW_SHOW);
}

MainWindow::~MainWindow()
{
    delete _model;
    delete ui;
}

void MainWindow::initModel()
{
    _model->clear();
    _model->setColumnCount( 3 );

    QList<QStandardItem *> rowData;
    rowData.append( new QStandardItem("first") );
    rowData.append( new QStandardItem("breakfirst") );
    QStandardItem *cell = new QStandardItem("Potato");
    QHash<QString, QVariant> itemData;
    itemData.insert("Potato", 1000);
    itemData.insert("Tomato", 2000);
    itemData.insert("Onion", 3000);
    cell->setData(itemData, Qt::UserRole+2);
    cell->setData(2000, Qt::UserRole + 1 );
    rowData.append( cell);
    _model->appendRow(rowData);

    rowData.clear();
    rowData.append( new QStandardItem("second") );
    rowData.append( new QStandardItem("lunch") );
    cell = new QStandardItem("SuperMan");
    itemData.clear();
    itemData.insert("SuperMan", 1);
    itemData.insert("SpiderMan", 2);
    itemData.insert("SpeedMan", 3);
    cell->setData(itemData, Qt::UserRole+2);
    cell->setData(2, Qt::UserRole + 1 );
    rowData.append( cell);
    _model->appendRow(rowData);

    rowData.clear();
    rowData.append( new QStandardItem("third") );
    rowData.append( new QStandardItem("supper") );
    cell = new QStandardItem("Iron");
    itemData.clear();
    itemData.insert("Iron", 1);
    itemData.insert("Silicon", 2);
    itemData.insert("Stone", 3);
    cell->setData(itemData, Qt::UserRole+2);
    cell->setData(2, Qt::UserRole + 1 );
    rowData.append( cell);


    _model->appendRow(rowData);

    rowData.clear();
    rowData.append( new QStandardItem("Forth") );
    rowData.append( new QStandardItem("Vegetable") );
    cell = new QStandardItem("Fruit");
    itemData.clear();
    itemData.insert("strawberry", 1);
    itemData.insert("WaterMalon", 2);
    itemData.insert("Apple", 3);
    cell->setData(itemData, Qt::UserRole+2);
    cell->setData(2, Qt::UserRole + 1 );
    rowData.append( cell);
    _model->appendRow(rowData);

    rowData.clear();
    rowData.append( new QStandardItem("Fifth") );
    rowData.append( new QStandardItem("Meat") );
    cell = new QStandardItem("Chicken");
    itemData.clear();
    itemData.insert("CowMeat", 1);
    itemData.insert("Fork", 2);
    itemData.insert("Yam", 3);
    cell->setData(itemData, Qt::UserRole+2);
    cell->setData(2, Qt::UserRole + 1 );
    rowData.append( cell);
    _model->appendRow(rowData);

    rowData.clear();
    rowData.append( new QStandardItem("Sixth") );
    rowData.append( new QStandardItem("Soup") );
    cell = new QStandardItem("Table");
    itemData.clear();
    itemData.insert("Table", 1);
    itemData.insert("Desk", 2);
    itemData.insert("Chair", 3);
    cell->setData(itemData, Qt::UserRole+2);
    cell->setData(2, Qt::UserRole + 1 );
    rowData.append( cell);

    _model->appendRow(rowData);

    rowData.clear();
    rowData.append( new QStandardItem("Seventh") );
    rowData.append( new QStandardItem("braise") );
    cell = new QStandardItem("Instrument");
    itemData.clear();
    itemData.insert("TV", 1);
    itemData.insert("Recorder", 2);
    itemData.insert("AirC", 3);
    cell->setData(itemData, Qt::UserRole+2);
    cell->setData(2, Qt::UserRole + 1 );
    rowData.append( cell);
    _model->appendRow(rowData);

    ui->tableView->resizeColumnsToContents();
    ui->tableView->setColumnWidth( 2, 256 );

    QAbstractItemDelegate *oldCbd = ui->tableView->itemDelegateForColumn( 2 );
    if ( oldCbd != nullptr ) {
        delete oldCbd;
    }

    ComboBoxDelegate *cbd = new ComboBoxDelegate(this);
    //CmbDelegate *cbd = new CmbDelegate(this);
    ui->tableView->setItemDelegateForColumn(2, cbd);
    ui->tableView->openPersistentEditor( _model->index(3,2) );
    ui->tableView->openPersistentEditor( _model->index(4,2) );

    ui->tableView->resizeColumnsToContents();

}

void MainWindow::firstType()
{
#if 0
    QStandardItem *firstTypeItem = _model->item( 3, 2 );
    if ( firstTypeItem != nullptr ) {
        QHash<QString, QVariant> itemData;
        itemData.insert("This is my puppy", 1);
        itemData.insert("His name is Buddy", 2);
        itemData.insert("He is 2-year-old", 3);
        itemData.insert("Smart Boarder Collie", 4);
        firstTypeItem->setData(itemData, Qt::UserRole+2);
        firstTypeItem->setData(2, Qt::UserRole + 1 );
//下边两行用于直接显式修改后的结果
        ui->tableView->openPersistentEditor( firstTypeItem->index() );
        ui->tableView->setCurrentIndex( firstTypeItem->index() );

    }


    ui->tableView->resizeColumnsToContents();

#endif
}

void MainWindow::printModel()
{
    QItemSelectionModel *selection = ui->tableView->selectionModel();
    if ( selection == nullptr ) {
        return;
    }

    if ( selection->hasSelection() == false ) {
        return;
    }

    QString displayTxt = selection->currentIndex().model()->data( selection->currentIndex(), Qt::DisplayRole ).toString();
    QString userTxt = selection->currentIndex().model()->data( selection->currentIndex(), Qt::UserRole + 1 ).toString();
    ui->plainTextEdit->appendPlainText( QString("displayRole:%1--UserRole1:%2").arg(displayTxt).arg(userTxt) );
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    static QString strMsg;
    if(event->key() == Qt::Key_Return){//enter
        qDebug() << strMsg;
        strMsg.clear();
    }else{
        strMsg += event->text();
    }
}
