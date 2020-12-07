#ifndef __WD_THREAD_H__
#define __WD_THREAD_H__

#include "Noncopyable.hpp"

#include <pthread.h>

#include <iostream>
#include <string>
#include <functional>
using std::cout;
using std::endl;
using std::string;

namespace wd
{

	//代表的是线程的名字
	namespace current_thread
	{
		extern __thread const char *threadName;
	}

	using ThreadCallback = std::function<void()>;

	class Thread
		: Noncopyable
	{
	public:
		Thread(ThreadCallback &&cb, const string &name = string())
			: _pthid(0), _name(name), _isRunning(false), _cb(std::move(cb))
		{
			cout << "Thread(ThreadCallback &&)" << endl;
		}

		~Thread();

		void start(); //开启线程 ==> pthread_create
		void join();  //回收线程

	private:
		static void *threadFunc(void *arg); //线程函数执行体

	private:
		pthread_t _pthid; // 线程id
		string _name;
		bool _isRunning;
		ThreadCallback _cb;
	};
} //end of namespace wd

#endif
