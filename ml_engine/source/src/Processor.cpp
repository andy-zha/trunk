#include "Processor.h"

//构造函数
Processor::Processor()
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
Processor::~Processor()
{}

//初始化接口
int32_t Processor::Init()
{
	if (RET::SUC != _Parser.Init())
	{
		return RET::FAIL;
	}

	//读取服务端ip
	if (RET::SUC != Config::GetCfg(NS_CONFIG::EM_CFGID_SERVER_IP, m_ServerIp))
	{
		std::cout<<"Processor: Read Server Ip Failed!"<<std::endl;
		return RET::FAIL;	
	}

	//读取服务端port
	int32_t iValue = -1;
	if (RET::SUC != Config::GetCfg(NS_CONFIG::EM_CFGID_SERVER_PORT, iValue))
	{
		std::cout<<"Processor: Read Server Port Failed!"<<std::endl;
		return RET::FAIL;	
	}
	m_ServerPort = iValue;

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

int32_t Processor::SocketInit()
{
	if ((m_Fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout<<"Processor: Socket Init Failed!"<<std::endl;
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
		std::cout<<"Processor: Bind Failed!"<<std::endl;
		return RET::FAIL;
	}

	//监听
	if ((listen(m_Fd, NS_ACCEPTOR::LISTEN_NUM)) < 0)
	{
		std::cout<<"Processor: Listen Failed!"<<std::endl;
		return RET::FAIL;
	}

	return RET::SUC;
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

//epoll初始化
int32_t Processor::EpollInit()
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
NS_ACCEPTOR::epoll_buf *Processor::Alloc(int32_t fd)
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

void Processor::Free(NS_ACCEPTOR::epoll_buf *ptr)
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
	InputPacket *pInputPkt = nullptr;
	while (m_Run)
	{
		//加载业务规则
		IPDRuleMgr::GetInstance().Process();

#ifdef _MEMCHECK_
		MemCheck();
#endif

		try
		{
			if (nullptr == pInputPkt)
			{
				pInputPkt = new InputPacket();
			}
		}
		catch(std::bad_alloc)
		{
			std::cout<<"Processor: New InputPacket Error!"<<std::endl;
			continue;
		}

		int32_t iRev = epoll_wait(m_Epfd, env, 32, 10000);
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
					NS_ACCEPTOR::epoll_buf *buf = (NS_ACCEPTOR::epoll_buf*)env[Index].data.ptr;
					if (buf->fd == m_Fd && env[Index].events & EPOLLIN)
					{
						struct sockaddr_in client;
						socklen_t Len = sizeof(client);
						int32_t sock = accept(m_Fd, (struct sockaddr*)&client, &Len);
						while (sock > 0)
						{
							SetNoBlock(sock);
							ev.events = EPOLLIN|EPOLLET;
							pEndfree = ev.data.ptr;
							ev.data.ptr = Alloc(sock);
							if (epoll_ctl(m_Epfd, EPOLL_CTL_ADD, sock, &ev) < 0)
							{
								m_EpollCtlFailed++;
								break;
							}
							
							pInputPkt->m_ClientIp = inet_ntoa(client.sin_addr);
							pInputPkt->m_ClientPort = ntohs(client.sin_port);
						}

						if (0 > sock)
						{
							m_AcceptFailed++;
						}
						continue;
					}
					else if (env[Index].events & EPOLLIN)
					{
						Read(buf, &env[Index], pInputPkt);
						_Parser.Start(pInputPkt);
						if (nullptr != pInputPkt)
						{
							delete pInputPkt;
							pInputPkt = nullptr;
						}
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
void Processor::Read(NS_ACCEPTOR::epoll_buf *buf, struct epoll_event *ev_arr, InputPacket *pInputPkt)
{
	int32_t res = -1;
	//读取http上行内容
	if ((res = read(buf->fd, buf->Buf, NS_ACCEPTOR::ACCEPT_SIZE)) > 0)
	{
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
		if (epoll_ctl(m_Epfd, EPOLL_CTL_DEL, buf->fd, nullptr) < 0)
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

//给出http下行响应
void Processor::Write(NS_ACCEPTOR::epoll_buf *buf)
{
	const char * temp ="HTTP/1.1 200 OK\r\n Content-Length :%s \r\n\r\n Hello world! ";
	int ret= sprintf(buf->Buf, "%s", temp);
	write(buf->fd, buf->Buf, ret);
	epoll_ctl(m_Epfd, EPOLL_CTL_DEL, buf->fd, nullptr);
	close(buf->fd);
	Free((NS_ACCEPTOR::epoll_buf*)ev.data.ptr);
	Free((NS_ACCEPTOR::epoll_buf*)pEndfree);
}

#ifdef _MEMCHECK_

void Processor::MemCheck()
{
	DbAdmin m_db;
	m_db.Connect();
	
	MYSQL_RES *pResult = nullptr;
	std::string Sql = "SELECT memcheck FROM cmd;";
	m_db.ExecQuery(Sql, pResult);

	if (nullptr != pResult && 0 != mysql_num_rows(pResult))
	{
		MYSQL_ROW row = mysql_fetch_row(pResult);
		if (1 == std::stoul(row[0]))
		{
			MemCheck::GetInstance().WriteLog();
			Sql = "UPDATE cmd SET memcheck = 0;";
			m_db.ExecSql(Sql);
		}
	}

	mysql_free_result(pResult);
	pResult = nullptr;	
	m_db.Close();
}

#endif
