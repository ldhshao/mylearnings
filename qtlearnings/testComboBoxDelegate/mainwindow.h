#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

QT_BEGIN_NAMESPACE
class QStandardItemModel;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void initModel();
    void firstType();
    void printModel();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *_model;
};

#endif // MAINWINDOW_H
