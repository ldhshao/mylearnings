#include <iostream>
#include <thread>
#include <utility>//move
#include <stdexcept>//logic_error
using namespace std;

class scoped_thread{
	thread t;
public:
	explicit scoped_thread(std::thread t_)
		:t(move(t_))
	{
		if(!t.joinable())
			throw logic_error("No thread");
	}
	~scoped_thread()
	{
		t.join();
		cout<<"scoped_thread disconstructor\n";
	}
	scoped_thread(scoped_thread const&)=delete;
	scoped_thread& operator=(scoped_thread const&)=delete;

};
void foo(){
	//do stuff
	cout<<"foo is running"<<endl;
}
void test0(){
	cout<<"test0 running\n";
#if 0
	thread thd(foo);
	scoped_thread t(move(thd));
#endif
#if 1
	scoped_thread t(move(thread(foo)));
#endif
	cout<<"test0 closed\n";
}

int main(){
	test0();
	return 0;
}
