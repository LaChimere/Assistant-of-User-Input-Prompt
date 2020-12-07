#include "TimerThread.hpp"
#include "CacheManager.hpp"

#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>
using std::cout;
using std::endl;

void process()
{
	::srand(::time(nullptr));
	int number = ::rand() % 100;
	cout << ">> number : " << number << endl;
}

void test0()
{
	LRUCache cache(2);
	cache.set("1", "1");
	cache.set("2", "2");
	cout << cache.get("1") << endl;
	cache.set("3", "3");
	cout << cache.get("2") << endl;
	cache.set("4", "4");
	cout << cache.get("1") << endl;
	cout << cache.get("3") << endl;
	cout << cache.get("4") << endl;
}

void test1()
{
	wd::Timer timer(process, 3, 6);
	//start方法与stop方法应该运行在不同的线程
	//才能退出定时器
	wd::Thread thread(std::bind(&wd::Timer::start, &timer));
	thread.start();

	::sleep(22);

	timer.stop();
	thread.join();
}

void test2()
{
	CacheManager::getInstance()->init(4, "cache.dat");

	wd::TimerThread timer(std::bind(&CacheManager::periodicUpdateCaches,
									CacheManager::getInstance()),
						  3, 6);
	timer.start();
	::sleep(22);
	timer.stop();
}

int main(void)
{
	/* test0(); */
	/* test1(); */
	test2();
	return 0;
}
