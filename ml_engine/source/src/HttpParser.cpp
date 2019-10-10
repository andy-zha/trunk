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
	std::cout<<"header: "<<http_header<<std::endl;
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
		//return RET::FAIL;
	}
#endif

	uint32_t uHashKey = 10;
	HashSlot<HashNode> *pSlot = HashTableMgr::getInstance().findHashSlot(uHashKey);
	if (nullptr != pSlot) {
		//上锁
		pSlot->lock();

		DList<HashNode> *pList = nullptr;
		if (RET::SUC != HashTableMgr::getInstance().findHashList(pSlot, pList))
		{
			pSlot->unLock();
			return RET::FAIL;
		}

		DList<HashNode> *pNode = nullptr;
		while (RET::SUC == pList->ForwardTraver(pNode)) {
			if (RET::SUC == compareMessage(pNode, pInputPkt))
			{
				//统计请求次数
				pNode->m_dlist_data.m_reqNum++;

				//统计请求方法
				statisticalMethod(pNode, pInputPkt);

				//解析Query
				parserQuery(pInputPkt);

				//解析cookie体	
				parserCookie(cookie, pInputPkt);

				//解析http体
				parserBody(http_body, pInputPkt);
				
				//去锁
				pSlot->unLock();
				return RET::SUC;
			}
		}

		//检查模型状态
		if (RET::SUC == checkModelStatus())
		{
			pSlot->unLock();
			return RET::SUC;
		}
		
		//新建结点
		createNewNode(pList, pInputPkt, cookie, http_body); 

		//去锁
		pSlot->unLock();
		return RET::SUC;
	}

	return RET::FAIL;
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

/** 比较结点信息 **/
int32_t HttpParser::compareMessage(DList<HashNode> *pNode, InputPacket *pInputPkt)
{
	if (pNode->m_dlist_data.m_businessId != pInputPkt->m_businessId
					|| pNode->m_dlist_data.m_siteId != pInputPkt->m_siteId
					|| pNode->m_dlist_data.m_url != pInputPkt->m_Url)
	{
		return RET::FAIL;
	}

	return RET::SUC;
}

/** 统计学习方法 **/
int32_t HttpParser::statisticalMethod(DList<HashNode> *&pNode, InputPacket *pInputPkt)
{
	//异常判断
	if (nullptr == pNode || nullptr == pInputPkt)
	{
		return RET::FAIL;
	}

	//统计标准请求方法
	uint32_t uIndex = 0;
	while (uIndex < NS_HTTPPARSER::EM_METHOD_END) {
		if (pInputPkt->m_HttpMethod == NS_HTTPPARSER::HttpMethod[uIndex])
		{
			pNode->m_dlist_data.m_methodList[uIndex]++;
			return RET::SUC;
		}
		uIndex++;
	}

	//统计非标请求方法
	std::vector<NS_HASHNODE::Method>::iterator iter = pNode->m_dlist_data.m_espMethodList.begin();
	for (; iter != pNode->m_dlist_data.m_espMethodList.end(); iter++)
	{
		if (pInputPkt->m_HttpMethod == iter->m_method)
		{
			iter->m_total++;
			return RET::SUC;
		} 
	}

	NS_HASHNODE::Method method;
	method.m_method = pInputPkt->m_HttpMethod;
	method.m_total = 1;
	pNode->m_dlist_data.m_espMethodList.push_back(method);
	return RET::SUC;
}

/** 查询模型状态 **/
int32_t HttpParser::checkModelStatus()
{
	return RET::FAIL;
}

/** 创建新的结点 **/
int32_t HttpParser::createNewNode(DList<HashNode> *&pList, InputPacket *pInputPkt, 
				std::string cookie, std::string http_body)
{
	//异常判断
	if (nullptr == pList || nullptr == pInputPkt)
	{
		return RET::FAIL;
	}

	//分配新结点内存
	DList<HashNode> *pNode = nullptr;
	try {
		pNode = new DList<HashNode>;
	} catch (std::bad_alloc) {
		return RET::FAIL;
	}

	//数据库连接
	DbAdmin db;
	if (RET::SUC != db.connect()) {
		if (nullptr != pNode) {
			delete pNode;
			pNode = nullptr;
		}
		return RET::FAIL;
	}

	//插入url记录
	std::vector<std::string> args;
	args.push_back(pInputPkt->m_Url);
	args.push_back(pInputPkt->m_Url);
	uint64_t nowtime = Timer::getLocalTime();
	std::string sql = "INSERT INTO url_" + pInputPkt->m_businessId + "_" 
			+ pInputPkt->m_siteId + "(u_id, name, status, learn_process, \
			learn_rate, hits, cycle_hits, readonly, lastactivetime, reliability) \
			SELECT 0, ?, 1, 0, 0, 0, 0, 0, " + std::to_string(nowtime) + ", 0 FROM \
			DUAL WHERE NOT EXISTS (SELECT u_id FROM url_" + pInputPkt->m_businessId
			+ "_" + pInputPkt->m_siteId + " WHERE name = ?);";
	db.stmtExecSql(sql, args);

	//查询uid
	std::vector<std::string> _args;
	_args.push_back(pInputPkt->m_Url);
	std::vector<std::vector<std::string>> res;	
	sql = "SELECT u_id FROM url_" + pInputPkt->m_businessId + "_" + pInputPkt->m_siteId
			+ " WHERE name = ?;";
	if (RET::SUC != db.stmtExecQuery(sql, _args, 1, res))
	{
		if (nullptr != pNode) {
			delete pNode;
			pNode = nullptr;
		}

		db.close();
		return RET::FAIL;
	}

	//创建args表
	std::string argsTable = "args_" + pInputPkt->m_businessId + "_"
			+ pInputPkt->m_siteId + "_" + res[0][0];
	createArgsTable(argsTable);

	//数据库连接
	db.close();

	//赋值业务id、站点id、url及urlid
	uint16_t uIndex = 0;
	pNode->m_dlist_data.m_businessId = pInputPkt->m_businessId;
	pNode->m_dlist_data.m_siteId = pInputPkt->m_siteId;
	pNode->m_dlist_data.m_url = pInputPkt->m_Url;
	pNode->m_dlist_data.m_urlId = res[0][0];
	pNode->m_dlist_data.m_reqNum = 1;
	while (uIndex < NS_HTTPPARSER::EM_METHOD_END) {
		pNode->m_dlist_data.m_methodList[uIndex] = 0;
		uIndex++;
	}

	//统计学习方法
	statisticalMethod(pNode, pInputPkt);

	//解析Query
	parserQuery(pInputPkt);

	//解析cookie体	
	parserCookie(cookie, pInputPkt);

	//解析http体
	parserBody(http_body, pInputPkt);

	//入链
	pList->TailAddNode(pNode);

	return RET::SUC;
}

/** 创建args表 **/
int32_t HttpParser::createArgsTable(std::string argsTable)
{
	return RET::SUC;
}
