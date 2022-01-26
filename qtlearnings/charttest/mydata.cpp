#include "mydata.h"
//#include <string>

CMyData::CMyData()
{

}

QString CMyData::getDesc(int idx)
{
    QString str("index ");
    str += QString::asprintf("%d", idx);

    return str;
}
