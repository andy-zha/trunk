#include "Acceptor.h"

//构造函数
Acceptor::Acceptor()
{
	m_Run = false;
	m_ServerPort = 0;
	m_Fd = -1;
	m_Epfd = -1;
	m_EpollTimeout = 0;
	m_EpollWait = 0;
	m_AcceptFailed = 0;
}

//析构函数
Acceptor::~Acceptor()
{}

//初始化接口
int32_t Acceptor::Init()
{
	m_ServerIp = "192.168.125.130";
	m_ServerPort = 8080;
	m_Run = true;

	//初始化套接字
	if (RET::SUC != SocketInit())
	{
		return RET::FAIL;
	}
	
	//epoll初始化
	if (RET::SUC != EpollInit())
	{
		return RET::FAIL;
	}

	return RET::SUC;
}

int32_t Acceptor::SocketInit()
{
	if ((m_Fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout<<"Acceptor: Socket Init Failed!"<<std::endl;
		return RET::FAIL;
	}

	//设置非阻塞
	if (RET::SUC != SetNoBlock(m_Fd))
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
		std::cout<<"Acceptor: Bind Failed!"<<std::endl;
		return RET::FAIL;
	}

	//监听
	if ((listen(m_Fd, NS_ACCEPTOR::LISTEN_NUM)) < 0)
	{
		std::cout<<"Acceptor: Listen Failed!"<<std::endl;
		return RET::FAIL;
	}

	return RET::SUC;
}

int32_t Acceptor::SetNoBlock(int32_t fd)
{
	int32_t fd_flag = fcntl(fd, F_GETFL);
	if (fcntl(fd, F_SETFL, fd_flag|O_NONBLOCK) < 0)
	{
		std::cout<<"Acceptor: fcntl Failed!"<<std::endl;
		return RET::FAIL;
	}

	return RET::SUC;
}

//epoll初始化
int32_t Acceptor::EpollInit()
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
		m_EpollCtlFailed++;
		return RET::FAIL;
	} 

	return RET::SUC;
}

//内存分配
NS_ACCEPTOR::epoll_buf_p Acceptor::Alloc(int32_t fd)
{
	size_t Len = sizeof(NS_ACCEPTOR::epoll_buf);
	NS_ACCEPTOR::epoll_buf_p ret = NULL;
	try
	{
		ret = (NS_ACCEPTOR::epoll_buf_p)_MEM_NEW_(Len);
	}
	catch(std::bad_alloc)
	{
		return NULL;
	}
	ret->fd = fd;
	return ret;
} 

int32_t Acceptor::Start()
{
	Accept();
	return RET::SUC;
}

//线程处理函数
void Acceptor::Accept()
{
	InputPacket *pInputPkt = NULL;
	while (m_Run)
	{
		try
		{
			if (NULL == pInputPkt)
			{
				pInputPkt = new InputPacket();
			}
		}
		catch(std::bad_alloc)
		{
			std::cout<<"Acceptor: New InputPacket Error!"<<std::endl;
			return;
		}

		int32_t iRev = epoll_wait(m_Epfd, env, sizeof(env)/sizeof(env[0]), -1);
		std::cout<<"Rev:"<<iRev<<std::endl;
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
					NS_ACCEPTOR::epoll_buf_p buf = (NS_ACCEPTOR::epoll_buf_p)env[Index].data.ptr;
					if (buf->fd == m_Fd && env[Index].events & EPOLLIN)
					{
						struct sockaddr_in client;
						socklen_t Len = sizeof(client);
						int32_t sock = accept(m_Fd, (struct sockaddr*)&client, &Len);
						while (sock > 0)
						{
							SetNoBlock(sock);
							ev.events = EPOLLIN|EPOLLET;
							ev.data.ptr = Alloc(sock);
							if (epoll_ctl(m_Epfd, EPOLL_CTL_ADD, sock, &ev) < 0)
							{
								m_EpollCtlFailed++;
								break;
							}
							
							pInputPkt->m_ClientIp = inet_ntoa(client.sin_addr);
							pInputPkt->m_ClientPort = ntohs(client.sin_port);

							const char * pCliIp = inet_ntoa(client.sin_addr);
							int32_t iCliPort = ntohs(client.sin_port);
							std::cout<<"clientip:"<<pCliIp<<std::endl<<"clientport:"<<iCliPort<<std::endl;
						}

						if (0 > sock)
						{
							m_AcceptFailed++;
						}
						continue;
					}
					else if (env[Index].events & EPOLLIN)
					{
						std::cout<<"read"<<std::endl;
						Read(buf, &env[Index], pInputPkt);
						Processor::GetInstance().Process(pInputPkt);
						pInputPkt = NULL;
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

void Acceptor::Read(NS_ACCEPTOR::epoll_buf_p buf, struct epoll_event *ev_arr, InputPacket *pInputPkt)
{
	int32_t res = -1;
	if ((res = read(buf->fd, buf->Buf, NS_ACCEPTOR::ACCEPT_SIZE)) > 0)
	{
		//(buf->Buf)[res] = 0;
		try
		{
			pInputPkt->pStr = (char*)_MEM_NEW_(res + 1);
		}
		catch(std::bad_alloc)
		{
			return;
		}
		_MEM_CPY_(pInputPkt->pStr, buf->Buf, res);
		_MEM_ZERO_(pInputPkt->pStr, res + 1, res);
		pInputPkt->uLength = res;
		fflush(stdout);
	}

	if (res == 0)
	{
		fflush(stdout);
		if (epoll_ctl(m_Epfd, EPOLL_CTL_DEL, buf->fd, NULL) < 0)
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
	if (epoll_ctl(m_Epfd, EPOLL_CTL_MOD, buf->fd, &e) < 0)
	{
		m_EpollCtlFailed++;
	}

	return;
}

void Acceptor::Write(NS_ACCEPTOR::epoll_buf_p buf)
{
	const char * temp ="HTTP/1.1 200 OK\r\n Content-Length :%s \r\n\r\n Hello world! ";
	int ret= sprintf(buf->Buf, "%s", temp);
	std::cout<<temp<<std::endl;
	write(buf->fd, buf->Buf, ret);
	epoll_ctl(m_Epfd, EPOLL_CTL_DEL, buf->fd, NULL);
	close(buf->fd);
}
