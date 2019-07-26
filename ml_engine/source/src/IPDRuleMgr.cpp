#include "IPDRuleMgr.h"

//构造函数
IPDRuleMgr::IPDRuleMgr()
{
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
	return RET::SUC;
}

//主处理接口
void IPDRuleMgr::Process()
{
	uint64_t Nowtime = Timer::GetLocalTime();
	//未到加载时间不加载
	if (Nowtime - m_LastActiveTime < m_LoadThreshold)
	{
		return;
	}

	//数据库连接
	if (RET::SUC != m_DbAdmin.Connect())
	{
		m_ConnectDbFailed++;
		return;
	}

	//遍历业务槽结点
	std::vector<NS_IPDRULE::BrSlot>::iterator iter = m_BusinessList.begin();
	for (; iter != m_BusinessList.end(); iter++)
	{
		//上锁
		iter->Lock();

		//查询业务
		QueryBusiness(iter);

		//去锁
		iter->UnLock();
	}

	//查询新业务
	QueryNewBusiness();

	//数据库去连接
	if (RET::SUC != m_DbAdmin.Close())
	{
		m_CloseDbFailed++;
	}

	m_LastActiveTime = Nowtime;
	return;
}

//查询业务
int32_t IPDRuleMgr::QueryBusiness(std::vector<NS_IPDRULE::BrSlot>::iterator iter)
{
	//加载不学习Url、信任IP、不信任IP
	LoadNotLearnUrl(iter); 
	LoadTrustIp(iter);
	LoadUnTrustIp(iter);

	//加载学习周期、自动删除配置、业务学习状态
	MYSQL_RES *pResult = nullptr;
	std::string Sql = "SELECT cycle, d_auto, d_time, d_num, learn_status FROM business \
					   WHERE b_id = " + std::to_string(iter->m_BusinessId) + ";";
	if (RET::SUC != m_DbAdmin.ExecQuery(Sql, pResult) || nullptr == pResult)
	{
		m_ExecQueryFailed++;
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
		
		//加载学习状态
		if (nullptr != row[4])
		{
			iter->m_LearnStatus = std::stoul(row[4]);
		}
	}

	//内存释放
	mysql_free_result(pResult);
	pResult = nullptr;

	//更新站点状态
	UpdateSite(iter);

	return RET::SUC;
}

//加载不学习url
int32_t IPDRuleMgr::LoadNotLearnUrl(std::vector<NS_IPDRULE::BrSlot>::iterator iter)
{
	//读取不学习的url
	MYSQL_RES *pResult = nullptr;
	std::string Sql = "SELECT url FROM not_learn_url WHERE b_id = " 
		+ std::to_string(iter->m_BusinessId) + ";";
	if (RET::SUC != m_DbAdmin.ExecQuery(Sql, pResult))
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
				DeleteUrlData(std::to_string(iter->m_BusinessId), row[0]);
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
	if (RET::SUC != m_DbAdmin.ExecQuery(Sql, pResult) || nullptr == pResult)
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
			if (RET::SUC != m_DbAdmin.ExecQuery(Sql, pUrl) || nullptr == pUrl)
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
	m_DbAdmin.ExecSql(Sql);
}

//删除Args表
void IPDRuleMgr::DropArgsTable(std::string BusinessId, std::string SiteId, std::string UrlId)
{
	std::string Sql = "DROP TABLE args_" + BusinessId + "_" + SiteId + "_" + UrlId + ";";
	m_DbAdmin.ExecSql(Sql);
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
	std::string Sql = "SELECT ip FROM trustip WHERE b_id = " 
			+ std::to_string(iter->m_BusinessId) + ";";
	if (RET::SUC != m_DbAdmin.ExecQuery(Sql, pResult) || nullptr == pResult)
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
			+ std::to_string(iter->m_BusinessId) + ";";
	if (RET::SUC != m_DbAdmin.ExecQuery(Sql, pResult) || nullptr == pResult)
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

int32_t IPDRuleMgr::LoadAutoDeleteConfig(MYSQL_ROW row, std::vector<NS_IPDRULE::BrSlot>::iterator iter)
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
		std::string Sql = "SELECT learn_status FROM site_" + std::to_string(iter->m_BusinessId)
				+ " WHERE s_id = " + std::to_string(it->m_SiteId) + ";";
		if (RET::SUC != m_DbAdmin.ExecQuery(Sql, pResult) || nullptr == pResult)
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
	uint32_t uBusinessIdMax = 0;
	if (0 < m_BusinessList.size())
	{
		uBusinessIdMax = m_BusinessList.end()->m_BusinessId;
	}

	//业务id比当前最大业务id大的进行注册
	MYSQL_RES *pResult = nullptr;
	std::string Sql = "SELECT * FROM business WHERE b_id > " 
			+ std::to_string(uBusinessIdMax) + ";";
	if (RET::SUC != m_DbAdmin.ExecQuery(Sql, pResult))
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
	_Slot.m_BusinessId = std::stoul(row[NS_DBADMIN::EM_BUSINESS_ID]);
	_Slot.m_Ip = row[NS_DBADMIN::EM_BUSINESS_IP];
	_Slot.m_Port = std::stoul(row[NS_DBADMIN::EM_BUSINESS_PORT]);
	_Slot.m_Domain = std::stoul(row[NS_DBADMIN::EM_BUSINESS_DOMAIN]);
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

	//读取业务学习状态
	if (nullptr != row[NS_DBADMIN::EM_BUSINESS_LEARNSTATUS])
	{
		_Slot.m_LearnStatus = std::stoul(row[NS_DBADMIN::EM_BUSINESS_LEARNSTATUS]);
	}

	//插入业务结点
	m_BusinessList.push_back(_Slot);
	return RET::SUC;
}
