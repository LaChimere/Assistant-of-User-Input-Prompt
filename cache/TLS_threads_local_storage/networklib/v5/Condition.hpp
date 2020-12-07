#ifndef __WD_CONDITION_H__
#define __WD_CONDITION_H__

//#include "test.hh"
#include "Noncopyable.hpp"
/* #include "MutexLock.hpp"    //1. 自定义头文件 */

#include <pthread.h> //2. C头文件

#include <iostream> //3. C++头文件

//#include <log4cpp/Category.hh> //4. 第三方头文件

using std::cout;
using std::endl;

namespace wd
{

	class MutexLock; //类的前向声明

	class Condition : Noncopyable
	{
	public:
		explicit Condition(MutexLock &mutex);
		~Condition();

		void wait();
		void notify();
		void notifyAll();

	private:
		pthread_cond_t _cond;
		MutexLock &_mutex;
	};

} //end of namespace wd

#endif
