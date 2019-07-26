#include "MemCheck.h"

MemCheck::MemCheck()
{
	m_TranslationFailed = 0;
}

MemCheck::~MemCheck()
{}

//初始化接口
int32_t MemCheck::Init()
{
	return RET::SUC;
}

//内存分配
void MemCheck::New(void *pAddr)
{
	if (nullptr == pAddr)
	{
		return;
	}

	void *buf[128];
	int32_t size = backtrace(buf, 128);
	uint32_t uHashKey = (uint64_t)pAddr % NS_MEMCHECK::HASHSIZE;
	std::map<uint32_t, NS_MEMCHECK::MemSlot>::iterator iter = m_AddrMapOne.find(uHashKey);

	if (iter != m_AddrMapOne.end())
	{
		std::list<NS_MEMCHECK::MemNode>::iterator it = iter->second.m_Memlist.begin();
		for(; it != iter->second.m_Memlist.end(); it++)
		{
			if (it->addr == pAddr)
			{
				return;
			}
		}
	
		NS_MEMCHECK::MemNode Node;
		Node.addr = pAddr;
		Node.Buf = buf;
		Node.size = size;
		iter->second.m_Memlist.push_back(Node);
	}

	NS_MEMCHECK::MemSlot slot;
	NS_MEMCHECK::MemNode mem;
	mem.addr = pAddr;
	mem.Buf = buf;
	mem.size = size;
	slot.m_Memlist.push_back(mem);
	m_AddrMapOne[uHashKey] = slot;

	return;
}

//内存释放
void MemCheck::Delete(void *pAddr)
{
	if (nullptr == pAddr)
	{
		return;
	}

	uint32_t uHashKey = (uint64_t)pAddr % NS_MEMCHECK::HASHSIZE;
	std::map<uint32_t, NS_MEMCHECK::MemSlot>::iterator iter = m_AddrMapOne.find(uHashKey);

	if (iter != m_AddrMapTwo.end())
	{
		std::list<NS_MEMCHECK::MemNode>::iterator it = iter->second.m_Memlist.begin();
		for(; it != iter->second.m_Memlist.end(); it++)
		{
			if (it->addr == pAddr)
			{
				iter->second.m_Memlist.erase(it);
				return;
			}
		}
	}

	return; 
}

//堆栈信息处理
int32_t MemCheck::Translation(void *buf[1024], int32_t size, FILE *fd)
{
	if (nullptr == buf || nullptr == fd)
	{
		return RET::FAIL;
	}	

	char **trace = backtrace_symbols(buf, size);
	if (nullptr == trace)
	{
		return RET::FAIL;
	}

	size_t uNameSize = 128;
	char *pName = new char [128];
	for (int32_t iIndex = 0; iIndex < size; iIndex++)
	{
		char *pBegin = nullptr;
		char *pAdr = nullptr;
		char *pEnd = nullptr;
		for (char *p = trace[iIndex]; *p; ++p)
		{
			if ('(' == *p)
			{
				pBegin = p;
			}
			else if ('+' == *p && pBegin)
			{
				pAdr = p;
			}
			else if (')' == *p)
			{
				pEnd = p;
			}
		}

		if (pBegin && pAdr && pEnd)
		{
			*pBegin++ = '\0';
			*pAdr++ = '\0';
			*pEnd++ = '\0';
			int32_t status = -4;
			char Out[128];
			uint32_t uLen = 0;
			char *Ret = abi::__cxa_demangle(pBegin, pName, &uNameSize, &status);
			if (0 == status)
			{
				pName = Ret;
				uLen = snprintf(Out, 128, "%s+%s", pName, pAdr);
				fwrite(Out, uLen, 1, fd);
				std::cout<<pName<<"+"<<pAdr<<std::endl;
			}
			else
			{
				uLen = snprintf(Out, 128, "%s+%s", pBegin, pAdr);
				fwrite(Out, uLen, 1, fd);
				std::cout<<pBegin<<"+"<<pAdr<<std::endl;
			}
		}
		else
		{
			std::cout<<trace[iIndex]<<std::endl;
		}
	}

	delete [] pName;
	free(trace);
	return RET::SUC;
}

int32_t MemCheck::WriteLog()
{
	int32_t iValue = 1;

	if (0 == iValue)
	{
		return RET::SUC;
	}

	char File[128];
	time_t nowtime = time(nullptr);
	struct tm *local = localtime(&nowtime);
	snprintf(File, 128, "MemCheck_%04d%02d%02d%2d%2d%2d_%d.log", local->tm_year + 1900,
			local->tm_mon + 1, local->tm_mday, local->tm_hour, local->tm_min,
			local->tm_sec, getpid());
	FILE *fd = fopen(File, "a+");

	if (nullptr == fd)
	{
		return RET::FAIL;
	}	

	uint32_t uTotal = 0;

	std::map<uint32_t, NS_MEMCHECK::MemSlot>::iterator iter = m_AddrMapOne.begin();
	for (; iter != m_AddrMapOne.end(); iter++)
	{
		std::list<NS_MEMCHECK::MemNode>::iterator it = iter->second.m_Memlist.begin();
		for (; it != iter->second.m_Memlist.end(); it++)
		{
			NS_MEMCHECK::MemNode Node;
			Node.addr = it->addr;
			Node.Buf = it->Buf;
			Node.size = it->size; 
			
			std::map<uint32_t, NS_MEMCHECK::MemSlot>::iterator i = m_AddrMapTwo.find(iter->first);
			if (i != m_AddrMapTwo.end())
			{
		 		i->second.m_Memlist.push_back(Node);
				continue;
			}

			NS_MEMCHECK::MemSlot Slot;
			Slot.m_Memlist.push_back(Node);
			m_AddrMapOne[iter->first] = Slot;
		
			uTotal++;
		}
	}

	std::map<uint32_t, NS_MEMCHECK::MemSlot>::iterator slot = m_AddrMapTwo.begin();
	for (; slot != m_AddrMapTwo.end(); slot++)
	{
		std::list<NS_MEMCHECK::MemNode>::iterator mem = slot->second.m_Memlist.begin();
		for (; mem != slot->second.m_Memlist.end(); mem++)
		{
			if (RET::SUC != Translation(mem->Buf, mem->size, fd))
			{
				m_TranslationFailed++;
			}
		}
	}

	fclose(fd);
	return RET::SUC;
}
