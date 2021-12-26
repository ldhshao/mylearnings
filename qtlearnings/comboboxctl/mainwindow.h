#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class CKeyDnComboBoxSet;
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
    CKeyDnComboBoxSet *cmbSet;
    uint16_t           data[5];
};
#endif // MAINWINDOW_H
