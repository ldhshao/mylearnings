#include <iostream>
#include <functional>
using namespace std;

void test(){
	int a = 10;
	int &b = a;
	reference_wrapper<int> aref(a);
	reference_wrapper<int> tref = ref(a);
	int &c = aref;
	int &d = tref;

	cout<<(void*)&a<<"\n";
	cout<<(void*)&b<<"\n";
	cout<<(void*)&aref<<"\n";
	cout<<(void*)&tref<<"\n";
	cout<<(void*)&c<<"\n";
	cout<<(void*)&d<<"\n";
	cout<<aref<<"\n";
}
int main(){
	test();
	return 0;
}
