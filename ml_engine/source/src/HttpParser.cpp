#include "HttpParser.h"
#include "DbAdmin.h"

//构造函数
HttpParser::HttpParser()
{}

//析构函数
HttpParser::~HttpParser()
{}

//解析器初始化接口
int32_t HttpParser::Init()
{
	return RET::SUC;
}

int32_t HttpParser::Start(InputPacket *pInputPkt)
{
	//异常判断
	if (nullptr == pInputPkt || nullptr == pInputPkt->pStr
					|| 0 == pInputPkt->uLength)
	{
		return RET::FAIL;
	}

	//取出请求行和请求头
	char *pPos = strstr(pInputPkt->pStr, "\r\n\r\n");
	uint32_t uHeaderlen = pPos - pInputPkt->pStr;
	std::string http_header = std::string(pInputPkt->pStr, uHeaderlen);

	//取出请求体
	std::string http_body;
	if (nullptr != (pPos + 4))
		http_body = pPos;

	//解析请求行与请求头
	const char *pMethod = nullptr;
	const char *pPath = nullptr;
	struct phr_header headers[32];
	size_t MethodLen = 0;
	size_t PathLen = 0;
	size_t HeaderNum = 32;
	size_t Ret = phr_parse_request(http_header.data(), http_header.size(), &pMethod, 
					&MethodLen, &pPath, &PathLen, &pInputPkt->MinorVer, headers, &HeaderNum, 0);
	if (-1 == Ret)
	{
		return RET::FAIL;
	}

	//取请求方法、Uri
	pInputPkt->m_HttpMethod = std::string(pMethod, MethodLen);
	std::string uri = std::string(pPath, PathLen);
	//解析url
	ParserUri(uri, pInputPkt);

	std::string cookie;
	for (uint32_t uIndex = 0; uIndex < HeaderNum; uIndex++)
	{
		std::string key = std::string(headers[uIndex].name, headers[uIndex].name_len);
		std::string value = std::string(headers[uIndex].value, headers[uIndex].value_len);

		//根据Content-Type中信息若为音视频等信息的不学习
		if (key == NS_HTTPPARSER::CONTENT_TYPE)
		{
			if (value.find("audio/") == 0 || value.find("font/") == 0
							|| value.find("image/") == 0 || value.find("multipart/") == 0
							|| value.find("video/") == 0)
			{
				return RET::SUC;
			}
		} 
		//请求头内只取host与cookie
		if (key == NS_HTTPPARSER::HOST)
		{
			pInputPkt->m_Host = value;
		}

		if (key == NS_HTTPPARSER::COOKIE)
		{
			cookie = value;
		}
	}

	//过滤规则
	//if (RET::SUC != IPDRuleMgr::GetInstance().MatchRules(pInputPkt))
	//{
	//	return RET::FAIL;
	//}

	//数据库连接
	//DbAdmin db;	
	//if (RET::SUC != db.Connect())
	//{
	//	return RET::FAIL;
	//}
	////url状态查询
	//MYSQL_RES *pResult = nullptr;
	//uint64_t NowTime = Timer::GetLocalTime();	
	//std::string Sql = "SELECT u_id, status FROM url_1_1 WHERE name = '" + pInputPkt->m_Url + "';";
	//if (RET::SUC != db.ExecQuery(Sql, pResult) || nullptr == pResult)
	//{
	//	Sql = "INSERT INTO url_1_1(u_id, name, status, learn_process, learn_rate, hits, \
	//			cycle_hits, readonly, lastactivetime, reliability) SELECT 0, '" 
	//			+ pInputPkt->m_Url + "', 0, 1, 0, 0, 0, 0, " + std::to_string(NowTime) 
	//			+ ", 0 FROM DUAL WHERE NOT EXISTS (SELECT name FROM url_1_1 WHERE name = '"
	//			+ pInputPkt->m_Url + "');";
	//	db.ExecSql(Sql);
	//}

	//if (0 == mysql_num_rows(pResult))
	//{
	//	Sql = "INSERT INTO url_1_1(u_id, name, status, learn_process, learn_rate, hits, \
	//			cycle_hits, readonly, lastactivetime, reliability) SELECT 0, '" 
	//			+ pInputPkt->m_Url + "', 0, 1, 0, 0, 0, 0, " + std::to_string(NowTime) 
	//			+ ", 0 FROM DUAL WHERE NOT EXISTS (SELECT name FROM url_1_1 WHERE name = '"
	//			+ pInputPkt->m_Url + "');";
	//	db.ExecSql(Sql);
	//}

	//MYSQL_ROW row = mysql_fetch_row(pResult);
	//if (nullptr == row[0] || nullptr == row[1])
	//{
	//	return RET::FAIL;
	//}

	//Sql = "UPDATE url_1_1 SET hits = hits + 1, cycle_hits = cycle_hits + 1 WHERE name = '"
	//		+ pInputPkt->m_Url + "';";
	//db.ExecSql(Sql);
	//db.Close();
	
	return RET::SUC;
}

void HttpParser::ParserUri(std::string uri, InputPacket *pInputPkt)
{
	//size为0直接返回
	if (0 == uri.size()) 
	{
		return;
	}
	
	//按?进行切割
	uint32_t uPos = uri.find('?');
	if (uPos == std::string::npos)
	{
		pInputPkt->m_Url = uri;
		return;
	}

	pInputPkt->m_Url = std::string(uri, 0, uPos);
} 
