#ifndef CMODPARAMQUERY_H
#define CMODPARAMQUERY_H

#include <QWidget>

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

private slots:
    void on_pushButton_query_clicked();

private:
    void initTable();
    void autoSetSize();

    Ui::CModParamQuery *ui;
};

#endif // CMODPARAMQUERY_H
