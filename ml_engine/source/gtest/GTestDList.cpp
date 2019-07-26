#include "DList.h"
#include "gtest/gtest.h"

namespace 
{
	class TestDList : public testing::Test
	{
		void SetUp() override
		{};

		void TearDown() override
		{};
	};
}

class GTest
{
	public:
		GTest(){}

		~GTest(){}
	public:
		int32_t iTest;
};

TEST_F(TestDList, Test_DList)
{
	DList<GTest> m_TestLists;

	//test HeadAddNode
	DList<GTest> *pNode1 = new DList<GTest>;
	pNode1->m_dlist_data.iTest = 1;
	ASSERT_EQ(RET::SUC, m_TestLists.HeadAddNode(pNode1));

	//test TailAddNode
	DList<GTest> *pNode2 = new DList<GTest>;
	pNode2->m_dlist_data.iTest = 2;
	ASSERT_EQ(RET::SUC, m_TestLists.TailAddNode(pNode2));

	//test NextAddNode
	DList<GTest> *pNode4 = new DList<GTest>;
	pNode4->m_dlist_data.iTest = 4;
	ASSERT_EQ(RET::SUC, m_TestLists.NextAddNode(pNode4, pNode2));

	//test PrevAddNode
	DList<GTest> *pNode3 = new DList<GTest>;
	pNode3->m_dlist_data.iTest = 3;
	ASSERT_EQ(RET::SUC, m_TestLists.PrevAddNode(pNode3, pNode4));

	//test GetLen
	ASSERT_EQ(4, m_TestLists.GetLen());

	//test ForwardTraver
	DList<GTest> *pNode = nullptr;
	ASSERT_EQ(RET::SUC, m_TestLists.ForwardTraver(pNode));
	ASSERT_EQ(1, pNode->m_dlist_data.iTest);

	//test BackwardTraver
	pNode = nullptr;
	ASSERT_EQ(RET::SUC, m_TestLists.BackwardTraver(pNode));
	ASSERT_EQ(4, pNode->m_dlist_data.iTest);

	//test PopNode
	ASSERT_EQ(RET::SUC, m_TestLists.PopNode(pNode));
	ASSERT_EQ(3, m_TestLists.GetLen());
	delete pNode;
	pNode = nullptr;

	//test RemoveNode
	ASSERT_EQ(RET::SUC, m_TestLists.BackwardTraver(pNode));
	ASSERT_EQ(RET::SUC, m_TestLists.RemoveNode(pNode));
	ASSERT_EQ(2, m_TestLists.GetLen());

	//test DestroyList
	ASSERT_EQ(RET::SUC, m_TestLists.DestroyList());
	ASSERT_EQ(0, m_TestLists.GetLen());
}
