#include <iostream>
#include <thread>
#include <string>
using namespace std;

void foo(){
	//do stuff
	cout<<"foo is running"<<endl;
}
void bar(int x){
	//do stuff
	cout<<"bar is running with param x "<<x<<endl;
}

void test0(){
	thread thd_foo(foo);
	thread thd_bar(bar, 1);

	cout<<"test0: foo and bar now execute concurrently.\n";
	thd_foo.join();
	thd_bar.join();
	cout<<"test0: foo and bar now has completed\n";
}

void testcout(){
	cout.setf(ios::hex, ios::basefield);
	cout.setf(ios::showbase);
	cout<<100<<"\n";
	cout.unsetf(ios::basefield);
	cout<<100<<"\n";
	char str[20] = "hello";
	string str1(str);
	string str2;
	cout<<str<<"\n";
	cout<<(void*)str<<"\n";
	cout<<str1.c_str()<<"\n";
	cout<<(void*)str1.c_str()<<"\n";
	str1 = "world";
	cout<<str1.data()<<"\n";
	cout<<(void*)str1.data()<<"\n";
	string str3 = str;
	cout<<(void*)str3.data()<<"\n";
	string const &str4 = str;
	cout<<(void*)str4.data()<<"\n";
}
//void f(int i, string const &s){
//	cout<<;
//}
//void oops1(int iparam){
//	char buffer[256] = {0};
//	thread thd(f, 3, buffer);
//	thd.detach();
//}

int main(){
	//test0();
	testcout();
	//oops1(2);

	return 0;
}
