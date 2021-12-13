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
        connect(pCmb, SIGNAL(sig_valueChanged(uint16_t*, uint32_t)), this, SLOT(slot_valueChanged(uint16_t*, uint32_t)));
        valUiMap[pVal][val].push_back(w);

        slot_valueChanged(pVal, *pVal);//set init state
    }
}
void CEnableMngr::slot_valueChanged(uint16_t* pVal, uint32_t valNew)
{
    qDebug()<<"addr "<<pVal<<" value "<<valNew<<" value "<<*pVal;
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
