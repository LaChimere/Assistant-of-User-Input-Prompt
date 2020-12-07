#include "Threadpool.hpp"
#include <unistd.h>
#include <stdio.h>

namespace wd
{

	Threadpool::~Threadpool()
	{
		if (!_isExit)
		{
			stop();
		}
	}

	void Threadpool::start()
	{
		for (size_t idx = 0; idx != _threadNum; ++idx)
		{
			unique_ptr<Thread> up(new Thread(
				std::bind(&Threadpool::threadFunc, this), std::to_string(idx)));
			_threads.push_back(std::move(up));
		}

		for (auto &pthread : _threads)
		{
			pthread->start();
		}
	}

	void Threadpool::stop()
	{
		if (!_isExit)
		{

			while (!_taskQue.empty())
			{
				/* ::usleep(1000); */
				cout << ">>> stop :  _taskQue.empty(): "
					 << _taskQue.empty() << endl;
				sleep(1);
			}
			_isExit = true;
			_taskQue.wakeup();

			cout << "---stop enter for loop --" << endl;
			for (auto &pthread : _threads)
			{
				pthread->join(); //工作线程无法退出
			}
			cout << "---stop end---" << endl;
		}
	}

	void Threadpool::addTask(Task &&task)
	{
		_taskQue.push(task);
	}

	Task Threadpool::getTask()
	{
		return _taskQue.pop();
	}

	//每一个工作线程都会做的事情
	void Threadpool::threadFunc()
	{
		while (!_isExit)
		{
			printf("---threadpool: threadFunc---\n");
			Task task = getTask();
			if (task)
			{
				//任务执行的速度太快，快到_isExit被设置为true
				//之前，工作线程就已经进入下一次循环，阻塞到了
				//getTask方法之上
				task();
			}
		}
	}

} //end of namespace wd
