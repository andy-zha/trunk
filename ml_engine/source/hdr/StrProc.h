#ifndef _STRPROC_H_
#define _STRPROC_H_

#include "RetCodeDefine.h"
#include "MemoryDefine.h"
#include <string>

class StrProc
{
	public:
		/**
		 * @brief 构造函数
		 */
		StrProc();

		/**
		 * @brief 析构函数
		 */
		~StrProc();

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
		 * @brief url解码
		 *
		 * @prame src 源字符串 
		 *
		 * @return 目的字符串
		 */
		static std::string UrlDecode(const std::string &src);

		/**
		 * @brief url加码
		 *
		 * @prame src 源字符串 
		 *
		 * @return 目的字符串 
		 */
		static std::string UrlEncode(const std::string &src);

		/**
		 * @brief 转16进制
		 *
		 * @prame ch 字符
		 *
		 * @return 16进制字符
		 */
		static char toHex(char ch);

	private:
		/**
		 * @brief 十六进制数组
		 */
		static char HEX2DEC[256];
};

#endif
