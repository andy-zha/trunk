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
		QueryBusiness(iter);
	}

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
	//读取不学习的url
	MYSQL_RES *pResult = NULL;
	std::string Sql = "SELECT url FROM not_learn_url WHERE b_id = " 
		+ std::to_string(iter->m_BusinessId) + ";";
	if (RET::SUC != m_DbAdmin.ExecQuery(Sql, pResult))
	{
		m_ExecQueryFailed++;
	}

	if (NULL != pResult && 0 != mysql_num_rows(pResult))
	{
		ProcessNotLearnUrl(pResult, iter);
		mysql_free_result(pResult);
		pResult = NULL;
	}
}

//处理不学习url
int32_t IPDRuleMgr::ProcessNotLearnUrl(MYSQL_RES *pResult, std::vector<NS_IPDRULE::BrSlot>::iterator iter)
{
	if (NULL == pResult)
	{
		return RET::FAIL;
	}

	uint32_t uTuples = mysql_num_rows(pResult);
	for (uint32_t uIndex = 0; uIndex < uTuples; uIndex++)
	{
		MYSQL_ROW row = mysql_fetch_row(pResult);
		if (NULL != row && NULL != row[0])
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

void IPDRuleMgr::DeleteUrlData(std::string BusinessId, std::string Url)
{
	if (0 == Url.size())
	{
		return;
	}

	//查询当前业务下所有站点
	MYSQL_RES *pResult = NULL;
	std::string Sql = "SELECT s_id FROM site_" + BusinessId + ";";
	if (RET::SUC != m_DbAdmin.ExecQuery(Sql, pResult) || NULL == pResult)
	{
		m_ExecQueryFailed++;
		return;	
	}

	uint32_t uTuples = mysql_num_rows(pResult);
	for (uint32_t uIndex = 0; uIndex < uTuples; uIndex++)
	{
		MYSQL_ROW row = mysql_fetch_row(pResult);
		if (NULL != row && NULL != row[0])
		{
			//查询站点下UrlId
			MYSQL_RES *pUrl = NULL;
			Sql = "SELECT u_id FROM url_" + BusinessId + "_" + row[0]
					+ " WHERE name = '" + Url + ";";
			if (RET::SUC != m_DbAdmin.ExecQuery(Sql, pUrl) || NULL == pUrl)
			{
				m_ExecQueryFailed++;
				continue;
			}

			MYSQL_ROW uid = mysql_fetch_row(pUrl);
			if (NULL != uid && NULL != uid[0])
			{
				//删除Url表内的Url记录及其相关联的args表
				DropArgsTable(BusinessId, row[0], uid[0]);
				DeleteUrl(BusinessId, row[0], uid[0]);
				break;
			}
		}
	}
	
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
