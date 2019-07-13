#ifndef _PARSERBASE_H_
#define _PARSERBASE_H_

#include "InputPacket.h"

class ParserBase
{
	public:
		/**
		 * @brief 构造函数
		 */
		ParserBase(){}

		/**
		 * @brief 析构函数
		 */
		virtual ~ParserBase(){}

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
		virtual int32_t Init()
		{
			return RET::SUC;
		}

		/**
		 * @brief 解析开始接口
		 *
		 * @prame InputPkt 包体
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		virtual int32_t Start(InputPacket *&InputPkt)
		{
			return RET::SUC;
		}
};

#endif
