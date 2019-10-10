#include "Accepter.h"

//构造函数
Accepter::Accepter()
{
	m_Run = false;
	m_ServerPort = 0;
	m_Fd = -1;
	m_Epfd = -1;
	m_epollTimeout = 0;
	m_epollWait = 0;
	m_acceptFailed = 0;
	m_pushSuc = 0;
	m_pushFailed = 0;
}

//析构函数
Accepter::~Accepter()
{}

//初始化接口
int32_t Accepter::init()
{
	//读取服务端ip
	if (RET::SUC != Config::getCfg(NS_CONFIG::EM_CFGID_SERVER_IP, m_ServerIp))
	{
		std::cout<<"Accepter: Read Server Ip Failed!"<<std::endl;
		return RET::FAIL;	
	}

	//读取服务端port
	int32_t iValue = -1;
	if (RET::SUC != Config::getCfg(NS_CONFIG::EM_CFGID_SERVER_PORT, iValue))
	{
		std::cout<<"Accepter: Read Server Port Failed!"<<std::endl;
		return RET::FAIL;	
	}
	m_ServerPort = iValue;

	//初始化套接字
	if (RET::SUC != socketInit())
	{
		return RET::FAIL;
	}
	
	//epoll初始化
	if (RET::SUC != epollInit())
	{
		return RET::FAIL;
	}

	return RET::SUC;
}

int32_t Accepter::socketInit()
{
	if ((m_Fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout<<"Accepter: Socket Init Failed!"<<std::endl;
		return RET::FAIL;
	}

	//设置非阻塞
	if (RET::SUC != setNoBlock(m_Fd))
	{
		return RET::FAIL;
	}	

	int32_t opt = 1;
	setsockopt(m_Fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	struct sockaddr_in sock_addr;
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(m_ServerPort);
	sock_addr.sin_addr.s_addr = inet_addr(m_ServerIp.c_str());

	//bind绑定
	if ((bind(m_Fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr))) < 0)
	{
		std::cout<<"Accepter: Bind Failed!"<<std::endl;
		return RET::FAIL;
	}

	//监听
	if ((listen(m_Fd, NS_ACCEPTOR::LISTEN_NUM)) < 0)
	{
		std::cout<<"Accepter: Listen Failed!"<<std::endl;
		return RET::FAIL;
	}

	return RET::SUC;
}

int32_t Accepter::setNoBlock(int32_t fd)
{
	int32_t fd_flag = fcntl(fd, F_GETFL);
	if (fcntl(fd, F_SETFL, fd_flag|O_NONBLOCK) < 0)
	{
		std::cout<<"Accepter: fcntl Failed!"<<std::endl;
		return RET::FAIL;
	}

	return RET::SUC;
}

//epoll初始化
int32_t Accepter::epollInit()
{
	m_Epfd = epoll_create(NS_ACCEPTOR::EPOLL_CREATE_MAX);
	if (m_Epfd < 0)
	{
		return RET::FAIL;
	}

	ev.events = EPOLLIN|EPOLLET;
	ev.data.ptr = Alloc(m_Fd);
	if (epoll_ctl(m_Epfd, EPOLL_CTL_ADD, m_Fd, &ev) < 0)
	{
		m_epollCtlFailed++;
		return RET::FAIL;
	} 

	return RET::SUC;
}

//内存分配
NS_ACCEPTOR::epoll_buf *Accepter::Alloc(int32_t fd)
{
	NS_ACCEPTOR::epoll_buf *ret = nullptr;
	try
	{
		ret = new NS_ACCEPTOR::epoll_buf();
	}
	catch(std::bad_alloc)
	{
		return nullptr;
	}

	ret->fd = fd;
	return ret;
}

void Accepter::Free(NS_ACCEPTOR::epoll_buf *ptr)
{
	if (nullptr != ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

int32_t Accepter::start()
{
	//唤醒线程状态
	m_Run = true;

	//启动线程
	m_thread = std::thread(std::bind(&Accepter::process, this));

	return RET::SUC;
}

//线程处理函数
void Accepter::process()
{
	while (m_Run)
	{
		int32_t iRev = epoll_wait(m_Epfd, env, 32, 30000);
		switch (iRev)
		{
			case 0:
				m_epollTimeout++;
				break;
			case -1:
				m_epollWait++;
				break;
			default:
			{
				for (int32_t Index = 0; Index < iRev; Index++)
				{
					NS_ACCEPTOR::epoll_buf *buf = (NS_ACCEPTOR::epoll_buf*)env[Index].data.ptr;
					if (buf->fd == m_Fd && env[Index].events & EPOLLIN)
					{
						struct sockaddr_in client;
						socklen_t Len = sizeof(client);
						int32_t sock = accept(m_Fd, (struct sockaddr*)&client, &Len);
						while (sock > 0)
						{
							setNoBlock(sock);
							ev.events = EPOLLIN|EPOLLET;
							pEndfree = ev.data.ptr;
							ev.data.ptr = Alloc(sock);
							if (epoll_ctl(m_Epfd, EPOLL_CTL_ADD, sock, &ev) < 0)
							{
								m_epollCtlFailed++;
								break;
							}
							
							m_ClientIp = inet_ntoa(client.sin_addr);
							m_ClientPort = ntohs(client.sin_port);
						}

						if (0 > sock)
						{
							m_acceptFailed++;
						}
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
void Accepter::Read(NS_ACCEPTOR::epoll_buf *buf, struct epoll_event *ev_arr)
{
	int32_t res = -1;
	//读取http上行内容
	if ((res = read(buf->fd, buf->Buf, NS_ACCEPTOR::ACCEPT_SIZE)) > 0)
	{
		InputPacket *pInputPkt = nullptr;
		try {
			pInputPkt = new InputPacket();
		} catch(std::bad_alloc) {
			return;
		}
		pInputPkt->m_ClientIp = m_ClientIp;
		pInputPkt->m_ClientPort = m_ClientPort;

		try	{
			pInputPkt->pStr = (char*)_MEM_NEW_(res + 1);
		} catch(std::bad_alloc) {
			if (nullptr != pInputPkt) {
				delete pInputPkt;
				pInputPkt = nullptr;
			}
			return;
		}
		_MEM_CPY_(pInputPkt->pStr, buf->Buf, res);
		_MEM_ZERO_(pInputPkt->pStr, res + 1, res);
		pInputPkt->uLength = res;

		//push到主缓冲区
		if (RET::SUC != Processor::getInstance().pushData(pInputPkt))
		{
			m_pushFailed++;
			if (nullptr != pInputPkt) {
				delete pInputPkt;
				pInputPkt = nullptr;
			}
		}

		m_pushSuc++;
		fflush(stdout);
	}

	if (res == 0)
	{
		fflush(stdout);
		if (epoll_ctl(m_Epfd, EPOLL_CTL_DEL, buf->fd, nullptr) < 0)
		{
			m_epollCtlFailed++;
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
	if (epoll_ctl(m_Epfd, EPOLL_CTL_MOD, buf->fd, &e) < 0)
	{
		m_epollCtlFailed++;
	}

	return;
}

//给出http下行响应
void Accepter::Write(NS_ACCEPTOR::epoll_buf *buf)
{
	const char * temp ="HTTP/1.1 200 OK\r\n Content-Length :%s \r\n\r\n Hello world! ";
	int ret= sprintf(buf->Buf, "%s", temp);
	write(buf->fd, buf->Buf, ret);
	epoll_ctl(m_Epfd, EPOLL_CTL_DEL, buf->fd, nullptr);
	close(buf->fd);
	Free((NS_ACCEPTOR::epoll_buf*)ev.data.ptr);
	ev.data.ptr = nullptr;
	Free((NS_ACCEPTOR::epoll_buf*)pEndfree);
	pEndfree = nullptr;
}

/** 输出日志接口 **/
void Accepter::sprintfLogStream(std::string &log)
{
	log = "Accepter: EpollTimeout[" + std::to_string(m_epollTimeout)
			+ "] EpollWait[" + std::to_string(m_epollWait)
			+ "] AcceptFailed[" + std::to_string(m_acceptFailed)
			+ "] EpollCtlFailed[" + std::to_string(m_epollCtlFailed)
			+ "] PushSuc[" + std::to_string(m_pushSuc)
			+ "] PushFailed[" + std::to_string(m_pushFailed)
			+ "]";
}

/** 线程join **/
void Accepter::threadJoin()
{
	m_thread.join();
}
