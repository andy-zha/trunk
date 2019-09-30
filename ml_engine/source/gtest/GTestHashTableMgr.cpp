#include "HashTableMgr.h"
#include "gtest/gtest.h"

namespace 
{
	class TestHashTableMgr : public testing::Test
	{
		void SetUp() override
		{};

		void TearDown() override
		{};
	};
}

class HashNode : public HashNodeBase
{
	public:
		HashNode() {}
		~HashNode() {}

	public:
		int32_t m_index;
		std::string m_str;
};

TEST_F(TestHashTableMgr, TEST_HASHTABLEMGR)
{
	//test 注册接口
	ASSERT_EQ(RET::SUC, HashTableMgr::getInstance().registerFunc(0, 17777));
	//test 初始化接口
	ASSERT_EQ(RET::SUC, HashTableMgr::getInstance().init());
	//查找哈希槽结点
	HashSlot<HashNode> *pSlot = nullptr;
	pSlot = (HashSlot<HashNode>*)(HashTableMgr::getInstance().findHashSlot(0, 10));
	//test 哈希链
	DList<HashNode> *pList = &pSlot->m_hashList;
	//ASSERT_EQ(RET::SUC, HashTableMgr::getInstance().findHashList((HashSlot<HashNodeBase*>*)pSlot, pList));
	DList<HashNode> *pNode = new DList<HashNode>;
	pNode->m_dlist_data.m_index = 0;
	pNode->m_dlist_data.m_str = "yes";
	ASSERT_EQ(RET::SUC, pList->TailAddNode(pNode));
}
