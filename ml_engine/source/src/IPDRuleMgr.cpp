#include "IPDRuleMgr.h"

//构造函数
IPDRuleMgr::IPDRuleMgr()
{}

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
	//遍历业务槽结点
	std::vector<NS_IPDRULE::BrSlot>::iterator iter = m_BusinessList.begin();
	for (; iter != m_BusinessList.end(); iter++)
	{
		int32_t iRet = QueryBusiness(iter);
	}
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
		return QUERY::FAIL;
	}

	if (NULL == pResult || 0 == mysql_num_rows(pResult))
	{
		return QUERY::FAIL;
	}
}
