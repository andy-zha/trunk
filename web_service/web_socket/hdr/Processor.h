#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <fstream>
#include "Config.h"
#include "Timer.h"

namespace NS_PROCESSOR
{
	//监听最大数量
	static const uint32_t LISTEN_MAXNUM = 1024;
	//epoll事件最大数量
	static const uint32_t EPOLL_MAX_EVENT = 256;
	//接收最大字节数
	static const uint32_t ACCEPT_SIZE = 1514;
	//日志刷新间隔
	static const uint32_t LOG_TIMEOUT = 30;

	//epoll buf
	typedef struct epoll_buf
	{
		public:
			epoll_buf()
			{}

			~epoll_buf()
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

		public:
			int32_t fd;
	 		char Buf[NS_PROCESSOR::ACCEPT_SIZE];
	}epoll_buf;
}

class Processor
{
	public:
		/*
		 * @brief 析构函数
		 */
		~Processor();

		/**
		 * @brief 去除拷贝构造函数
		 */
		Processor(Processor const&) = delete;

		/**
		 * @brief 去除重载=
		 */
		void operator=(Processor const&) = delete;
	
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
		 * @brief 获取单例
		 *
		 * @return 返回单例
		 */
		static Processor& GetInstance()
		{
			static Processor Instance;
			return Instance;
		}

		/**
		 * @brief 初始化接口
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t Init();

		/**
		 * @brief 启动接口
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t Start();

	private:
		/**
		 * @brief 构造函数
		 */
		Processor();

		/**
		 * @brief 创建套接字函数
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t SocketInit();

		/**
		 * @brief 创建epoll事件
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t EpollInit();

		/**
		 * @brief 内存分配
		 *
		 * @return 指针
		 */
		NS_PROCESSOR::epoll_buf *Alloc(int32_t fd);

		/**
		 * @brief 内存释放
		 *
		 * @return 指针
		 */
		void Free(NS_PROCESSOR::epoll_buf *ptr);

		/**
		 * @brief 处理接口 
		 */
		void Accept();

		/**
		 * @brief 读取信息
		 *
		 * @prame buf 发送段信息
		 */
		void Read(NS_PROCESSOR::epoll_buf *buf, struct epoll_event *ev_arr);

		/**
		 * @brief 响应信息
		 *
		 * @prame buf 发送段信息
		 */
		void Write(NS_PROCESSOR::epoll_buf *buf);

		/**
		 * @brief 设置非阻塞状态
		 *
		 * @prame fd 套接字
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t SetNoBlock(int32_t fd);

		/**
		 * @brief 写日志 
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t WriteLog();

		/**
		 * @brief 拼接日志流
		 *
		 * @prame LogStream 日志流
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t SprintfLogStream(std::string &LogStream);

	private:
		/*
		 * @brief 启动状态开关
		 */
		bool m_Run;

		/**
		 * @brief 监听端口
		 */
		uint32_t m_ListenPort;

		/**
		 * @brief 套接字
		 */
		int32_t m_SockFd;

		/**
		 * @brief epoll套接字
		 */
		int32_t m_EpollFd;

		/**
		 * @brief epoll控制的失败次数（日志信息）
		 */
		uint32_t m_EpollCtlFailed;

		/**
		 * @brief epoll超时次数（日志信息）
		 */
		uint32_t m_EpollTimeout;

		/**
		 * @brief 接收失败次数（日志信息）
		 */
		uint32_t m_AcceptFailed;

		/**
		 * @brief epoll等待次数（日志信息）
		 */
		uint32_t m_EpollWait;

		/**
		 * @brief 写日志失败次数（日志信息）
		 */
		uint32_t m_WriteLogFailed;

		/**
		 * @brief 日志最后活跃时间
		 */
		uint64_t m_LastLogTime;

		/**
		 * @brief
		 */
		void *pEndfree;

		/**
		 * @brief 监听ip
		 */
		std::string m_ListenIp;

		/**
		 * @brief epoll事件
		 */
		struct epoll_event ev;

		/**
		 * @brief epoll事件数组
		 */
		struct epoll_event env[32];
};

#endif
