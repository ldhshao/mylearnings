#include <iostream>
using namespace std;

template <class T,class BaseT>
class CInheritIdentify
{
    private:
        static char Identify( void* vd )
        {
            return 0;
        }
        static int Identify( BaseT* bt )
        {
            return 1;
        }

    public:
        enum
        {
            IdentifyResult = (sizeof(Identify((T*)NULL)) == sizeof(int)),
        };

};

class CBase{
};
class CSub :public CBase{
};
class CMyClass{
};

int main(){
    cout<<"hello"<<((2<3)? "yes": "no")<<"\n";
    cout<<"CSub "<<((CInheritIdentify<CSub, CBase>::IdentifyResult)?"inherit":"not inherit")<<" CBase\n";
    cout<<"CMyClass "<<((CInheritIdentify<CMyClass, CBase>::IdentifyResult)?"inherit":"not inherit")<<" CBase\n";

    return 0;
}
