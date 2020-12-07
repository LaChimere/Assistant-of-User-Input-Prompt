#include "Timer.hpp"

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <poll.h>
#include <sys/timerfd.h>

#include <iostream>
using std::cout;
using std::endl;

namespace wd
{
	Timer::Timer(TimerCallback &&cb, int initialTime, int periodicTime)
		: _fd(createTimerfd()), _cb(std::move(cb)), _initialTime(initialTime), _periodicTime(periodicTime), _isStarted(false)
	{
	}

	Timer::~Timer()
	{
		if (_isStarted)
		{
			stop();
		}
	}

	void Timer::start()
	{
		if (!_isStarted)
			_isStarted = true;

		struct pollfd pfd;
		pfd.fd = _fd;
		pfd.events = POLLIN;
		setTimerfd(_initialTime, _periodicTime);

		while (_isStarted)
		{
			int nready;
			do
			{
				//水平触发: 只要不处理，当poll返回时，
				//该文件描述符一直被触发
				nready = ::poll(&pfd, 1, 5000);
			} while (nready == -1 && errno == EINTR);

			if (nready == -1)
			{
				perror("poll");
				return;
			}
			else if (nready == 0)
			{
				printf(">> poll timeout\n");
			}
			else
			{
				if (pfd.revents & POLLIN)
				{
					//处理定时器
					handleRead();
					if (_cb)
						_cb();
				}
			}
		}
	}

	void Timer::stop()
	{
		setTimerfd(0, 0);
		if (_isStarted)
			_isStarted = false;
	}

	int Timer::createTimerfd()
	{
		int fd = ::timerfd_create(CLOCK_REALTIME, 0);
		if (fd < 0)
		{
			perror("timerfd_create");
		}
		return fd;
	}

	void Timer::setTimerfd(int initialTime, int periodicTime)
	{
		struct itimerspec value;
		value.it_value.tv_sec = initialTime;
		value.it_value.tv_nsec = 0;
		value.it_interval.tv_sec = periodicTime;
		value.it_interval.tv_nsec = 0;

		int ret = timerfd_settime(_fd, 0, &value, nullptr);
		if (ret < 0)
		{
			perror("timerfd_settime");
		}
	}

	void Timer::handleRead()
	{
		uint64_t howmany;
		int ret = ::read(_fd, &howmany, sizeof(howmany));
		if (ret != sizeof(howmany))
		{
			perror("read");
		}
	}

} //end of namespace wd
