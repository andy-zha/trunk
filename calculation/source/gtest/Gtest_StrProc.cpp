#include "gtest/gtest.h"
#include "StrProc.h"

namespace {
    class Test_StrProc : public testing::Test
	{
	    void SetUp() override{};

		void TearDown() override{};
	};
}

TEST_F(Test_StrProc, GTest_StrProc)
{
    //test 获取字符串匹配度
    std::string strone = "abc";
	std::string strtwo = "abe";
    ASSERT_FLOAT_EQ(0.66666663, StrProc::GetStrMatchDegree(strone, strtwo));
}
