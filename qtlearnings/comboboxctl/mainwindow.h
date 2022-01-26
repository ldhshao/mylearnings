#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#define  TEST_DATA_SETSIZE  5
#define  TEST_DATA_SETCOUNT 5
#define  TEST_DATA_COUNT    (TEST_DATA_SETSIZE * TEST_DATA_SETCOUNT)

class QLabel;
class CKeyDnComboBoxSet;
class CKeyDnComboBoxEx;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slot_dataIndexChanged(int dataIdx);
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QLabel         *nameLbl;
    CKeyDnComboBoxEx *cmbSet;
    uint16_t           data[TEST_DATA_COUNT];
};
#endif // MAINWINDOW_H
