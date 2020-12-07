#include "TcpServer.h"
#include "Threadpool.hpp"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
using std::cout;
using std::endl;
using std::string;

wd::Threadpool *pthreadpool = nullptr;

//该变量的设置不能太过随意
__thread int idx = 0;

#if 1
class MyTask
{
public:
	MyTask(const string &msg,
		   const wd::TcpConnectionPtr &conn)
		: _msg(msg), _conn(conn)
	{
	}

	//运行在线程池的某一个子线程中
	void process()
	{
		//decode
		//compute
		//encode
		//_msg;
		//0. 先从缓存中去查找
		//只需要传递一个具体的Cache的下标
		//在CacheManager中是一个vector<Cache>
		//pthread_self();//pthread_t
		cout << "threadName: " << wd::current_thread::threadName << endl;

		/* Cache & cache = CacheManager::getCache(str2int(wd::current_thread::threadName)); */

		//1. 执行查询模块

		//2. 发送结果
		string response = _msg; //要返回给客户端的消息
		//_conn->send(response);//由线程池的线程(计算线程)完成数据的发送,在设计上来说，是不合理的
		//数据发送的工作要交还给IO线程(Reactor所在的线程)完成
		//将send的函数的执行延迟到IO线程去进行操作

		_conn->sendInLoop(response);
	}

private:
	string _msg;
	wd::TcpConnectionPtr _conn;
};
#endif

//回调函数体现了扩展性
void onConnection(const wd::TcpConnectionPtr &conn)
{
	cout << conn->toString() << " has connected!" << endl;
	conn->send("welcome to server.");
}

void onMessage(const wd::TcpConnectionPtr &conn)
{
	cout << "onMessage...." << endl;
	string msg = conn->receive();
	cout << ">> receive msg from client: " << msg << endl;
	//业务逻辑的处理要交给线程池处理
	//decode
	//compute
	//encode
	//::sleep(2);//碰到需要长时间的处理时，响应速度会降下来
	//conn->send(msg);

	MyTask task(msg, conn);

	//拿到线程池之后，就将该任务交给线程池去执行
	pthreadpool->addTask(std::bind(&MyTask::process, task));
}

void onClose(const wd::TcpConnectionPtr &conn)
{
	cout << "onClose...." << endl;
	cout << conn->toString() << " has closed!" << endl;
}

#if 0
class MyServer
{
public:

	void onConnection(const wd::TcpConnectionPtr & conn);
	void onMessage(const wd::TcpConnectionPtr & conn)
	{
		_threadpool.addTask();
	}

	void onClose(const wd::TcpConnectionPtr & conn);

private:
	wd::Threadpool _threadpool;
	wd::TcpServer _server;
};
#endif

int main(void)
{
	wd::Threadpool threadpool(4, 10);
	threadpool.start();
	pthreadpool = &threadpool;

	wd::TcpServer server("192.168.30.128", 8888);

	server.setConnectionCallback(onConnection);
	server.setMessageCallback(onMessage);
	server.setCloseCallback(onClose);

	server.start();

	return 0;
}
