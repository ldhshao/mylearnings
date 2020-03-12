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
void f(int i, string const &s){
	cout<<i<<"\n";
	cout<<s<<"\n";
	cout<<(void*)s.data()<<"\n";
}
void oops1(int iparam){
	char buffer[256] = {0};
	cout<<(void*)buffer<<"\n";
	sprintf(buffer, "%d", iparam);
	thread thd(f, 3, string(buffer));
	//thread thd(f, 3, buffer);
	thd.detach();
}
struct widget_data{
	int left, top;
	int width, heigth;

	widget_data(){
		left = top = 0;
		width = 100; heigth = 50;
	}
	void dump(){
		cout<<"left, top, width, heigth "<<left<<","<<top<<","<<width<<","<<heigth<<"\n";
	}
};
void update_widget(int& wid){
	cout<<wid<<"\n";
	cout<<(void*)&wid<<"\n";
}
void update_data_for_widget(int wid, struct widget_data& wdata){
	cout<<wid<<"\n";
	cout<<(void*)&wdata<<"\n";
	wdata.dump();
}
void oops2(int wid){
	struct widget_data wdata;
	cout<<(void*)&wdata<<"\n";
	wdata.dump();
	cout<<wid<<"\n";
	cout<<(void*)&wid<<"\n";

	//thread thd(update_widget, ref(wid));
	thread thd(update_data_for_widget, wid, ref(wdata));
	thd.detach();
}

class CX
{
	public:
		void do_some_work(){ cout<<"do some work\n";}
		void do_some_work_ex(int param){ cout<<"do some work with param "<<param<<"\n";}
} my_x;
void test3(){
	for(int i = 0; i < 100; i++){
	//thread thd(&CX::do_some_work, &my_x);
	thread thd(&CX::do_some_work_ex, &my_x, i);
	thd.detach();
	}
}

void test4(){
	for(int i = 0; i < 100; i++){
	thread thd(&CX::do_some_work_ex, &my_x, i);
	thd.detach();
	}
}

int main(){
	//test0();
	//testcout();
	//oops1(2);
	//oops2(3);
	test3();

	return 0;
}
