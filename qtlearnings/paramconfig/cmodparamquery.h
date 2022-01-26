#ifndef CMODPARAMQUERY_H
#define CMODPARAMQUERY_H

#include <QWidget>
#include <map>
using namespace std;

class QLabel;
namespace Ui {
class CModParamQuery;
}

class CModParamQuery : public QWidget
{
    Q_OBJECT

public:
    explicit CModParamQuery(QWidget *parent = nullptr);
    ~CModParamQuery();

protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void changeEvent(QEvent *event);

private slots:
    void on_pushButton_query_clicked();

private:
    void initTable();
    void autoAdjustTableColumns();
    void autoSetFocus();
    void readModifiedRecord(int y, int m, int d, map<QString, QStringList> *mapTimeRecords);

    Ui::CModParamQuery *ui;
    QLabel             *bkLbl;
};

#endif // CMODPARAMQUERY_H
