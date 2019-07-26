#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_

#include "RetCodeDefine.h"
#include "MemoryDefine.h"
#include "InputPacket.h"
#include "HttpParser.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <dmalloc.h>
#include <thread>

namespace NS_ACCEPTOR
{
	//监听数量
	static const uint32_t LISTEN_NUM = 1024;
	//EPOLL创建最大数
	static const uint32_t EPOLL_CREATE_MAX = 256;
	//接收字节数
	static const uint32_t ACCEPT_SIZE = 4096;

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
	 		char Buf[NS_ACCEPTOR::ACCEPT_SIZE];
	}epoll_buf;
}

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
		 * @brief 套接字初始化
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t SocketInit();

		/**
		 * @brief 设置非阻塞状态
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t SetNoBlock(int32_t fd);

		/**
		 * @brief Epoll初始化		 *
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t EpollInit();

		/**
		 * @brief 内存分配
		 *
		 * @return 指针
		 */
		NS_ACCEPTOR::epoll_buf *Alloc(int32_t fd);

		/**
		 * @brief 内存释放
		 *
		 * @return 指针
		 */
		void Free(NS_ACCEPTOR::epoll_buf *ptr);

		/**
 		 * @brief 线程处理函数
 		 */
		void Accept();

		/**
 		 * @brief 读取
 		 *
 		 * @prame buf ev_arr
 		 */
		void Read(NS_ACCEPTOR::epoll_buf *buf, struct epoll_event *ev_arr, InputPacket *pInputPkt);

		/**
 		 * @brief 响应
 		 *
 		 * @prame buf ev_arr
 		 */
		void Write(NS_ACCEPTOR::epoll_buf *buf);

#ifdef _MEMCHECK_
		/**
 		 * @brief 内存泄漏检查
 		 */
		void MemCheck();
#endif

	private:
		/**
		 * @brief http解析器
		 */
		HttpParser _Parser;

		/**
 		 * @brief 线程运行状态
 		 */
		bool m_Run;

		/**
 		 * @brief 服务端Port
 		 */
		uint16_t m_ServerPort;

		/**
 	     * @brief 服务端Ip
 	     */
		std::string m_ServerIp;

		/**
 		 * @brief 套接字
 		 */
		int32_t m_Fd;

		/**
 		 * @brief epoll创建fd
 		 */
		int32_t m_Epfd;

		/**
		 * @brief epoll事件
		 */
		struct epoll_event ev;

		/**
		 * @brief epoll事件数组
		 */
		struct epoll_event env[32];

		/**
		 * @brief 托管内存
		 */
		void *pEndfree;

		/**
		 * @brief epoll超时次数
		 */
		uint32_t m_EpollTimeout;

		/**
		 * @brief epoll等待次数
		 */
		uint32_t m_EpollWait;

		/**
		 * @brief 连接失败次数
		 */
		uint32_t m_AcceptFailed;

		/**
		 * @brief epollctl失败次数
		 */
		uint32_t m_EpollCtlFailed;
};

#endif
