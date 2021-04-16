#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

QT_BEGIN_NAMESPACE
class QStandardItemModel;
QT_END_NAMESPACE

class CKeyPressFilter : public QObject
{
    Q_OBJECT

public:
    explicit CKeyPressFilter(QObject *parent=Q_NULLPTR);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *eventy) override;

private slots:
    void initModel();
    void firstType();
    void printModel();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *_model;
};

#endif // MAINWINDOW_H
