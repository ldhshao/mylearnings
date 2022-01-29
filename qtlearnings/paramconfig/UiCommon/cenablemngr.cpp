#include "cenablemngr.h"
#include "ckeydncombobox.h"
#include "ckeydnedit.h"
#include "../Util/UiCfgItem.h"
#include <QDebug>


CEnableMngr* CEnableMngr::instance()
{
    static CEnableMngr _ins;
    return &_ins;
}
void CEnableMngr::registerEableUi(CKeyDnComboBox* pCmb, uint16_t* pVal, uint16_t val, UiCfgItem* item)
{
    if (nullptr != pCmb && nullptr != pVal && nullptr != item){
        if (sourceMap.find(pCmb) == sourceMap.end()){
            connect(pCmb, SIGNAL(sig_valueChanged(uint16_t*, uint32_t)), this, SLOT(slot_valueChanged(uint16_t*, uint32_t)));
            sourceMap[pCmb] = true;
        }
        uiEnableMap[item->getWidget()].push_back({pVal, val});
        auto it = valUiMap.find(pVal);
        if (it != valUiMap.end()){
            bool bFind = false;
            for (auto itVal = it->second.begin(); itVal != it->second.end(); itVal++){
               if (val == itVal->val){
                   itVal->itemList.push_back(item);
                   bFind = true;
                   break;
               }
            }
            if (!bFind){
                list<UiCfgItem*> tList;
                tList.push_back(item);
                it->second.push_back({val, tList});
            }
        }else {
            list<UiCfgItem*> tList;
            list<struct SValListPair> valList;
            tList.push_back(item);
            valList.push_back({val, tList});
            valUiMap[pVal] = valList;
        }

        slot_valueChanged(pVal, *pVal);//set init state
        qDebug()<<"item "<<item<<" enableaddr "<<pVal<<" enableval "<<val;
    }
}
void CEnableMngr::slot_valueChanged(uint16_t* pVal, uint32_t valNew)
{
    qDebug()<<"addr "<<pVal<<" value "<<valNew<<" value "<<*pVal;
    //auto itOut = valUiMap.find(pVal);
    //if (itOut != valUiMap.end()){
    //    auto itWid = itOut->second.begin();
    //    for(; itWid != itOut->second.end(); itWid++){
    //        auto itEnable = uiEnableMap.find(*itWid);
    //        if(itEnable != uiEnableMap.end()){
    //            bool en = true;
    //            auto it = itEnable->second.begin();
    //            for (; it != itEnable->second.end(); it++){
    //                if (*it->addr != it->val){
    //                    en = false;
    //                    break;
    //                }
    //            }
    //            (*itWid)->setEnabled(en);
    //        }
    //    }
    //}

    auto itOut = valUiMap.find(pVal);
    if (itOut != valUiMap.end()){
        for (auto itVal = itOut->second.begin(); itVal != itOut->second.end(); itVal++){
            bool enable = (itVal->val == valNew);
            for (auto itItem = itVal->itemList.begin(); itItem != itVal->itemList.end(); itItem++){
                QWidget* w = (*itItem)->getWidget();
                if (nullptr != w) w->setEnabled(enable);
                if (!enable)//set deault val ;
                    (*itItem)->setDefaultVal();
                qDebug()<<*itItem;
            }
        }
    }
}

CMaxValMngr* CMaxValMngr::instance()
{
    static CMaxValMngr _mngr;
    return &_mngr;
}
void CMaxValMngr::registerMaxValUi(CKeyDnEdit* pEdit, uint16_t* pVal, UiCfgItem* item)
{
    if (nullptr == pVal || nullptr == pEdit || nullptr == item) return ;

    auto itFind = valListMap.find(pVal);
    if (itFind == valListMap.end()){
        connect(pEdit, SIGNAL(sig_valueChanged(uint16_t*, uint32_t)), this, SLOT(slot_valueChanged(uint16_t*, uint32_t)));
        list<UiCfgItem*> itemList;
        itemList.push_back(item);
        valListMap[pVal] = itemList;
    }else {
        itFind->second.push_back(item);
    }
}
void CMaxValMngr::slot_valueChanged(uint16_t* pVal, uint32_t valNew)
{
    auto it = valListMap.find(pVal);
    if (valListMap.end() != it){
        auto itItem = it->second.begin();
        for (; itItem != it->second.end(); itItem++){
            (*itItem)->onMaxValChanged(valNew);
        }
    }
}

///CMinValMngr
CMinValMngr* CMinValMngr::instance()
{
    static CMinValMngr _mngr;
    return &_mngr;
}
void CMinValMngr::registerMinValUi(CKeyDnEdit* pEdit, uint16_t* pVal, UiCfgItem* item)
{
    if (nullptr == pVal || nullptr == pEdit || nullptr == item) return ;

    auto itFind = valListMap.find(pVal);
    if (itFind == valListMap.end()){
        connect(pEdit, SIGNAL(sig_valueChanged(uint16_t*, uint32_t)), this, SLOT(slot_valueChanged(uint16_t*, uint32_t)));
        list<UiCfgItem*> itemList;
        itemList.push_back(item);
        valListMap[pVal] = itemList;
    }else {
        itFind->second.push_back(item);
    }
}
void CMinValMngr::slot_valueChanged(uint16_t* pVal, uint32_t valNew)
{
    auto it = valListMap.find(pVal);
    if (valListMap.end() != it){
        auto itItem = it->second.begin();
        for (; itItem != it->second.end(); itItem++){
            (*itItem)->onMinValChanged(valNew);
        }
    }
}

//CVisibleMngr
CVisibleMngr* CVisibleMngr::instance()
{
    static CVisibleMngr _ins;
    return &_ins;
}

bool isListEqaul(list<uint16_t> *pList1, list<uint16_t> *pList2)
{
    if (pList1->size() == pList2->size()){
        for(auto it1= pList1->begin(), it2 = pList2->begin(); it1 != pList1->end() && it2 != pList2->end(); it1++, it2++){
            //qDebug()<<*it1<<*it2;
            if (*it1 != *it2) return false;
        }

        return true;
    }
    return false;
}

void CVisibleMngr::registerVisibleUi(CKeyDnComboBox* pCmb, uint16_t* pVal, list<uint16_t> *pValList, UiCfgItem* item)
{
    if (nullptr != pCmb && nullptr != pVal && nullptr != item){
        if (sourceMap.find(pCmb) == sourceMap.end()){
            connect(pCmb, SIGNAL(sig_valueChanged(uint16_t*, uint32_t)), this, SLOT(slot_valueChanged(uint16_t*, uint32_t)));
            sourceMap[pCmb] = true;
        }
        auto it = valUiMap.find(pVal);
        if (it != valUiMap.end()){
            bool bFind = false;
            for (auto itPair = it->second.begin(); itPair != it->second.end(); itPair++){
               if (isListEqaul(pValList, &itPair->valList)){
                   itPair->itemList.push_back(item);
                   bFind = true;
                   break;
               }
            }
            if (!bFind){
                list<UiCfgItem*> tList;
                tList.push_back(item);
                it->second.push_back({*pValList, tList});
            }
        }else {
            struct SVallistListPair pairList;
            pairList.valList = *pValList;
            pairList.itemList.push_back(item);
            pairList.valList.sort();
            list<struct SVallistListPair> tList;
            tList.push_back(pairList);
            valUiMap[pVal] = tList;
        }

        slot_valueChanged(pVal, *pVal);//set init state
    }
}
void CVisibleMngr::slot_valueChanged(uint16_t* pVal, uint32_t valNew)
{
    qDebug()<<"addr "<<pVal<<" value "<<valNew<<" value "<<*pVal;
    auto itOut = valUiMap.find(pVal);
    if (itOut != valUiMap.end()){
        for (auto itPair = itOut->second.begin(); itPair != itOut->second.end(); itPair++){
            bool enable = false;
            for (auto itVal = itPair->valList.begin(); itVal != itPair->valList.end(); itVal++){
                if (valNew == *itVal){
                    enable = true; break;
                }
            }
            for (auto itItem = itPair->itemList.begin(); itItem != itPair->itemList.end(); itItem++){
                QWidget* w = (*itItem)->getWidget();
                if (nullptr != w) w->setVisible(enable);
                w = (*itItem)->getWidName();
                if (nullptr != w) w->setVisible(enable);
                w = (*itItem)->getWidDes();
                if (nullptr != w) w->setVisible(enable);
            }
        }
    }
}
