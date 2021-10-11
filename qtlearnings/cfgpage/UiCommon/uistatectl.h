#ifndef UISTATECTL_H
#define UISTATECTL_H

#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <map>
using namespace std;

class CStateCtlMgr;
class CStateCtl
{
public:
    CStateCtl() : pManager(nullptr) {}
    ~CStateCtl(){}

    enum CTL_STATE{
        CTLS_UNSELECT,
        CTLS_SELECTED,
        CTLS_COUNT
    };

    virtual void setState(CTL_STATE state) {}
    void setManager(CStateCtlMgr *pMgr){	pManager = pMgr;	}

protected:
    CStateCtlMgr *pManager;
};

class CStateCtlMgr : public QObject
{
    Q_OBJECT

public:
    CStateCtlMgr() : pLastCtl(nullptr) {}

    void selectCtl(CStateCtl* pCtl);
    CStateCtl *currentCtl() {	return pLastCtl;	}

protected:
    CStateCtl *pLastCtl;
};

class CStateLineEdit : public QLineEdit, public CStateCtl
{
    Q_OBJECT

public:
    CStateLineEdit(QWidget *parent = nullptr);
    ~CStateLineEdit(){}

    virtual void setState(CTL_STATE state);

protected:
    virtual void focusInEvent(QFocusEvent *event);
};

class CStateComboBox : public QComboBox, public CStateCtl
{
    Q_OBJECT

public:
    CStateComboBox(QWidget *parent = nullptr);
    ~CStateComboBox(){}

    virtual void setState(CTL_STATE state);

protected:
    virtual void focusInEvent(QFocusEvent *event);
};

class CStateCheckBox : public QCheckBox, public CStateCtl
{
    Q_OBJECT

public:
    CStateCheckBox(QWidget *parent = nullptr);
    ~CStateCheckBox(){}

    virtual void setState(CTL_STATE state);

protected:
    virtual void focusInEvent(QFocusEvent *event);
};

#endif // CWIDGETBUTTON_H
