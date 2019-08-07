#include "IPDRuleMgr.h"
#include "gtest/gtest.h"

namespace 
{
	class TestIPDRuleMgr : public testing::Test
	{
		void SetUp() override
		{
		};

		void TearDown() override
		{};
	};
}

TEST_F(TestIPDRuleMgr, Test_Init)
{
	//test init
	ASSERT_EQ(RET::SUC, IPDRuleMgr::GetInstance().Init());
}

TEST_F(TestIPDRuleMgr, Test_IPDRuleMgr)
{
	//test QueryNewBusiness
	ASSERT_EQ(RET::SUC, IPDRuleMgr::GetInstance().QueryNewBusiness());

	//test QueryBusiness
	std::vector<NS_IPDRULE::BrSlot> m_List;
	NS_IPDRULE::BrSlot _Slot;
	_Slot.m_BusinessId = "1";
	m_List.push_back(_Slot);
	std::vector<NS_IPDRULE::BrSlot>::iterator iter = m_List.begin();
	ASSERT_EQ(RET::SUC, IPDRuleMgr::GetInstance().QueryBusiness(iter));
}
