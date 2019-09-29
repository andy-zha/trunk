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
	ASSERT_EQ(RET::SUC, super.build(pattern));
	ASSERT_EQ(RET::SUC, super.matchPattern("abcdefg"));
	ASSERT_EQ(RET::SUC, super.matchPattern("abcefgd"));
	ASSERT_EQ(RET::SUC, super.matchPattern("gfedcba"));
	ASSERT_EQ(RET::FAIL, super.matchPattern("gfedcbaad"));    //超长
	ASSERT_EQ(RET::FAIL, super.matchPattern("gfed"));         //子串
	ASSERT_EQ(RET::FAIL, super.matchPattern("gedcba"));
	ASSERT_EQ(RET::FAIL, super.matchPattern("abcfg"));
	ASSERT_EQ(RET::FAIL, super.matchPattern("abfgd"));

	ASSERT_EQ(RET::SUC, super.subMatchPattern("abcdef"));
	ASSERT_EQ(RET::SUC, super.subMatchPattern("abcde"));
	ASSERT_EQ(RET::SUC, super.subMatchPattern("abcd"));
	ASSERT_EQ(RET::SUC, super.subMatchPattern("abc"));
	ASSERT_EQ(RET::SUC, super.subMatchPattern("ab"));
	ASSERT_EQ(RET::SUC, super.subMatchPattern("a"));
	ASSERT_EQ(RET::SUC, super.subMatchPattern("abcd"));

	ASSERT_EQ(RET::SUC, super.fuzzyMatchPattern("acdsdsf"));
	ASSERT_EQ(RET::FAIL, super.fuzzyMatchPattern("acdddsf"));
	ASSERT_EQ(RET::FAIL, super.fuzzyMatchPattern("acd"));

	ASSERT_EQ(RET::SUC, super.softMatch("AbcDeFg"));
	ASSERT_EQ(RET::SUC, super.softMatch("ABceFGd"));
	ASSERT_EQ(RET::SUC, super.softMatch("GfeDcbA"));

	ASSERT_EQ(RET::SUC, super.softSubMatch("GfeD"));
	ASSERT_EQ(RET::SUC, super.softSubMatch("GfeD"));
	ASSERT_EQ(RET::SUC, super.softSubMatch("GfeD"));
	ASSERT_EQ(RET::SUC, super.softFuzzyMatch("AcDsdsf"));
}
