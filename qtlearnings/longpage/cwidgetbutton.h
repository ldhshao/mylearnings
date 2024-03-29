#ifndef CWIDGETBUTTON_H
#define CWIDGETBUTTON_H

#include <QPushButton>
#include <map>
using namespace std;

class CStateButton : public QPushButton
{
    Q_OBJECT

public:
    CStateButton(QWidget *parent = nullptr);
    ~CStateButton(){}

    enum BUTTON_STATE{
        BS_NORMAL,
        BS_SELECTED,
        BS_COUNT
    };

    void setButtonState(BUTTON_STATE state);

signals:
    void sig_button_clicked(CStateButton* pBtn);

public slots:
    void slot_clicked(bool);
};

class CWidgetButton : public CStateButton
{
    Q_OBJECT

public:
    explicit CWidgetButton(QWidget *w, QWidget *parent = nullptr);
    QWidget *getWidget(){	return pWidget;	}

protected:
    QWidget *pWidget;
};


class CStateButtonMgr : public QObject
{
    Q_OBJECT

public:
    CStateButtonMgr(QObject *parent=nullptr);

    void registerButton(CStateButton* pBtn);
    void selectButton(CStateButton* pBtn);
    CStateButton *currentButton() {	return pLastBtn;	}

signals:
    void sig_button_clicked(CStateButton* pBtn);

protected slots:
    void slot_btnClicked(CStateButton* pBtn);

protected:
    CStateButton *pLastBtn;
};
class CWidgetButtonMgr : public QObject
{
    Q_OBJECT

public:
    CWidgetButtonMgr(QObject *parent=nullptr);
    ~CWidgetButtonMgr();

    //void addWidgetButton(CWidgetButton* pBtn);
    CWidgetButton* addWidgetButton(QWidget* w, QString strName);
    void clear();

signals:
    void sig_showWidget(QWidget* pSubWid);

protected slots:
    void slot_btnClicked(CStateButton* pBtn);
    void slot_selectButton(QWidget* pSubWid);

protected:
    void _selectButton(CStateButton* pBtn);
    CWidgetButton *pLastBtn;
    map<QWidget*, CWidgetButton*> mapButtons;
};

#endif // CWIDGETBUTTON_H
