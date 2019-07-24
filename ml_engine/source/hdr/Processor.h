#ifndef _PROCESSOR_H
#define _PROCESSOR_H

#include "IPDRuleMgr.h"
#include "HttpParser.h"

#ifdef _READ_PACKET_
#include "InputPacket.h"
#include "ParserMgr.h"
#endif

class Processor
{
	public:
		/**
		 * @brief 析构函数
		 */
		~Processor();

		/**
		 * @brief 去除拷贝函数
		 */
		Processor(Processor const&) = delete;

		/**
		 * @brief 去除重载赋值
		 */
		void operator=(Processor const&) = delete;
		
		/**
		 * @brief 获取单例
		 *
		 * @return 返回单例
		 */
		static Processor &GetInstance()
		{
			static Processor Instance;
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
		 * @brief 处理函数
		 */
		void Process(InputPacket *pInputPkt);

	private:
		/**
		 * @brief 构造函数
		 */
		Processor();

		/**
		 * @brief http解析器
		 */
		HttpParser _Parser;

		///**
 		// * @brief 主缓冲区
 		// */
		//std::queue<InputPacket*> _Queue;
		
		/**
		 * @brief 解析失败次数（日志信息)
		 */
		uint32_t m_ParserFailed;
};

#endif
