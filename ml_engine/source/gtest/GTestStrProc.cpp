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
	ASSERT_STREQ(std::string("\xC4\xE3\xBA\xC3 \xCA\xC0\xBD\xE7").c_str(),
					StrProc::UrlDecode("%C4%E3%BA%C3%20%CA%C0%BD%E7").c_str());
}
