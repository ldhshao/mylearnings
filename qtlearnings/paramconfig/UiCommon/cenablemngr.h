#ifndef CENABLEMNFR_H
#define CENABLEMNGR_H
#include <QObject>
#include <map>
using namespace std;

class CKeyDnComboBox;
class CKeyDnEdit;
class QWidget;
class UiCfgItem;
struct SEnablePair{
    uint16_t *addr;
    uint16_t  val;
};
struct SValListPair{
    uint16_t val;
    list<UiCfgItem*> itemList;
};
class CEnableMngr : public QObject
{
    Q_OBJECT

public:
    static CEnableMngr* instance();
    void registerEableUi(CKeyDnComboBox* pCmb, uint16_t* pVal, uint16_t val, UiCfgItem* item);
    bool isEnableSource(uint16_t* pVal) { return valUiMap.find(pVal) != valUiMap.end(); }

protected:
    CEnableMngr(QWidget *parent = 0){}
    virtual ~CEnableMngr(){
    }

protected slots:
    void slot_valueChanged(uint16_t* pVal, uint32_t valNew);

private:
    map<QWidget*, list<struct SEnablePair>>  uiEnableMap;
    map<uint16_t*, struct SValListPair>      valUiMap;
    map<QWidget*, bool> sourceMap;
};

class CMaxValMngr : public QObject
{
    Q_OBJECT

public:
    static CMaxValMngr* instance();
    void registerMaxValUi(CKeyDnEdit* pEdit, uint16_t* pVal, UiCfgItem* item);

protected:
    CMaxValMngr(QObject *parent = 0):QObject(parent){}
    virtual ~CMaxValMngr(){
    }

protected slots:
    void slot_valueChanged(uint16_t* pVal, uint32_t valNew);

private:
    map<uint16_t*, list<UiCfgItem*>>      valListMap;
};
#endif // CKEYDNEDIT_H
