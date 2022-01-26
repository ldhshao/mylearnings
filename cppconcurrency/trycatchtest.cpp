#include <iostream>  
#include <stdexcept> //runtime_error
using namespace std;  
template <typename T>  T Div(T x,T y)  
{  
    if(y==0)  
        throw y;//抛出异常  
    return x/y;  
}  
void test1()
{
    int x=5,y=0;  
    double x1=5.5,y1=0.0;  
    try  
    {  
        //被检查的语句  
        //cout<<x<<"/"<<y<<"="<<Div(x,y)<<endl;  
        cout<<x1<<"/"<<y1<<"="<<Div(x1,y1)<<endl;  
    }  
    catch(int)//异常类型  
    {  
        cout<<"除数为0,计算错误！"<<endl;//异常处理语句  
    }  
    catch(double)//异常类型  
    {  
        cout<<"除数为0.0,计算错误！"<<endl;//异常处理语句  
    }  
}

int triangle(int a, int b, int c)
{
    if(a<0 || b<0 || c<0 || a+b<=c || a+c<=b || b+c<=a)
        throw runtime_error("The lengths of three sides can't form triangle");
    return a + b + c;
}
void test2()
{
    int total = 0;
    try
    {
        total = triangle(3,4,7);
    }
    catch(const runtime_error& e)
    {
        cout<<e.what()<<endl;
    }
    cout<<total<<endl;
}

int main()  
{  
    //test1();
    test2();
    return 0;  
}  
