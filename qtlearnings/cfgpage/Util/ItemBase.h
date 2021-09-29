#ifndef ITEMBASE_H
#define ITEMBASE_H
#include "DynamicLib.h"
#include <string>
using namespace std;

class ItemBase : public BaseObject
{
public:
    int getId() {return m_id;}
    int getName() {return m_name;}

protect:
    int m_id;
    string m_name;
};
#endif
