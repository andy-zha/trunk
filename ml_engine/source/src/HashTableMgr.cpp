#include "HashTableMgr.h"

/** 构造函数 **/
HashTableMgr::HashTableMgr()
{
	m_hashSize = 0;
}

/** 析构函数 **/
HashTableMgr::~HashTableMgr()
{}

/** 初始化接口 **/
int32_t HashTableMgr::init()
{
	m_hashSize = 17777;
	//哈希表初始化
	if (RET::SUC != m_hashTable.hashTableInit(m_hashSize))
	{
		std::cout<<"HashTableMgr: Hash table init failed!"<<std::endl;
		return RET::FAIL;
	}

	return RET::SUC;
}

/** 查找哈希槽结点接口 **/
HashSlot<HashNode> *HashTableMgr::findHashSlot(uint32_t uHashKey)
{
	return m_hashTable.findHashSlot(uHashKey);
}

/** 查找哈希业务链接口 **/
int32_t HashTableMgr::findHashList(HashSlot<HashNode> *pSlot, DList<HashNode> *&pList)
{
	//异常判断，槽空返回失败
	if (nullptr == pSlot || nullptr == &pSlot->m_hashList) 
	{
		return RET::FAIL;
	}

	pList = &pSlot->m_hashList;
	return RET::SUC;
}
