#include <pthread.h>
#include <errno.h>
#include <iostream>
using std::cout;
using std::endl;

int gNumber = 100;
//线程局部存储
__thread int tNumber = 0;

void *threadFunc1(void *arg)
{
	cout << ">> 1 thread: gNumber: " << gNumber << endl;
	cout << ">> 1 thread: &gNumber:" << &gNumber << endl;
	//子线程在运行的过程中，能够直接拿到的
	tNumber = 10;
	cout << ">>> 1 thread: tNumber: " << tNumber << endl;
	cout << ">> 1 thread: &tNumber:" << &tNumber << endl;
	cout << ">> 1 thread: &errno: " << &errno << endl;

	return nullptr;
}

void *threadFunc2(void *arg)
{
	cout << ">> 2 thread: gNumber: " << gNumber << endl;
	cout << ">> 2 thread: &gNumber:" << &gNumber << endl;
	tNumber = 100;
	cout << ">>> 2 thread: tNumber: " << tNumber << endl;
	cout << ">> 2 thread: &tNumber:" << &tNumber << endl;
	cout << ">> 2 thread: &errno: " << &errno << endl;

	return nullptr;
}

int main(void)
{
	pthread_t pthid1, pthid2;
	cout << ">>> main thread: gNumber: " << gNumber << endl;
	cout << ">> main thread: &gNumber:" << &gNumber << endl;
	tNumber = 1;
	cout << ">>> main thread: tNumber: " << tNumber << endl;
	cout << ">> main thread: &tNumber:" << &tNumber << endl;
	cout << ">> main thread: &errno: " << &errno << endl;

	pthread_create(&pthid1, nullptr, threadFunc1, nullptr);
	pthread_create(&pthid2, nullptr, threadFunc2, nullptr);

	pthread_join(pthid1, nullptr);
	pthread_join(pthid2, nullptr);

	return 0;
}
