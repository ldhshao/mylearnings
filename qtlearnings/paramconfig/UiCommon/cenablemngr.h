#ifndef CENABLEMNFR_H
#define CENABLEMNGR_H
#include <QObject>
#include <map>
using namespace std;

class CKeyDnComboBox;
class QWidget;
class CEnableMngr : public QObject
{
    Q_OBJECT

public:
    static CEnableMngr* instance();
    void registerEableUi(CKeyDnComboBox* pCmb, uint16_t* pVal, uint16_t val, QWidget* w);

protected:
    CEnableMngr(QWidget *parent = 0){}
    virtual ~CEnableMngr(){
    }

protected slots:
    void slot_valueChanged(uint16_t* pVal, uint16_t valNew);

private:
    map<uint16_t*, map<uint16_t, list<QWidget*>>>  valUiMap;
};

#endif // CKEYDNEDIT_H
