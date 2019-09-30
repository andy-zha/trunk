#include "Config.h"
#include "gtest/gtest.h"

namespace 
{
	class TestConfig : public testing::Test
	{
		void SetUp() override
		{};

		void TearDown() override
		{};
	};
}

TEST_F(TestConfig, TEST_CONFIG)
{
	int32_t iValue = -1;
	ASSERT_EQ(RET::SUC, Config::getCfg(NS_CONFIG::EM_CFGID_SERVER_PORT, iValue));
	ASSERT_EQ(8080, iValue);

	std::string Value;
	ASSERT_EQ(RET::SUC, Config::getCfg(NS_CONFIG::EM_CFGID_SERVER_IP, Value));
	ASSERT_EQ(std::string("192.168.125.131"), Value);

	ASSERT_EQ(RET::FAIL, Config::getCfg(NS_CONFIG::EM_CFGID_END, iValue));
	ASSERT_EQ(RET::FAIL, Config::getCfg(NS_CONFIG::EM_CFGID_END, Value));

	std::string cfgFile = "../etc/filter.json";
	const char *cfgPath[CFG_MAX_DEPTH] = {"ALL", "TOTAL", (const char*)0};
	ASSERT_EQ(RET::SUC, Config::getCfg(cfgFile, cfgPath, iValue));
	ASSERT_EQ(8, iValue);
	const char *Path[CFG_MAX_DEPTH] = {"ALL", "1", (const char*)0};
	ASSERT_EQ(RET::SUC, Config::getCfg(cfgFile, Path, Value));
	ASSERT_STREQ(std::string("ASP.NETSessionid").c_str(), Value.c_str());
}
