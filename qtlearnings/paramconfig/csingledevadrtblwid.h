#ifndef CSINGLEDEVADRTBLWID_H
#define CSINGLEDEVADRTBLWID_H

#include <QWidget>
#include <list>
#include <utility>
using namespace std;

namespace Ui {
class CSingleDevAdrTblWid;
}

class CSingleDevAdrTblWid : public QWidget
{
    Q_OBJECT

public:
    explicit CSingleDevAdrTblWid(QWidget *parent = nullptr);
    ~CSingleDevAdrTblWid();

    void setTitle(const QString &strTitle);
    void setPortInfo(list<pair<uint32_t, QString>> *pList);
    bool pageAction(int action);//

    enum {
      ACTION_INC = 0,
      ACTION_DES,
      ACTION_CNT
    };
protected:
    void updatePage();
    void onResize(int width, int height);
    virtual void resizeEvent(QResizeEvent *event);

private:
    Ui::CSingleDevAdrTblWid *ui;
    int  pageSize;
    int  pageIdx;
    list<pair<uint32_t, QString>> portList;
};

#endif // CSINGLEDEVADRTBLWID_H
