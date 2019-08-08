#include "HttpParser.h"

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

//解析器主入口
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
	std::cout<<"Uri: "<<pInputPkt->m_Url<<std::endl;
	std::cout<<"Query: "<<pInputPkt->m_Query<<std::endl;

	std::string cookie;
	for (uint32_t uIndex = 0; uIndex < HeaderNum; uIndex++)
	{
		std::string key = std::string(headers[uIndex].name, headers[uIndex].name_len);
		std::string value = std::string(headers[uIndex].value, headers[uIndex].value_len);

		//根据Content-Type中信息若为音视频等信息的不学习
		if (key == NS_HTTPPARSER::CONTENT_TYPE)
		{
			if (value.find("audio/") != std::string::npos 
							|| value.find("font/") != std::string::npos
							|| value.find("image/") != std::string::npos
							|| value.find("multipart/") != std::string::npos
							|| value.find("video/") != std::string::npos)
			{
				return RET::SUC;
			}
		}
 
		//请求头内只取host与cookie
		if (key == NS_HTTPPARSER::HOST)
		{
			pInputPkt->m_Host = std::string(value, value.size());
		}

		if (key == NS_HTTPPARSER::COOKIE)
		{
			cookie = value;
		}
	}

	//过滤规则
	if (RET::SUC != IPDRuleMgr::GetInstance().MatchRules(pInputPkt))
	{
		return RET::FAIL;
	}

	//解析cookie体	
	ParserCookie(cookie);

	//解析http体
	ParserBody(http_body);

	return RET::SUC;
}

//解析uri
int32_t HttpParser::ParserUri(std::string uri, InputPacket *pInputPkt)
{
	//size为0直接返回
	if (0 == uri.size()) 
	{
		return RET::FAIL;
	}

	//decode解码
	std::string decode_uri = StrProc::UrlDecode(uri);
	//按?进行切割,取url/query
	uint32_t uPos = decode_uri.find('?');
	if (uPos != std::string::npos)
	{
		pInputPkt->m_Url = std::string(uri, 0, uPos);
		pInputPkt->m_Query = std::string(decode_uri, 
						uPos + 1, uri.size() - (uPos + 1));
	}
	else
	{
		pInputPkt->m_Url = std::string(uri);
	}

	return RET::SUC;
} 

//解析http请求头cookie体
void HttpParser::ParserCookie(std::string cookie)
{
	if (0 == cookie.size())
	{
		return;
	}
}

//解析http请求体
void HttpParser::ParserBody(std::string http_body)
{
	//异常判断
	if (0 == http_body.size())
	{
		return;
	}
}
