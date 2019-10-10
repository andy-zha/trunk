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

TEST_F(TestHashTableMgr, TEST_HASHTABLEMGR)
{
	//test 初始化接口
	ASSERT_EQ(RET::SUC, HashTableMgr::getInstance().init());
	//查找哈希槽结点
	HashSlot<HashNode> *pSlot = nullptr;
	pSlot = HashTableMgr::getInstance().findHashSlot(10);
	//test 哈希链
	DList<HashNode> *pList = nullptr;
	ASSERT_EQ(RET::SUC, HashTableMgr::getInstance().findHashList(pSlot, pList));
	DList<HashNode> *pNode = new DList<HashNode>;
	ASSERT_EQ(RET::SUC, pList->TailAddNode(pNode));
}
