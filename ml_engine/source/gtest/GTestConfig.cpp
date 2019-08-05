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
	ASSERT_EQ(RET::SUC, Config::GetCfg(NS_CONFIG::EM_CFGID_SERVER_PORT, iValue));
	ASSERT_EQ(8080, iValue);

	std::string Value;
	ASSERT_EQ(RET::SUC, Config::GetCfg(NS_CONFIG::EM_CFGID_SERVER_IP, Value));
	ASSERT_EQ(std::string("192.168.125.131"), Value);
}
