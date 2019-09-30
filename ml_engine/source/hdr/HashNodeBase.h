#ifndef _HASHNODEBASE_H_
#define _HASHNODEBASE_H_

#include "RetCodeDefine.h"
#include "MemoryDefine.h"

class HashNodeBase
{
	public:
		/**
		 * @brief 构造函数
		 */
		HashNodeBase()
		{}

		/**
		 * @brief 析构函数
		 */
		virtual ~HashNodeBase()
		{}

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

};

#endif
