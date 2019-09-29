#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_

#include "IPDRuleMgr.h"
#include "HttpParser.h"
#include "Accepter.h"
#include <unistd.h>
#include <queue>
#include <thread>

#define QUEUE_MAX_SIZE 1000

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
		static Processor &getInstance()
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
		int32_t init();

		/**
		 * @brief 启动接口
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t start();

		/**
		 * @brief 线程join
		 */
		void threadJoin();

		/**
		 * @brief push数据
		 *
		 * @prame pInputPkt 数据包
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t pushData(InputPacket *pInputPkt);

		/**
		 * @brief 封装日志流
		 *
		 * @prame log 日志流
		 */
		void sprintfLogStream(std::string &log);

	private:
		/**
		 * @brief 构造函数
		 */
		Processor();

		/**
		 * @brief 线程处理接口
		 */
		void process();

		/**
		 * @brief 日志接口
		 */
		void writeLog();

		/**
		 * @brief 内存检测接口
		 */
		void memCheck();

	private:
		/**
		 * @brief 线程状态
		 */
		bool m_run;

		/**
		 * @brief 丢列接收最大个数
		 */
		uint32_t m_queueMaxSize;

		/**
		 * @brief 数据包为空个数(日志信息)
		 */
		uint32_t m_pktNull;

		/**
 		 * @brief 解析失败次数(日志信息)
 		 */
		uint32_t m_parserFailed;

		/**
 		 * @brief 数据库连接失败次数(日志信息)
 		 */
		uint32_t m_connectDbFailed;

		/**
		 * @brief 数据库查询失败次数(日志信息)
		 */
		uint32_t m_execQueryFailed;

		/**
		 * @brief 处理包总数(日志信息)
		 */
		uint32_t m_proTotal;

		/**
		 * @brief 处理包成功个数(日志信息)
		 */
		uint32_t m_proSuc;

		/**
		 * @brief 丢包个数
		 */
		uint32_t m_deletePkt;

		/**
		 * @brief 数据缓冲区
		 */
		std::queue<InputPacket*> _queue;

		/**
		 * http解析器对象
		 */
		HttpParser _parser;

		/**
		 * @brief 线程对象
		 */
		std::thread m_thread;
};

#endif
