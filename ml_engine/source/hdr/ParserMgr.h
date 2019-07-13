#ifndef _PARSERMGR_H
#define _PARSERMGR_H

#include "InputPacket.h"
#include "Protocol.h"
#include "Eth2Parser.h"

class ParserMgr
{
	public:
		/**
		 * @brief 析构函数
		 */
		~ParserMgr();

		/**
		 * @brief 去除拷贝函数
		 */
		ParserMgr(ParserMgr const&) = delete;

		/**
		 * @brief 去除重载赋值
		 */
		void operator=(ParserMgr const&) = delete;
		
		/**
		 * @brief 获取单例
		 *
		 * @return 返回单例
		 */
		static ParserMgr &GetInstance()
		{
			static ParserMgr Instance;
			return Instance;
		}

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
		int32_t Init();

		/**
		 * @brief 获取解析器接口
		 */
		ParserBase *GetParser(uint8_t uProType);

	private:
		/**
		 * @brief 构造函数
		 */
		ParserMgr();

	private:
		/**
		 * @brief 解析器数组
		 */
		ParserBase **_Parser;
};

#endif
