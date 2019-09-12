#include "SuperMatch.h"
#include "gtest/gtest.h"

namespace 
{
	class TestSuperMatch : public testing::Test
	{
		void SetUp() override
		{};

		void TearDown() override
		{};
	};
}

TEST_F(TestSuperMatch, TEST_SUPERMATCH)
{
	SuperMatch super;
	std::vector<std::string> pattern;
	pattern.push_back("abcdefg");
	pattern.push_back("abcefgd");
	pattern.push_back("gfedcba");
	pattern.push_back("acds*");
	ASSERT_EQ(RET::SUC, super.Build(pattern));
	ASSERT_EQ(RET::SUC, super.MatchPattern("abcdefg"));
	ASSERT_EQ(RET::SUC, super.MatchPattern("abcefgd"));
	ASSERT_EQ(RET::SUC, super.MatchPattern("gfedcba"));
	ASSERT_EQ(RET::FAIL, super.MatchPattern("gfedcbaad"));    //超长
	ASSERT_EQ(RET::FAIL, super.MatchPattern("gfed"));         //子串
	ASSERT_EQ(RET::FAIL, super.MatchPattern("gedcba"));
	ASSERT_EQ(RET::FAIL, super.MatchPattern("abcfg"));
	ASSERT_EQ(RET::FAIL, super.MatchPattern("abfgd"));

	ASSERT_EQ(RET::SUC, super.SubMatchPattern("abcdef"));
	ASSERT_EQ(RET::SUC, super.SubMatchPattern("abcde"));
	ASSERT_EQ(RET::SUC, super.SubMatchPattern("abcd"));
	ASSERT_EQ(RET::SUC, super.SubMatchPattern("abc"));
	ASSERT_EQ(RET::SUC, super.SubMatchPattern("ab"));
	ASSERT_EQ(RET::SUC, super.SubMatchPattern("a"));
	ASSERT_EQ(RET::SUC, super.SubMatchPattern("abcd"));

	ASSERT_EQ(RET::SUC, super.FuzzyMatchPattern("acdsdsf"));
	ASSERT_EQ(RET::FAIL, super.FuzzyMatchPattern("acdddsf"));
	ASSERT_EQ(RET::FAIL, super.FuzzyMatchPattern("acd"));

	ASSERT_EQ(RET::SUC, super.SoftMatch("AbcDeFg"));
	ASSERT_EQ(RET::SUC, super.SoftMatch("ABceFGd"));
	ASSERT_EQ(RET::SUC, super.SoftMatch("GfeDcbA"));

	ASSERT_EQ(RET::SUC, super.SoftSubMatch("GfeD"));
	ASSERT_EQ(RET::SUC, super.SoftSubMatch("GfeD"));
	ASSERT_EQ(RET::SUC, super.SoftSubMatch("GfeD"));
	ASSERT_EQ(RET::SUC, super.SoftFuzzyMatch("AcDsdsf"));
}
