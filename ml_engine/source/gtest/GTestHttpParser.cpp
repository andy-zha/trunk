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

TEST_F(TestHttpParser, TEST_INIT)
{
	HttpParser _Parser;
	ASSERT_EQ(RET::SUC, _Parser.init());
}

TEST_F(TestHttpParser, Test_HttpParser)
{
	HttpParser _Parser;
	std::string url = "/web/main.do;JESSIONID=dadkfahjhajdsakjh";
	ASSERT_STREQ(std::string("/web/main.do").c_str(), _Parser.filterUrl(url).c_str());

	InputPacket *pInputPkt= new InputPacket();
	url = "http://www.baidu.com:80/web/show.asp?id=1382&flag=%C4%E3%BA%C3%CA%C0%BD%E7";
	ASSERT_EQ(RET::SUC, _Parser.parserUri(url, pInputPkt)); 
	ASSERT_STREQ(std::string("/web/show.asp").c_str(), pInputPkt->m_Url.c_str());
	ASSERT_STREQ(std::string("id=1382&flag=\xC4\xE3\xBA\xC3\xCA\xC0\xBd\xE7").c_str(),
					pInputPkt->m_Query.c_str());
	delete pInputPkt;

	std::string str = "GET /web/show.asp?id=1382 HTTP/1.1\nCookie: lu=Rg3vHJZnehYLjVg7qi3bZjzg\nHost: www.baidu.com\nConnection: Keep-Alive\n\r\n";
	pInputPkt = new InputPacket;
	pInputPkt->pStr = (char*)_MEM_NEW_(str.size() + 1);
	pInputPkt->uLength = str.size() + 1;
	_MEM_CPY_(pInputPkt->pStr, const_cast<char*>(str.c_str()), str.size());
	_MEM_ZERO_(pInputPkt->pStr, str.size() + 1, str.size());
	ASSERT_EQ(RET::SUC, _Parser.start(pInputPkt));
	delete pInputPkt;	
}
