#ifndef __WD_TIMER_H__
#define __WD_TIMER_H__

#include <functional>

namespace wd
{

	class Timer
	{
	public:
		using TimerCallback = std::function<void()>;

		Timer(TimerCallback &&, int initialTime, int periodicTime);
		~Timer();

		//开启定时器
		void start();
		//停止定时器
		void stop();

	private:
		int createTimerfd();
		void setTimerfd(int initialTime, int periodicTime);
		void handleRead();

	private:
		int _fd;
		int _initialTime;
		int _periodicTime;
		TimerCallback _cb;
		bool _isStarted;
	};

} //end of namespace wd

#endif
