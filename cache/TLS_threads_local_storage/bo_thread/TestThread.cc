#include "Thread.hpp"

#include <unistd.h>

#include <iostream>
#include <memory>
using std::cout;
using std::endl;
using std::unique_ptr;
 

struct MyTask
{
	void process()
	{
		while(1) {
			cout << "MyThread::run()" << endl;
			sleep(1);
		}
	}
};


void process()
{
	while(1) {
		cout << "process()" << endl;
		cout << wd::current_thread::threadName << endl;
		sleep(1);
	}
}

void test()
{
	MyTask task;
	unique_ptr<wd::Thread> pthread(new wd::Thread(
		process, "1"));
	unique_ptr<wd::Thread> pthread2(new wd::Thread(
		process, "2"));

	pthread->start();//开启线程
	pthread2->start();
	pthread->join();
	pthread2->join();
}

int main(void)
{
	test();
	return 0;
}
