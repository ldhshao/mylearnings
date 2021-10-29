#include "cenablemngr.h"
#include "ckeydncombobox.h"
#include <QDebug>


CEnableMngr* CEnableMngr::instance()
{
    static CEnableMngr _ins;
    return &_ins;
}
void CEnableMngr::registerEableUi(CKeyDnComboBox* pCmb, uint16_t* pVal, uint16_t val, QWidget* w)
{
    if (nullptr != pCmb && nullptr != pVal && nullptr != w){
        connect(pCmb, SIGNAL(sig_valueChanged(uint16_t*, uint16_t)), this, SLOT(slot_valueChanged(uint16_t*, uint16_t)));
        valUiMap[pVal][val].push_back(w);

        slot_valueChanged(pVal, *pVal);//set init state
    }
}
void CEnableMngr::slot_valueChanged(uint16_t* pVal, uint16_t valNew)
{
    auto itOut = valUiMap.find(pVal);
    if (itOut != valUiMap.end()){
        auto it = itOut->second.begin();
        for(; it != itOut->second.end(); it++){
            bool enable = false;
            if (valNew == it->first){
                enable = true;
            }
            auto itList = it->second.begin();
            for (; itList!= it->second.end(); itList++){
                (*itList)->setEnabled(enable);
            }
        }
    }
}
