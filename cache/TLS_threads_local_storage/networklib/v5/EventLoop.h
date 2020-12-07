#ifndef __WD_EVENTLOOP_H__
#define __WD_EVENTLOOP_H__

#include "MutexLock.hpp"
#include <sys/epoll.h>

#include <map>
#include <memory>
#include <vector>
#include <functional>
using std::map;
using std::shared_ptr;
using std::vector;

namespace wd
{
	class Acceptor;
	class TcpConnection;

	class EventLoop
	{
	public:
		using Functor = std::function<void()>;
		using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
		using TcpConnectionCallback = std::function<void(const TcpConnectionPtr &)>;
		EventLoop(Acceptor &acceptor);
		void loop();
		void unloop();
		void setConnectionCallback(TcpConnectionCallback &&cb)
		{
			_onConnection = std::move(cb);
		}
		void setMessageCallback(TcpConnectionCallback &&cb)
		{
			_onMessage = std::move(cb);
		}
		void setCloseCallback(TcpConnectionCallback &&cb)
		{
			_onClose = std::move(cb);
		}

		void runInLoop(Functor &&cb);

	private:
		void waitEpollFd();
		void handleNewConnection();
		void handleMessage(int fd);
		int createEpollFd();

		void addEpollFdRead(int fd);
		void delEpollFdRead(int fd);
		bool isConnectionClosed(int fd);

		int createEventfd();
		void handleRead();
		void wakeup();
		void doPendingFunctors();

	private:
		int _efd;
		int _eventfd;
		Acceptor &_acceptor;
		vector<struct epoll_event> _eventList;
		map<int, TcpConnectionPtr> _conns;
		bool _isLooping;

		vector<Functor> _pendingFunctors; //临界资源
		MutexLock _mutex;

		TcpConnectionCallback _onConnection;
		TcpConnectionCallback _onMessage;
		TcpConnectionCallback _onClose;
	};

} //end of namespace wd

#endif
