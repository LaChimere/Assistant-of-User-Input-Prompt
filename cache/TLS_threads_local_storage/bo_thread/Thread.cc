#include "Thread.hpp"
#include <iostream>
using std::cout;
using std::endl;

namespace wd
{

	namespace current_thread
	{
		__thread const char *threadName = "wd thread";
	}

	struct ThreadData
	{
		string _name;
		ThreadCallback _cb;
		ThreadData(const string &name, const ThreadCallback &cb)
			: _name(name), _cb(cb)
		{
		}

		void runInThread()
		{
			current_thread::threadName = (_name == string() ? "wd thread" : _name.c_str());
			if (_cb)
				_cb();
		}
	};

	Thread::~Thread()
	{
		if (_isRunning)
		{
			pthread_detach(_pthid);
		}
	}

	void Thread::start() //开启线程 ==> pthread_create
	{
		ThreadData *pdata = new ThreadData(_name, _cb);
		pthread_create(&_pthid, nullptr, threadFunc, pdata);
		_isRunning = true;
	}

	void Thread::join() //回收线程
	{
		if (_isRunning)
		{
			pthread_join(_pthid, nullptr);
			_isRunning = false;
		}
	}

	void *Thread::threadFunc(void *arg)
	{
		ThreadData *pdata = static_cast<ThreadData *>(arg);
		if (pdata)
		{
			pdata->runInThread();
		}
		return nullptr;
	}

} //end of namespace wd
