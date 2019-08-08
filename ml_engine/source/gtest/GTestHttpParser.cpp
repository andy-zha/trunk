#include "HttpParser.h"
#include "gtest/gtest.h"

namespace 
{
	class TestHttpParser : public testing::Test
	{
		void SetUp() override
		{};

		void TearDown() override
		{};
	};
}

TEST_F(TestHttpParser, Test_HttpParser)
{
	HttpParser _Parser;
	InputPacket *pInputPkt= new InputPacket();
	std::string url = "/web/show.asp?id=1382&flag=%C4%E3%BA%C3%CA%C0%BD%E7";
	ASSERT_EQ(RET::SUC, _Parser.ParserUri(url, pInputPkt)); 
	ASSERT_STREQ(std::string("/web/show.asp").c_str(), pInputPkt->m_Url.c_str());
	ASSERT_STREQ(std::string("id=1382&flag=\xC4\xE3\xBA\xC3\xCA\xC0\xBd\xE7").c_str(), pInputPkt->m_Query.c_str());
}
