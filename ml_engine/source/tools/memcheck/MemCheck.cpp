#include "MemCheck.h"

MemCheck::MemCheck()
{
	m_NewNodeFailed = 0;
	m_FindNodeFailed = 0;
}

MemCheck::~MemCheck()
{}

//初始化接口
int32_t MemCheck::Init()
{
	if (RET::SUC != m_HashTableOne.HashTableInit(NS_MEMCHECK::HASHSIZE)
			|| RET::SUC != m_HashTableTwo.HashTableInit(NS_MEMCHECK::HASHSIZE))
	{
		return RET::FAIL;
	}

	return RET::SUC;
}

//内存分配
void MemCheck::New(void *pAddr)
{
	void *buf[128];
	int32_t size = backtrace(buf, 128);
	uint32_t uHashKey = (uint64_t)pAddr / NS_MEMCHECK::HASHSIZE;
	HashSlot<NS_MEMCHECK::st_BtNode> *pNode = m_HashTableOne.FindHashSlot(uHashKey);
	if (NULL != pNode)
	{
		//上锁
		pNode->Lock();

		//遍历业务链，未找到相同地址则插入哈希结点
		HDList<NS_MEMCHECK::st_BtNode> *pBtNode = NULL;
		while (RET::SUC == pNode->m_Hash_List.ForwardTraver(pBtNode))
		{
			if (pBtNode->m_dlist_data.p == pAddr)
			{
				//解锁
				pNode->UnLock();
				return;
			}
		}
		
		try
		{
			pBtNode = new HDList<NS_MEMCHECK::st_BtNode>;
		}
		catch(...)
		{
			m_NewNodeFailed++;
			//解锁
			pNode->UnLock();
			return;
		}
		pBtNode->m_dlist_data.p = pAddr;
		pBtNode->m_dlist_data.buf = buf;
		pBtNode->m_dlist_data.size = size;
		pNode->m_Hash_List.TailAddNode(pBtNode);

		//解锁
		pNode->UnLock();
	}

	return;
}

//内存释放
void MemCheck::Delete(void *pAddr)
{
	if (NULL == pAddr)
	{
		return;
	}

	uint32_t uHashKey = (uint64_t)pAddr / NS_MEMCHECK::HASHSIZE;
	HashSlot<NS_MEMCHECK::st_BtNode> *pNode = m_HashTableOne.FindHashSlot(uHashKey);
	if (NULL != pNode)
	{
		//上锁
		pNode->Lock();

		//遍历业务链，找到相同地址则删除哈希结点
		HDList<NS_MEMCHECK::st_BtNode> *pBtNode = NULL;
		while (RET::SUC == pNode->m_Hash_List.ForwardTraver(pBtNode))
		{
			if (pBtNode->m_dlist_data.p == pAddr)
			{
				pNode->m_Hash_List.RemoveNode(pBtNode);
			}
		}
	
		m_FindNodeFailed++;	
		//解锁
		pNode->UnLock();
	}

	return; 
}

//堆栈信息处理
int32_t MemCheck::Translation(void *buf[1024], int32_t size, FILE *fd)
{
	char **trace = backtrace_symbols(buf, size);
	if (NULL == trace)
	{
		return RET::FAIL;
	}

	size_t uNameSize = 128;
	char *pName = new char [128];
	for (int32_t iIndex = 0; iIndex < size; iIndex++)
	{
		char *pBegin = NULL;
		char *pAdr = NULL;
		char *pEnd = NULL;
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
	int32_t iValue = 0;

	if (0 == iValue)
	{
		return RET::SUC;
	}

	char File[128];
	time_t nowtime = time(NULL);
	struct tm *local = localtime(&nowtime);
	snprintf(File, 128, "MemCheck_%04d%02d%02d%2d%2d%2d_%d.log", local->tm_year + 1900,
			local->tm_mon + 1, local->tm_mday, local->tm_hour, local->tm_min,
			local->tm_sec, getpid());
	FILE *fd = fopen(File, "a+");
	if (NULL == fd)
	{
		return RET::FAIL;
	}	

	uint32_t uTotal = 0;
	for (uint32_t uIndex = 0; uIndex < NS_MEMCHECK::HASHSIZE; uIndex++)
	{
		HashSlot<NS_MEMCHECK::st_BtNode> *pNode = m_HashTableOne.FindHashSlot(uIndex);
		if (NULL != pNode)
		{
			//上锁
			pNode->Lock();
			
			if (0 == pNode->m_Hash_List.GetLen())
			{
				//解锁
				pNode->UnLock();
				continue;
			}

			HashSlot<NS_MEMCHECK::st_BtNode> *pNodeTwo = m_HashTableTwo.FindHashSlot(uIndex);
			pNodeTwo->Lock();

			//遍历业务链，从哈希1挂到哈希2，为内存未释放的结点
			HDList<NS_MEMCHECK::st_BtNode> *pBtNode = NULL;
			while (RET::SUC == pNode->m_Hash_List.ForwardTraver(pBtNode))
			{
				pNode->m_Hash_List.PopNode(pBtNode);
				pNodeTwo->m_Hash_List.TailAddNode(pBtNode);
				uTotal++;
			}
		
			//去锁	
			pNode->UnLock();
			pNodeTwo->UnLock();
		}
	}

	//遍历哈希2，写日志
	for (uint32_t u = 0; u < NS_MEMCHECK::HASHSIZE; u++)
	{
		HashSlot<NS_MEMCHECK::st_BtNode> *pHashNode = m_HashTableTwo.FindHashSlot(u);
		if (NULL != pHashNode)
		{
			pHashNode->Lock();

			HDList<NS_MEMCHECK::st_BtNode> *pWrite = NULL;
			while (RET::SUC == pHashNode->m_Hash_List.ForwardTraver(pWrite))
			{
				if (RET::SUC != Translation(pWrite->m_dlist_data.buf, pWrite->m_dlist_data.size, fd))
				{
					for (uint32_t uIndex = 0; uIndex < pWrite->m_dlist_data.size; uIndex++)
					{ 
						//写栈信息
						fwrite(pWrite->m_dlist_data.buf[uIndex], 
										strlen((char*)pWrite->m_dlist_data.buf[uIndex]), 1, fd);
					}
				}
			}

			//解锁	
			pHashNode->UnLock();
		}
	}

	fclose(fd);
}
