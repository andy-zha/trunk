#ifndef _SERVICEBASE_H_
#define _SERVICEBASE_H_

#include "MemoryDefine.h"
#include "RetCodeDefine.h"

class ServiceBase
{
	public:
		/**
		 * @brief 构造函数
		 */
		ServiceBase()
		{}

		/*
		 * @brief 析构函数
		 */
		virtual ~ServiceBase()
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

		/**
		 * @brief 初始化接口
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		virtual int32_t Init();

		/**
		 * @brief 启动接口
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		virtual int32_t Start();

};

#endif
 
