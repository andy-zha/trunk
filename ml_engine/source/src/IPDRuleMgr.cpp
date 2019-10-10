#include "IPDRuleMgr.h"

//构造函数
IPDRuleMgr::IPDRuleMgr()
{
	m_SiteNumMax = 0;
	m_LoadThreshold = 0;
	m_ConnectDbFailed = 0;
	m_CloseDbFailed = 0;
	m_ExecQueryFailed = 0;
	m_LastActiveTime = 0;
}

//析构函数
IPDRuleMgr::~IPDRuleMgr()
{}

//初始化接口
int32_t IPDRuleMgr::Init()
{
	//读取站点最大值
	int32_t iValue = -1;
	if (RET::SUC != Config::getCfg(NS_CONFIG::EM_CFGID_SITE_MAX_NUM, iValue))
	{
		std::cout<<"IPDRuleMgr: Read site max failed!"<<std::endl;
		return RET::FAIL;
	}
	m_SiteNumMax = iValue;

	//数据库连接
	if (RET::SUC != m_DbAdmin.connect())
	{
		std::cout<<"IPDRuleMgr: Db connect failed!"<<std::endl;
		return RET::FAIL;
	}

	//创建业务表
	if (RET::SUC != CreateBusinessTable())
	{
		std::cout<<"IPDRuleMgr: Create business table failed!"<<std::endl;
		return RET::FAIL;
	}

	//创建信任ip表
	if (RET::SUC != CreateTrustIpTable())
	{
		std::cout<<"IPDRuleMgr: Create trust ip table failed!"<<std::endl;
		return RET::FAIL;
	}

	//创建非信任ip表
	if (RET::SUC != CreateUnTrustIpTable())
	{
		std::cout<<"IPDRuleMgr: Create untrust ip table failed!"<<std::endl;
		return RET::FAIL;
	}

	//创建不学习url表
	if (RET::SUC != CreateNotLearnUrlTable())
	{
		std::cout<<"IPDRuleMgr: Create not learn url table failed!"<<std::endl;
		return RET::FAIL;
	}

	if (RET::SUC != m_DbAdmin.close()) 
	{
		std::cout<<"IPDRuleMgr: Close database failed!"<<std::endl;
		return RET::FAIL;
	}

	return RET::SUC;
}

//主处理接口
void IPDRuleMgr::process()
{
	uint64_t Nowtime = Timer::getLocalTime();
	//未到加载时间不加载
	if (Nowtime - m_LastActiveTime < m_LoadThreshold)
	{
		return;
	}

	//数据库连接
	if (RET::SUC != m_DbAdmin.connect())
	{
		m_ConnectDbFailed++;
		return;
	}

	//遍历业务槽结点
	std::vector<NS_IPDRULE::BrSlot>::iterator iter = m_BusinessList.begin();
	for (; iter != m_BusinessList.end(); iter++)
	{
		//查询业务
		QueryBusiness(iter);
	}

	//查询新业务
	QueryNewBusiness();

	//数据库去连接
	if (RET::SUC != m_DbAdmin.close())
	{
		m_CloseDbFailed++;
	}

	m_LastActiveTime = Nowtime;
	return;
}

//查询业务
int32_t IPDRuleMgr::QueryBusiness(std::vector<NS_IPDRULE::BrSlot>::iterator iter)
{
#ifdef _GTEST_
	m_DbAdmin.connect();
#endif

	//加载不学习Url、信任IP、不信任IP
	LoadNotLearnUrl(iter); 
	LoadTrustIp(iter);
	LoadUnTrustIp(iter);

	//加载学习周期、自动删除配置、业务学习状态
	MYSQL_RES *pResult = nullptr;
	std::string Sql = "SELECT cycle, d_auto, d_time, d_num FROM business \
					   WHERE b_id = " + iter->m_BusinessId + ";";
	if (RET::SUC != m_DbAdmin.execQuery(Sql, pResult) || nullptr == pResult)
	{
		m_ExecQueryFailed++;

#ifdef _GTEST_
		m_DbAdmin.close();
#endif

		return RET::FAIL;
	}

	if (0 == mysql_num_rows(pResult))
	{
		DestroyBusiness(iter);
		mysql_free_result(pResult);
		pResult = nullptr;
	}

	MYSQL_ROW row = mysql_fetch_row(pResult);
	if (nullptr != row)
	{
		//加载学习周期
		if (nullptr != row[0])
		{
			iter->m_Cycle = std::stoul(row[0]);
		}

		//加载自动删除配置
		LoadAutoDeleteConfig(row, iter);
	}

	//内存释放
	mysql_free_result(pResult);
	pResult = nullptr;

	//更新站点状态
	UpdateSite(iter);

#ifdef _GTEST_
		m_DbAdmin.close();
#endif

	return RET::SUC;
}

//加载不学习url
int32_t IPDRuleMgr::LoadNotLearnUrl(std::vector<NS_IPDRULE::BrSlot>::iterator iter)
{
	//读取不学习的url
	MYSQL_RES *pResult = nullptr;
	std::string Sql = "SELECT url FROM not_learn_url WHERE b_id = " 
		+ iter->m_BusinessId + ";";
	if (RET::SUC != m_DbAdmin.execQuery(Sql, pResult))
	{
		m_ExecQueryFailed++;
		return RET::FAIL;
	}

	if (nullptr != pResult && 0 != mysql_num_rows(pResult))
	{
		ProcessNotLearnUrl(pResult, iter);
		mysql_free_result(pResult);
		pResult = nullptr;
	}

	return RET::SUC;
}

//处理不学习url
int32_t IPDRuleMgr::ProcessNotLearnUrl(MYSQL_RES *pResult, std::vector<NS_IPDRULE::BrSlot>::iterator iter)
{
	if (nullptr == pResult)
	{
		return RET::FAIL;
	}

	uint32_t uTuples = mysql_num_rows(pResult);
	for (uint32_t uIndex = 0; uIndex < uTuples; uIndex++)
	{
		MYSQL_ROW row = mysql_fetch_row(pResult);
		if (nullptr != row && nullptr != row[0])
		{
			bool bFlag = false;
			std::list<std::string>::iterator it = iter->m_NotLearnUrlList.begin();
			for (; it != iter->m_NotLearnUrlList.end(); it++)
			{
				if (row[0] == *it)
				{
					bFlag = true;
					break;
				}
			}
		
			//新增不学习Url删除该Url的记录	
			if (!bFlag)
			{
				iter->m_NotLearnUrlList.push_back(row[0]);
				DeleteUrlData(iter->m_BusinessId, row[0]);
			}
		}
	}
	
	return RET::SUC;
}

//删除url相关数据
void IPDRuleMgr::DeleteUrlData(std::string BusinessId, std::string Url)
{
	if (0 == Url.size())
	{
		return;
	}

	//查询当前业务下所有站点
	MYSQL_RES *pResult = nullptr;
	std::string Sql = "SELECT s_id FROM site_" + BusinessId + ";";
	if (RET::SUC != m_DbAdmin.execQuery(Sql, pResult) || nullptr == pResult)
	{
		m_ExecQueryFailed++;
		return;	
	}

	uint32_t uTuples = mysql_num_rows(pResult);
	for (uint32_t uIndex = 0; uIndex < uTuples; uIndex++)
	{
		MYSQL_ROW row = mysql_fetch_row(pResult);
		if (nullptr != row && nullptr != row[0])
		{
			//查询站点下UrlId
			MYSQL_RES *pUrl = nullptr;
			Sql = "SELECT u_id FROM url_" + BusinessId + "_" + row[0]
					+ " WHERE name = '" + Url + ";";
			if (RET::SUC != m_DbAdmin.execQuery(Sql, pUrl) || nullptr == pUrl)
			{
				m_ExecQueryFailed++;
				continue;
			}

			MYSQL_ROW uid = mysql_fetch_row(pUrl);
			if (nullptr != uid && nullptr != uid[0])
			{
				//删除Url表内的Url记录及其相关联的args表
				DropArgsTable(BusinessId, row[0], uid[0]);
				DeleteUrl(BusinessId, row[0], uid[0]);
				mysql_free_result(pUrl);
				pUrl = nullptr;
				break;
			}
			
			mysql_free_result(pUrl);
			pUrl = nullptr;
		}
	}

	mysql_free_result(pResult);
	pResult = nullptr;	
	return;
}

//删除Url表内的Url记录
void IPDRuleMgr::DeleteUrl(std::string BusinessId, std::string SiteId, std::string UrlId)
{
	std::string Sql = "DELETE FROM url_" + BusinessId + "_" + SiteId + " WHERE u_id = "
			+ UrlId + ";";
	m_DbAdmin.execSql(Sql);
}

//删除Args表
void IPDRuleMgr::DropArgsTable(std::string BusinessId, std::string SiteId, std::string UrlId)
{
	std::string Sql = "DROP TABLE args_" + BusinessId + "_" + SiteId + "_" + UrlId + ";";
	m_DbAdmin.execSql(Sql);
}

//注销业务
int32_t IPDRuleMgr::DestroyBusiness(std::vector<NS_IPDRULE::BrSlot>::iterator iter)
{
	//清除节点
	m_BusinessList.erase(iter);
	//TODO 清除缓存
}

int32_t IPDRuleMgr::LoadTrustIp(std::vector<NS_IPDRULE::BrSlot>::iterator iter)
{
	MYSQL_RES *pResult = nullptr;
	std::string Sql = "SELECT ip FROM trustip WHERE b_id = " + iter->m_BusinessId + ";";
	if (RET::SUC != m_DbAdmin.execQuery(Sql, pResult) || nullptr == pResult)
	{
		m_ExecQueryFailed++;
		return RET::FAIL;
	}

	uint32_t uTuples = mysql_num_rows(pResult);
	if (0 == uTuples)
	{
		mysql_free_result(pResult);
		pResult = nullptr;
		return RET::SUC;
	}

	iter->m_TrustIpList.clear();
	for (uint32_t uIndex = 0; uIndex < uTuples; uIndex++)
	{
		MYSQL_ROW row = mysql_fetch_row(pResult);
		if (nullptr != row && nullptr != row[0])
		{
			iter->m_TrustIpList.push_back(row[0]);
		}
	}

	mysql_free_result(pResult);
	pResult = nullptr;
	return RET::SUC;
}

int32_t IPDRuleMgr::LoadUnTrustIp(std::vector<NS_IPDRULE::BrSlot>::iterator iter)
{
	MYSQL_RES *pResult = nullptr;
	std::string Sql = "SELECT ip FROM untrustip WHERE b_id = " 
			+ iter->m_BusinessId + ";";
	if (RET::SUC != m_DbAdmin.execQuery(Sql, pResult) || nullptr == pResult)
	{
		m_ExecQueryFailed++;
		return RET::FAIL;
	}

	uint32_t uTuples = mysql_num_rows(pResult);
	if (0 == uTuples)
	{
		mysql_free_result(pResult);
		pResult = nullptr;
		return RET::SUC;
	}

	iter->m_UnTrustIpList.clear();
	for (uint32_t uIndex = 0; uIndex < uTuples; uIndex++)
	{
		MYSQL_ROW row = mysql_fetch_row(pResult);
		if (nullptr != row && nullptr != row[0])
		{
			iter->m_TrustIpList.push_back(row[0]);
		}
	}

	mysql_free_result(pResult);
	pResult = nullptr;
	return RET::SUC;
}

int32_t IPDRuleMgr::LoadAutoDeleteConfig(MYSQL_ROW row, 
				std::vector<NS_IPDRULE::BrSlot>::iterator iter)
{
	//异常判断
	if (nullptr == row)
	{
		return RET::FAIL;
	}

	if (nullptr != row[1])
	{
		if (0 == std::stoul(row[0]))
		{
			iter->m_AutoDelConfig.m_Flag = true;
			if (nullptr != row[2])
			{
				iter->m_AutoDelConfig.m_Cycle = std::stoul(row[2]);
			}

			if (nullptr != row[3])
			{
				iter->m_AutoDelConfig.m_ReqNum = std::stoul(row[3]);
			}
		}
		else
		{
			iter->m_AutoDelConfig.m_Flag = false;
		}
	}

	return RET::SUC; 
}

int32_t IPDRuleMgr::UpdateSite(std::vector<NS_IPDRULE::BrSlot>::iterator iter)
{
	std::vector<NS_IPDRULE::SiteSlot>::iterator it = iter->m_SiteTable.begin();
	for (; it != iter->m_SiteTable.end(); it++)
	{
		//查询站点状态
		MYSQL_RES *pResult = nullptr;
		std::string Sql = "SELECT learn_status FROM site_" + iter->m_BusinessId
				+ " WHERE s_id = " + it->m_SiteId + ";";
		if (RET::SUC != m_DbAdmin.execQuery(Sql, pResult) || nullptr == pResult)
		{
			m_ExecQueryFailed++;
			continue;
		}

		if (0 != mysql_num_rows(pResult))
		{
			MYSQL_ROW row = mysql_fetch_row(pResult);
			if (nullptr != row[0])
			{
				it->m_LearnStatus = std::stoul(row[0]);
			}
		}
		
		//释放内存
		mysql_free_result(pResult);
		pResult = nullptr;
		
		//处理自动删除
		ProcessAutoDelete();
	}

	return RET::SUC;
}

int32_t IPDRuleMgr::ProcessAutoDelete()
{}

//查询新业务
int32_t IPDRuleMgr::QueryNewBusiness()
{
#ifdef _GTEST_
	m_DbAdmin.connect();
#endif
	
	std::string BusinessIdMax = "0";
	if (0 < m_BusinessList.size())
	{
		BusinessIdMax = m_BusinessList.end()->m_BusinessId;
	}

	//业务id比当前最大业务id大的进行注册
	MYSQL_RES *pResult = nullptr;
	std::string Sql = "SELECT * FROM business WHERE b_id > " 
			+ BusinessIdMax + ";";
	if (RET::SUC != m_DbAdmin.execQuery(Sql, pResult))
	{
		m_ExecQueryFailed++;

#ifdef _GTEST_
		m_DbAdmin.close();
#endif

		return RET::FAIL;
	}

	uint32_t uTuples = mysql_num_rows(pResult);
	if (0 == uTuples)
	{
		mysql_free_result(pResult);
		pResult = nullptr;

#ifdef _GTEST_
		m_DbAdmin.close();
#endif

		return RET::SUC;
	}

	//循环注册新业务
	for (uint32_t uIndex = 0; uIndex < uTuples; uIndex++)
	{
		MYSQL_ROW row = mysql_fetch_row(pResult);
		if (nullptr != row)
		{
			//注册业务
			RegisterBusiness(row);
		}
	}

	mysql_free_result(pResult);
	pResult = nullptr;

#ifdef _GTEST_
		m_DbAdmin.close();
#endif

	return RET::SUC;
}

//注册业务
int32_t IPDRuleMgr::RegisterBusiness(MYSQL_ROW row)
{
	//异常判断
	if (nullptr == row)
	{
		return RET::FAIL;
	}

	//构建业务结点
	NS_IPDRULE::BrSlot _Slot;

	//业务id、业务ip、业务port、业务域名、站点key、学习周期均不能为null
	if (nullptr == row[NS_DBADMIN::EM_BUSINESS_ID] 
					|| nullptr == row[NS_DBADMIN::EM_BUSINESS_IP] 
					|| nullptr == row[NS_DBADMIN::EM_BUSINESS_PORT] 
					|| nullptr == row[NS_DBADMIN::EM_BUSINESS_DOMAIN] 
					|| nullptr == row[NS_DBADMIN::EM_BUSINESS_SITEKEY] 
					|| nullptr == row[NS_DBADMIN::EM_BUSINESS_CYCLE])
	{
		return RET::FAIL;
	}
	_Slot.m_BusinessId = row[NS_DBADMIN::EM_BUSINESS_ID];
	_Slot.m_Ip = row[NS_DBADMIN::EM_BUSINESS_IP];
	_Slot.m_Port = std::stoul(row[NS_DBADMIN::EM_BUSINESS_PORT]);
	_Slot.m_Domain = row[NS_DBADMIN::EM_BUSINESS_DOMAIN];
	_Slot.m_SiteKey = std::stoul(row[NS_DBADMIN::EM_BUSINESS_SITEKEY]);
	_Slot.m_Cycle = std::stoul(row[NS_DBADMIN::EM_BUSINESS_CYCLE]); 

	//读取自动删除配置
	if (nullptr != row[NS_DBADMIN::EM_BUSINESS_AUTODELETE])
	{
		if (0 == std::stoul(row[NS_DBADMIN::EM_BUSINESS_AUTODELETE]))
		{
			_Slot.m_AutoDelConfig.m_Flag = true;
			if (nullptr != row[NS_DBADMIN::EM_BUSINESS_AD_TIME])
			{
				_Slot.m_AutoDelConfig.m_Cycle = std::stoul(row[NS_DBADMIN::EM_BUSINESS_AD_TIME]);
			}

			if (nullptr != row[NS_DBADMIN::EM_BUSINESS_AD_REQNUM])
			{
				_Slot.m_AutoDelConfig.m_ReqNum = std::stoul(row[NS_DBADMIN::EM_BUSINESS_AD_REQNUM]);
			}
		}
		else
		{
			_Slot.m_AutoDelConfig.m_Flag = false;
		}
	}

	//插入业务结点
	m_BusinessList.push_back(_Slot);
	return RET::SUC;
}

//规则匹配
int32_t IPDRuleMgr::MatchRules(InputPacket *&pInputPkt)
{
	//异常判断
	if (nullptr == pInputPkt)
	{
		return RET::FAIL;
	}

	//业务匹配
	std::vector<NS_IPDRULE::BrSlot>::iterator iter;
	if (RET::SUC != MatchBusiness(pInputPkt, iter))
	{
		return RET::FAIL;
	}

	//站点匹配
	if (RET::SUC != MatchSite(pInputPkt, iter))
	{
		return RET::FAIL;
	}

	return RET::SUC;
}

//匹配业务
int32_t IPDRuleMgr::MatchBusiness(InputPacket *&pInputPkt,
				std::vector<NS_IPDRULE::BrSlot>::iterator &iterswap)
{
	if (nullptr == pInputPkt)
	{
		return RET::FAIL;
	}

	float fDegree = 0.0;
	std::string BusinessId;
	std::vector<NS_IPDRULE::BrSlot>::iterator iter = m_BusinessList.begin();
	for (; iter != m_BusinessList.end(); iter++)
	{
		//匹配业务且业务学习状态不能为学习暂停
		if (RET::SUC == CompareBusiness(pInputPkt, iter))
		{
			if (fDegree < iter->m_Degree)
			{
				BusinessId = iter->m_BusinessId;
				fDegree = iter->m_Degree;
				iterswap = iter;
			}
		}
	}

	//无匹配业务则失败
	if (0.0 == fDegree || RET::SUC == CompareConfig(pInputPkt, iterswap))
	{
		return RET::FAIL;
	}

	pInputPkt->m_BusinessId = BusinessId;
	return RET::SUC;
}

//匹配业务信息
int32_t IPDRuleMgr::CompareBusiness(InputPacket *&pInputPkt,
				std::vector<NS_IPDRULE::BrSlot>::iterator iter)
{
	//异常判断
	if (nullptr == pInputPkt)
	{
		return RET::FAIL;
	}

	//比较ip/port/domain
	if ((0 != iter->m_Ip.size() 
					&& RET::SUC != CompareIp(iter->m_Ip, pInputPkt->m_ServerIp))
					|| (0 != iter->m_Port && iter->m_Port != pInputPkt->m_ServerPort)
					|| (0 != iter->m_Domain.size() 
							&& RET::SUC != CompareDomain(iter->m_Domain, pInputPkt->m_Host)))
	{
		return RET::FAIL;
	}

	return RET::SUC;
}

//匹配业务内非信任ip及不学习url配置
int32_t IPDRuleMgr::CompareConfig(InputPacket *&pInputPkt, 
				std::vector<NS_IPDRULE::BrSlot>::iterator iter)
{
	//异常检测
	if (nullptr == pInputPkt)
	{
		return RET::FAIL;
	}

	//匹配不信任ip
	std::list<std::string>::iterator IpIter = iter->m_UnTrustIpList.begin();
	for (; IpIter != iter->m_UnTrustIpList.end(); IpIter++)
	{
		if (pInputPkt->m_ClientIp == *IpIter)
		{
			return RET::SUC;
		}
	}

	//匹配不学习url
	std::list<std::string>::iterator UrlIter = iter->m_NotLearnUrlList.begin();
	for (; UrlIter != iter->m_NotLearnUrlList.end(); UrlIter++)
	{
		if (pInputPkt->m_Url == *UrlIter)
		{
			return RET::SUC;
		}
	}

	return RET::FAIL;
}

int32_t IPDRuleMgr::MatchSite(InputPacket *&pInputPkt,
				std::vector<NS_IPDRULE::BrSlot>::iterator iter)
{
	//异常判断
	if (nullptr == pInputPkt)
	{
		return RET::FAIL;
	}

	//本进程内存中已存在站点匹配
	std::vector<NS_IPDRULE::SiteSlot>::iterator SiteIter = iter->m_SiteTable.begin();
	for (; SiteIter != iter->m_SiteTable.end(); SiteIter++)
	{
		//站点信息匹配及站点学习状态不为学习暂停
		if (RET::SUC == CompareSite(pInputPkt, SiteIter)
						&& 0 != SiteIter->m_LearnStatus)
		{
			pInputPkt->m_SiteId = SiteIter->m_SiteId;
			return RET::SUC;
		}
	}

	//查询数据库站点数是否达到最大数
	if (RET::SUC != m_DbAdmin.connect())
	{
		m_ConnectDbFailed++;
		return RET::FAIL;
	}

	//查询当前业务总站点数
	MYSQL_RES *pResult = nullptr;
	std::string Sql = "SELECT COUNT(*) FROM site_" + iter->m_BusinessId + ";";
	if (RET::SUC != m_DbAdmin.execQuery(Sql, pResult) || nullptr == pResult)
	{
		m_ExecQueryFailed++;
		m_DbAdmin.close();
		return RET::FAIL;
	}

	if (0 != mysql_num_rows(pResult))
	{
		MYSQL_ROW row = mysql_fetch_row(pResult);
		//站点数超过了最大数不增加新站点
		if (std::stoul(row[0]) >= m_SiteNumMax)
		{
			mysql_free_result(pResult);
			pResult = nullptr;
			m_DbAdmin.close();
			return RET::FAIL;
		}

		//创建新站点
		mysql_free_result(pResult);
		pResult = nullptr;
		CreateNewSite(pInputPkt, iter);
		m_DbAdmin.close();
		return RET::SUC;
	}

	mysql_free_result(pResult);
	pResult = nullptr;
	m_DbAdmin.close();
	return RET::FAIL;
}

//比较站点信息
int32_t IPDRuleMgr::CompareSite(InputPacket *&pInputPkt,
				std::vector<NS_IPDRULE::SiteSlot>::iterator iter)
{
	//异常判断
	if (nullptr == pInputPkt)
	{
		return RET::FAIL;
	}

	//比较站点信息
	if ((0 != iter->m_Ip.size() 
			&& RET::SUC != CompareIp(pInputPkt->m_ServerIp,iter->m_Ip))
			|| (0 != iter->m_Port && pInputPkt->m_ServerPort != iter->m_Port)
			|| (0 != iter->m_Domain.size() 
				&& RET::SUC != CompareDomain(pInputPkt->m_Host, iter->m_Domain)))
	{
		return RET::SUC;
	}

	return RET::FAIL;
}

int32_t IPDRuleMgr::CreateNewSite(InputPacket *&pInputPkt,
				std::vector<NS_IPDRULE::BrSlot>::iterator iter)
{
	if (nullptr == pInputPkt)
	{
		return RET::FAIL;
	}

	std::string Sql = "INSERT INTO site_" + iter->m_BusinessId + "(s_id int, \
					   dip text, d_port int, host text, hits int, learnstatus int, \
					   checkstatus int) SELECT 0, ";
}

//创建业务表
int32_t IPDRuleMgr::CreateBusinessTable()
{
	//创建表语句
	std::string Sql = "CREATE TABLE IF NOT EXISTS business(b_id int primary \
					   key auto_increment, name text, ip text, port int, \
					   host text, s_key int, cycle int, c_auto int, d_auto int, \
					   d_time int, d_num int, audit_ac int, dedit_ac int);";
	if (RET::SUC != m_DbAdmin.execSql(Sql))
	{
		return RET::FAIL;
	}

	//设置主键为1开始
	std::string Alter = "ALTER TABLE business auto_increment = 1;";
	if (RET::SUC != m_DbAdmin.execSql(Alter))
	{
		return RET::FAIL;
	}

	return RET::SUC;
}

//创建信任ip表
int32_t IPDRuleMgr::CreateTrustIpTable()
{
	//创建表语句
	std::string Sql = "CREATE TABLE IF NOT EXISTS trustip(ip text, b_id int);"; 
	if (RET::SUC != m_DbAdmin.execSql(Sql))
	{
		return RET::FAIL;
	}

	return RET::SUC;
}

//创建不信任ip表
int32_t IPDRuleMgr::CreateUnTrustIpTable()
{
	//创建表语句
	std::string Sql = "CREATE TABLE IF NOT EXISTS untrustip(ip text, b_id int);"; 
	if (RET::SUC != m_DbAdmin.execSql(Sql))
	{
		return RET::FAIL;
	}

	return RET::SUC;
}

//创建信任ip表
int32_t IPDRuleMgr::CreateNotLearnUrlTable()
{
	//创建表语句
	std::string Sql = "CREATE TABLE IF NOT EXISTS not_learn_url(url text, b_id int);"; 
	if (RET::SUC != m_DbAdmin.execSql(Sql))
	{
		return RET::FAIL;
	}

	return RET::SUC;
}

//比较ip是否相同
int32_t IPDRuleMgr::CompareIp(std::string ip, std::string _ip)
{
	//异常判断
	if (0 == ip.size() || 0 == _ip.size()) {
		return RET::FAIL;
	}

	//ip相同直接返回
	if (ip == _ip) {
		return RET::SUC;
	}

	//不包含*通配符或者ip长度小于非通配字符长度返回失败
	uint32_t uPos = _ip.find("/");
	if (std::string::npos == uPos) 
	{
		return RET::FAIL;
	}

	return RET::SUC;
}

//比较域名是否相同
int32_t IPDRuleMgr::CompareDomain(std::string domain, std::string _domain)
{
	//异常判断
	if (0 == domain.size() || 0 == _domain.size()) {
		return RET::FAIL;
	}

	//domain相同直接返回
	if (domain == _domain) {
		return RET::SUC;
	}

	//不包含*通配符或者domain长度小于非通配字符长度返回失败
	uint32_t uPos = _domain.find("*");
	if (std::string::npos == uPos || domain.size() < uPos) 
	{
		return RET::FAIL;
	}

	std::string Varone;
	std::string Vartwo;
	//取非通匹部分
	if (0 == uPos) {
		if (domain.size() < _domain.size() - 1) {
			return RET::FAIL;
		} 

		uint32_t start = domain.size() - _domain.size() + 1;
		Varone = std::string(domain, start);
		Vartwo = std::string(_domain, 1);
	} else {
		Varone = std::string(domain, 0, uPos);
		Vartwo = std::string(_domain, 0, uPos);
	}

	//比较非通配部分是否相同
	if (Varone != Vartwo) {
		return RET::FAIL;
	}

	return RET::SUC;
}
