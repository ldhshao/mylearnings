#ifndef CLINESELECTOR_H
#define CLINESELECTOR_H
#include "UiCommon/cwidgetbutton.h"
#include <vector>
#include <QWidget>
using namespace std;
class QTimer;
class QCheckBox;
class CDevPointEdit;

class CButtonSelector : public QWidget
{
    Q_OBJECT
public:
    void   selectButtonByIndex(int idx);
    void   adjustPosition(int x, int y, int w, int h);
    void   hideSelector();

protected:
    explicit CButtonSelector(QWidget *parent = nullptr);

    virtual void keyPressEvent(QKeyEvent *event);
    virtual void nextSelector(){}
    virtual void prevSelector(){}

protected slots:
    void slot_emitTimer();

protected:
    CStateButtonMgr btnMgr;
    vector<CStateButton*> btnList;
    int                   btnVisibleCnt;
    QTimer               *emitTimer;
    int                   timerInterval;
    int                   usrKey;
};

class CLineSelector : public CButtonSelector
{
    Q_OBJECT
public:
    static CLineSelector* instance();

protected:
    explicit CLineSelector(QWidget *parent = nullptr);
    virtual void nextSelector();
    virtual void prevSelector();

protected slots:
    void slot_btnClicked(CStateButton* pBtn);
};

class CMachineSelector : public CButtonSelector
{
    Q_OBJECT
public:
    static CMachineSelector* instance();
    void   showMachines(int line);

protected:
    explicit CMachineSelector(QWidget *parent = nullptr);
    virtual void nextSelector();
    virtual void prevSelector();
    int      currLine;

protected slots:
    void slot_btnClicked(CStateButton* pBtn);
};

class CPortSelector : public QWidget
{
    Q_OBJECT
public:
    static CPortSelector* instance();
    void   showPorts(int line, int machine);
    void   setAttachEdit(CDevPointEdit* edit) {	pEdit = edit; }
    void   setPortType(uint8_t type) { portType = type; }
    void   adjustPosition(int x, int y, int w, int h);
    void   exitSelector();

protected:
    explicit CPortSelector(QWidget *parent = nullptr);
    virtual void keyPressEvent(QKeyEvent *event);
    vector<QCheckBox*> btnList;
    int      currLine;
    int      currMachine;
    uint8_t  portType;
    bool     chkLoading;
    CDevPointEdit *pEdit;
    QTimer               *emitTimer;
    int                   timerInterval;
    int                   usrKey;

protected slots:
    void on_checkbox_stateChanged(int newState);
    void slot_emitTimer();
};
#endif // CLINESELECTOR_H
