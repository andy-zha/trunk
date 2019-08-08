#include "StrProc.h"
#include "gtest/gtest.h"

namespace 
{
	class TestStrProc : public testing::Test
	{
		void SetUp() override
		{};

		void TearDown() override
		{};
	};
}

TEST_F(TestStrProc, TESTSTRPROC)
{
//	ASSERT_STREQ(std::string("%B2%BB%D2%AA%C3%D4%C1%B5%B8%E7").c_str(), 
//					StrProc::UrlEncode("不要迷恋哥").c_str());
	ASSERT_STREQ(std::string("\xC4\xE3\xBA\xC3 \xCA\xC0\xBD\xE7").c_str(),
					StrProc::UrlDecode("%C4%E3%BA%C3%20%CA%C0%BD%E7").c_str());
}
