#include "HttpParser.h"

//构造函数
HttpParser::HttpParser()
{}

//析构函数
HttpParser::~HttpParser()
{}

/** 解析器初始化接口 **/
int32_t HttpParser::init()
{
	//状态机初始化
	if (RET::SUC != superMatchInit()) {
		return RET::FAIL;
	}

	return RET::SUC;
}

/** 状态机初始化 **/
int32_t HttpParser::superMatchInit()
{
	//读取规则路径
	std::string cfgFile;
	if (RET::SUC != Config::getCfg(NS_CONFIG::EM_CFGID_FILTER_CFG_FILE, cfgFile))
	{
		std::cout<<"HttpParser: Read cfgfile failed!"<<std::endl;
		return RET::FAIL;
	}

	//通用状态机构建
	if (RET::SUC != super.build(cfgFile, "ALL")) 
	{
		std::cout<<"HttpParser: Super init failed!"<<std::endl;
		return RET::FAIL;
	}

	//cookie状态机构建
	if (RET::SUC != cookieSuper.build(cfgFile, "COOKIE"))
	{
		std::cout<<"HttpParser: Cookie super init failed!"<<std::endl;
		return RET::FAIL;
	}

	//post状态机构建
	if (RET::SUC != postSuper.build(cfgFile, "POST"))
	{
		std::cout<<"HttpParser: Post super init failed!"<<std::endl;
		return RET::FAIL;
	}

	return RET::SUC;
}

/** 解析器主入口 **/
int32_t HttpParser::start(InputPacket *pInputPkt)
{
	//异常判断
	if (nullptr == pInputPkt || nullptr == pInputPkt->pStr
					|| 0 == pInputPkt->uLength)
	{
		return RET::FAIL;
	}

	//取出请求行和请求头
	uint32_t uHeaderlen = 0;
	char *pPos = strstr(pInputPkt->pStr, "\r\n");
	if (nullptr == pPos) {
		uHeaderlen = pInputPkt->uLength;
	}else {
		uHeaderlen = pPos - pInputPkt->pStr;
	}
	std::string http_header = std::string(pInputPkt->pStr, uHeaderlen);

	//取出请求体
	std::string http_body;
	if (nullptr != pPos && nullptr != (pPos + 4))
	{
		http_body = pPos;
	}

	//解析请求行与请求头
	const char *pMethod = nullptr;
	const char *pPath = nullptr;
	struct phr_header headers[32];
	size_t MethodLen = 0;
	size_t PathLen = 0;
	size_t HeaderNum = 32;
	if(-1 == phr_parse_request(http_header.data(), http_header.size(), &pMethod, 
							&MethodLen, &pPath, &PathLen, &pInputPkt->MinorVer, 
							headers, &HeaderNum, 0))
	{
		return RET::FAIL;
	}

	//取请求方法、Uri
	pInputPkt->m_HttpMethod = std::string(pMethod, MethodLen);

	//解析url
	std::string uri = std::string(pPath, PathLen);
	parserUri(uri, pInputPkt);

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
		if (key == NS_HTTPPARSER::HOST)	{
			pInputPkt->m_Host = std::string(value, value.size());
		}

		if (key == NS_HTTPPARSER::COOKIE) {
			cookie = value;
		}
	}

#ifndef _GTEST_
	//过滤规则
	if (RET::SUC != IPDRuleMgr::getInstance().MatchRules(pInputPkt))
	{
		return RET::FAIL;
	}
#endif

	//解析Query
	parserQuery(pInputPkt);

	//解析cookie体	
	parserCookie(cookie, pInputPkt);

	//解析http体
	parserBody(http_body, pInputPkt);

	return RET::SUC;
}

/** 解析uri **/
int32_t HttpParser::parserUri(std::string uri, InputPacket *pInputPkt)
{
	//size为0直接返回
	if (uri.empty()) {
		return RET::FAIL;
	}

	std::string Url;
	//decode解码
	std::string decode_uri = StrProc::UrlDecode(uri);
	//按?进行切割,取url/query
	uint32_t uPos = decode_uri.find('?');
	if (uPos != std::string::npos) {
		Url = std::string(uri, 0, uPos);
		pInputPkt->m_Query = std::string(decode_uri, uPos + 1, uri.size() - (uPos + 1));
	}else {
		Url = std::string(uri);
	}

	//一般情况下不会带域名
	std::string ParserUrl = Url;
	if (!Url.empty() && Url.at(0) != '/')
	{
		bool fullDomain = true;
		uint32_t uPosSch = Url.find(':') + 1;
		if (uPosSch != std::string::npos)
		{
			uint32_t uPosNet = Url.find("//", uPosSch) + 2;
			if (uPosNet != std::string::npos)
			{
				uint32_t uPosPath = Url.find('/', uPosNet);
				if (uPosPath != std::string::npos)
				{
					ParserUrl = Url.substr(uPosPath);
				}
			}
		}
	}

	pInputPkt->m_Url = filterUrl(ParserUrl);

	return RET::SUC;
}

/** 过滤url以分号为分隔符后面的请求数据 **/
std::string HttpParser::filterUrl(std::string url)
{
	uint32_t uPos = url.find(';');
	if (std::string::npos == uPos) {
		return url;
	}

	std::string ret = std::string(url, 0, uPos);
	return ret;
} 

/** 解析请求行中的Query **/
int32_t HttpParser::parserQuery(InputPacket *pInputPkt)
{
	//异常判断
	if (nullptr == pInputPkt || pInputPkt->m_Query.empty()) 
	{
		return RET::SUC;
	}

	//解析query
	std::string kv;
	std::string key;
	std::string value;
	uint32_t uPos = 0;
	uint32_t uOffset = 0;
	uint32_t uLength = pInputPkt->m_Query.size();
	while (uOffset < uLength) {
		//切割结构
		uPos = pInputPkt->m_Query.find("&", uOffset + 1);
		kv = std::string(pInputPkt->m_Query, uOffset, uPos - uOffset - 1);

		//偏移更新
		uOffset = uPos;

		//解析kv结构
		uPos = kv.find("=");
		if (std::string::npos == uPos) {
			continue;
		}

		//取key值
		key = std::string(kv, 0, uPos);

		//过滤不学习参数
		if (RET::SUC == super.softFuzzyMatch(key)) {
			continue;
		}

		//取value值
		value = std::string(kv, uPos + 1);
	}

	return RET::SUC;
}

/** 解析http请求头cookie体 **/
int32_t HttpParser::parserCookie(std::string cookie, InputPacket *pInputPkt)
{
	//异常判断
	if (nullptr == pInputPkt || cookie.empty()) 
	{
		return RET::SUC;
	}

	return RET::SUC;
}

/** 解析http请求体 **/
int32_t HttpParser::parserBody(std::string http_body, InputPacket *pInputPkt)
{
	//异常判断
	if (nullptr == pInputPkt || http_body.empty()) 
	{
		return RET::SUC;
	}

	return RET::SUC;
}
