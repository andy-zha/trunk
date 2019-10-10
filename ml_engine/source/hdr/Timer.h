#ifndef _TIMER_H_
#define _TIMER_H_

#include "MemoryDefine.h"
#include "RetCodeDefine.h"
#include <time.h>

class Timer
{
	public:
		/**
		 * @brief 构造函数
		 */
		Timer();

		/**
		 * @brief 析构函数
		 */
   		~Timer();

		/**
		 * @brief 重载 内存分配
		 *
		 * @prame size 内存大小
		 *
		 * @return 内存地址
		 */
		static void * operator new(size_t size)
		{
			void *p = (void*)_MEM_NEW_(size);
			return p;
		}

		/**
		 * @brief 重载 内存释放
		 *
		 * @prame p 释放地址
		 */
		static void operator delete(void *p)
		{
			_MEM_DEL_(p);
		}
		
		/**
 		 * @brief 获取当前时间(低精度)
 		 * 
 		 * @return 当前时间
 		 */
		static uint64_t getLocalTime();
};

#endif
