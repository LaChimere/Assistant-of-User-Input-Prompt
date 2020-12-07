#ifndef __WD_THREADPOOL_H__
#define __WD_THREADPOOL_H__

#include "Thread.hpp"
#include "TaskQueue.hpp"

#include <vector>
#include <memory>
#include <functional>

using std::unique_ptr;
using std::vector;

namespace wd
{

	using Task = std::function<void()>;

	class Threadpool
	{
		friend class WorkerThread;

	public:
		Threadpool(size_t threadNum, size_t queSize)
			: _threadNum(threadNum), _queSize(queSize), _taskQue(_queSize), _isExit(false)
		{
			_threads.reserve(_threadNum);
		}

		~Threadpool();

		void start();
		void stop();

		void addTask(Task &&task);

	private:
		void threadFunc();
		Task getTask();

	private:
		size_t _threadNum;
		size_t _queSize;
		vector<unique_ptr<wd::Thread>> _threads;
		TaskQueue _taskQue;
		bool _isExit;
	};

} //end of namespace wd

#endif
