#ifndef __WD_TIMERTHREAD_H__
#define __WD_TIMERTHREAD_H__
#include "Timer.hpp"
#include "Thread.hpp"

namespace wd
{

	class TimerThread
	{
		using TimerCallback = std::function<void()>;

	public:
		TimerThread(TimerCallback &&cb, int initialTime, int intervalTime)
			: _timer(std::move(cb), initialTime, intervalTime), _thread(std::bind(&Timer::start, &_timer))
		{
		}

		void start()
		{
			_thread.start();
		}

		void stop()
		{
			_timer.stop();
			_thread.join();
		}

	private:
		Timer _timer;
		Thread _thread;
	};

} //end of namespace wd

#endif
