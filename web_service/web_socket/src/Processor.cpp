#include "Processor.h"

Processor::Processor()
{
	m_Run = false;
	m_ListenPort = 0;
	m_SockFd = -1;
	m_EpollFd = -1;
	m_EpollCtlFailed = 0;
	m_EpollTimeout = 0;
	m_AcceptFailed = 0;
	m_EpollWait = 0;
	m_WriteLogFailed = 0;
	pEndfree = nullptr;
}

Processor::~Processor()
{}

int32_t Processor::Init()
{
	//获取监听ip
	if (RET::SUC != Config::GetCfg(NS_CONFIG::EM_CFGID_SERVER_IP, m_ListenIp))
	{
		std::cout<<"Processor: Get ListenIp Failed!"<<std::endl; 
		return RET::FAIL;
	}

	//获取监听端口
	int32_t iValue = -1;
	if (RET::SUC != Config::GetCfg(NS_CONFIG::EM_CFGID_SERVER_PORT, iValue))
	{
		std::cout<<"Processor: Get ListenPort Failed!"<<std::endl; 
		return RET::FAIL;
	}
	m_ListenPort = iValue;

	//套接字初始化
	if (RET::SUC != SocketInit())
	{
		std::cout<<"Processor: Create Socket Failed!"<<std::endl;
		return RET::FAIL;
	}

	//初始化epoll机制
	if (RET::SUC != EpollInit())
	{
		std::cout<<"Processor: Epoll Init Failed!"<<std::endl;
		return RET::FAIL;
	}

	return RET::SUC;
}

int32_t Processor::SocketInit()
{
	//建立套接字
	m_SockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == m_SockFd)
	{
		return RET::FAIL;
	}

	//设置非阻塞
	if (RET::SUC != SetNoBlock(m_SockFd))
	{
		return RET::FAIL;
	}	

	int32_t opt = 1;
	setsockopt(m_SockFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	//初始化服务端ip
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(m_ListenIp.c_str());
	server_addr.sin_port = htons(m_ListenPort);

	//绑定
	if(-1 == bind(m_SockFd, (struct sockaddr *)(&server_addr), sizeof(server_addr))) {
		return RET::FAIL;
	}

	//监听
	if(-1 == listen(m_SockFd, NS_PROCESSOR::LISTEN_MAXNUM)) {
		return -1;
	}

	return RET::SUC;
}

int32_t Processor::EpollInit()
{
	m_EpollFd = epoll_create(NS_PROCESSOR::EPOLL_MAX_EVENT);
	if (m_EpollFd < 0)
	{
		return RET::FAIL;
	}

	ev.events = EPOLLIN|EPOLLET;
	ev.data.ptr = Alloc(m_SockFd);
	if (epoll_ctl(m_EpollFd, EPOLL_CTL_ADD, m_SockFd, &ev) < 0)
	{
		m_EpollCtlFailed++;
		return RET::FAIL;
	} 

	return RET::SUC;
}
//内存分配
NS_PROCESSOR::epoll_buf *Processor::Alloc(int32_t fd)
{
	NS_PROCESSOR::epoll_buf *ret = nullptr;
	try
	{
		ret = new NS_PROCESSOR::epoll_buf();
	}
	catch(std::bad_alloc)
	{
		return nullptr;
	}

	ret->fd = fd;
	return ret;
}

void Processor::Free(NS_PROCESSOR::epoll_buf *ptr)
{
	if (nullptr != ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

int32_t Processor::Start()
{
	m_Run = true;
	Accept();
	return RET::SUC;
}

//线程处理函数
void Processor::Accept()
{
	while (m_Run)
	{
		//写日志
		if (RET::SUC != WriteLog())
		{
			m_WriteLogFailed++;
		}

		int32_t iRev = epoll_wait(m_EpollFd, env, 32, 10000);
		switch (iRev)
		{
			case 0:
				m_EpollTimeout++;
				break;
			case -1:
				m_EpollWait++;
				break;
			default:
			{
				for (int32_t Index = 0; Index < iRev; Index++)
				{
					NS_PROCESSOR::epoll_buf *buf = (NS_PROCESSOR::epoll_buf*)env[Index].data.ptr;
					if (buf->fd == m_SockFd && env[Index].events & EPOLLIN)
					{
						struct sockaddr_in client;
						socklen_t Len = sizeof(client);
						int32_t sock = accept(m_SockFd, (struct sockaddr*)&client, &Len);
						while (sock > 0)
						{
							SetNoBlock(sock);
							ev.events = EPOLLIN|EPOLLET;
							pEndfree = ev.data.ptr;
							ev.data.ptr = Alloc(sock);
							if (epoll_ctl(m_EpollFd, EPOLL_CTL_ADD, sock, &ev) < 0)
							{
								m_EpollCtlFailed++;
								break;
							}
						}

						if (0 > sock)
						{
							m_AcceptFailed++;
						}
						std::cout<<"accept!"<<std::endl;
						std::cout<<"ip:"<<inet_ntoa(client.sin_addr)<<std::endl;
						continue;
					}
					else if (env[Index].events & EPOLLIN)
					{
						Read(buf, &env[Index]);
					}
					else
					{
						Write(buf);
					}

				}//end for
				break;
			}
		}//end switch
	}//end while
}

//接收http请求
void Processor::Read(NS_PROCESSOR::epoll_buf *buf, struct epoll_event *ev_arr)
{
	int32_t res = -1;
	//读取http上行内容
	if ((res = read(buf->fd, buf->Buf, NS_PROCESSOR::ACCEPT_SIZE)) > 0)
	{
		std::cout<<"readf: "<<buf->Buf<<std::endl;
		fflush(stdout);
	}

	if (res == 0)
	{
		fflush(stdout);
		if (epoll_ctl(m_EpollFd, EPOLL_CTL_DEL, buf->fd, nullptr) < 0)
		{
			m_EpollCtlFailed++;
			return;
		}
		close(buf->fd);
	}

	if(res < 0 && errno != EAGAIN)
	{
		return;
	}

	struct epoll_event e;
    e.events = ev_arr->events|EPOLLOUT; //这步是为了将关心的事件改为即关心读又关心写 
    e.data.ptr = buf;
	if (epoll_ctl(m_EpollFd, EPOLL_CTL_MOD, buf->fd, &e) < 0)
	{
		m_EpollCtlFailed++;
	}

	return;
}

//给出http下行响应
void Processor::Write(NS_PROCESSOR::epoll_buf *buf)
{
	const char * temp ="HTTP/1.1 200 OK\r\n Content-Length :%s \r\n\r\n Hello world! ";
	int ret= sprintf(buf->Buf, "%s", temp);
	write(buf->fd, buf->Buf, ret);
	epoll_ctl(m_EpollFd, EPOLL_CTL_DEL, buf->fd, nullptr);
	close(buf->fd);
	Free((NS_PROCESSOR::epoll_buf*)ev.data.ptr);
	Free((NS_PROCESSOR::epoll_buf*)pEndfree);
}

int32_t Processor::SetNoBlock(int32_t fd)
{
	int32_t fd_flag = fcntl(fd, F_GETFL);
	if (fcntl(fd, F_SETFL, fd_flag|O_NONBLOCK) < 0)
	{
		std::cout<<"Processor: fcntl Failed!"<<std::endl;
		return RET::FAIL;
	}

	return RET::SUC;
}

int32_t Processor::WriteLog()
{
	uint64_t Nowtime = Timer::GetLocalTime();
	if (Nowtime - m_LastLogTime < NS_PROCESSOR::LOG_TIMEOUT)
	{
		return RET::SUC;
	}

	//建立io
	std::ofstream io;
	io.open("/web/data/logs/websocket.log", std::ios::app);

	//输出日志头
	std::string header;
	time_t nowtime = time(NULL);
	struct tm *local = localtime(&nowtime);
	header = "WebSocket[" + std::to_string(getpid()) + "] " 
			+ std::to_string(local->tm_year + 1900) + "/"
			+ std::to_string(local->tm_mon + 1) + "/" 
			+ std::to_string(local->tm_mday) + " " 
			+ std::to_string(local->tm_hour) + ":"
			+ std::to_string(local->tm_min) + ":"
			+ std::to_string(local->tm_sec);
	io<<header<<std::endl;

	//输出处理模块日志
	std::string pro;
	if (RET::SUC != SprintfLogStream(pro))
	{
		return RET::FAIL;
	}
	io<<pro<<std::endl;

	io<<std::endl<<std::endl;
	m_LastLogTime = Nowtime;
	return RET::SUC;
}

int32_t Processor::SprintfLogStream(std::string &LogStream)
{
	LogStream = "Processor: EpollCtlFailed[" + std::to_string(m_EpollCtlFailed)
			+ "] EpollTimeout[" + std::to_string(m_EpollTimeout) + "] EpollWait["
			+ std::to_string(m_EpollWait) + "] AcceptFailed[" 
			+ std::to_string(m_AcceptFailed) + "] WriteLogFailed["
			+ std::to_string(m_WriteLogFailed) + "]";

	return RET::SUC;
}
