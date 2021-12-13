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
        uiEnableMap[w].push_back({pVal, val});
        valUiMap[pVal].push_back(w);

        slot_valueChanged(pVal, *pVal);//set init state
        qDebug()<<"widget "<<w<<" enableaddr "<<pVal<<" enableval "<<val;
    }
}
void CEnableMngr::slot_valueChanged(uint16_t* pVal, uint32_t valNew)
{
    qDebug()<<"addr "<<pVal<<" value "<<valNew<<" value "<<*pVal;
    auto itOut = valUiMap.find(pVal);
    if (itOut != valUiMap.end()){
        auto itWid = itOut->second.begin();
        for(; itWid != itOut->second.end(); itWid++){
            auto itEnable = uiEnableMap.find(*itWid);
            if(itEnable != uiEnableMap.end()){
                bool en = true;
                auto it = itEnable->second.begin();
                for (; it != itEnable->second.end(); it++){
                    if (*it->addr != it->val){
                        en = false;
                        break;
                    }
                }
                (*itWid)->setEnabled(en);
            }
        }
    }
}
