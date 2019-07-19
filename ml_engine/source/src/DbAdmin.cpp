#include "DbAdmin.h"

//构造函数
DbAdmin::DbAdmin()
{
	conn = NULL;
}

//析构函数
DbAdmin::~DbAdmin()
{}

//连接接口
int32_t DbAdmin::Connect()
{
	try
	{
		conn = mysql_init(NULL);
		if (!mysql_real_connect(conn, NS_DBADMIN::HOST, NS_DBADMIN::USER, 
			NS_DBADMIN::PASSWD, NS_DBADMIN::DBNAME, 0, NULL, CLIENT_FOUND_ROWS))
		{
			return RET::FAIL;
		}
	}
	catch(...)
	{
		return RET::FAIL;
	}

	return RET::SUC;
}

//断开连接接口
int32_t DbAdmin::Close()
{
	if (NULL == conn)
	{
		return RET::SUC;
	}

	try
	{
		mysql_close(conn);
		mysql_library_end();
		conn = NULL;
	}
	catch(...)
	{
		return RET::FAIL;
	}

	return RET::SUC;
}

//执行查询语句接口
int32_t DbAdmin::ExecQuery(std::string Sql, MYSQL_RES *pResult)
{
	if (NULL == conn)
	{
		return RET::FAIL;
	}

	try
	{
		if (0 != mysql_query(conn, Sql.c_str()))
		{
			return RET::FAIL;
		}
	}
	catch(...)
	{	
		return RET::FAIL;
	}

	pResult = mysql_store_result(conn);
	return RET::SUC;
}

//执行语句接口
int32_t DbAdmin::ExecSql(std::string Sql)
{
	if (NULL == conn)
	{
		return RET::FAIL;
	}

	try
	{
		if (0 != mysql_query(conn, Sql.c_str()))
		{
			return RET::FAIL;
		}
	}
	catch(...)
	{	
		return RET::FAIL;
	}
	
	return RET::SUC;
}
