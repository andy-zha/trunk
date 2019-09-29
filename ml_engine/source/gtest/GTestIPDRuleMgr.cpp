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

TEST_F(TestIPDRuleMgr, Test_INIT)
{
	//test init
	ASSERT_EQ(RET::SUC, IPDRuleMgr::getInstance().Init());
}

TEST_F(TestIPDRuleMgr, Test_IPDRULEMGR)
{
	//test QueryNewBusiness
	ASSERT_EQ(RET::SUC, IPDRuleMgr::getInstance().QueryNewBusiness());

	//test QueryBusiness
	std::vector<NS_IPDRULE::BrSlot> m_List;
	NS_IPDRULE::BrSlot _Slot;
	_Slot.m_BusinessId = "1";
	m_List.push_back(_Slot);
	std::vector<NS_IPDRULE::BrSlot>::iterator iter = m_List.begin();
	ASSERT_EQ(RET::SUC, IPDRuleMgr::getInstance().QueryBusiness(iter));

	//test CompareIp
	std::string ip = "192.168.26.12";
	std::string _ip = "192.168.26.12/12";
	ASSERT_EQ(RET::SUC, IPDRuleMgr::getInstance().CompareIp(ip, _ip));

	//test CompareDomain
	std::string domain = "www.baidu.com";
	std::string _domain = "www.baidu.*";
	ASSERT_EQ(RET::SUC, IPDRuleMgr::getInstance().CompareDomain(domain, _domain));
	domain = "www.baiducom";
	ASSERT_EQ(RET::FAIL, IPDRuleMgr::getInstance().CompareDomain(domain, _domain));
	_domain = "*.baiducom";
	ASSERT_EQ(RET::SUC, IPDRuleMgr::getInstance().CompareDomain(domain, _domain));
	domain = "*.baidu.com";
	ASSERT_EQ(RET::FAIL, IPDRuleMgr::getInstance().CompareDomain(domain, _domain));
}
