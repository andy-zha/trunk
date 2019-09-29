#include "DbAdmin.h"

//构造函数
DbAdmin::DbAdmin()
{
	conn = nullptr;
}

//析构函数
DbAdmin::~DbAdmin()
{}

//连接接口
int32_t DbAdmin::connect()
{
	try
	{
		conn = mysql_init(nullptr);
		if (!mysql_real_connect(conn, NS_DBADMIN::HOST, NS_DBADMIN::USER, 
			NS_DBADMIN::PASSWD, NS_DBADMIN::DBNAME, 0, nullptr, CLIENT_FOUND_ROWS))
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
int32_t DbAdmin::close()
{
	if (nullptr == conn)
	{
		return RET::SUC;
	}

	try
	{
		mysql_close(conn);
		mysql_library_end();
		conn = nullptr;
	}
	catch(...)
	{
		return RET::FAIL;
	}

	return RET::SUC;
}

//执行查询语句接口
int32_t DbAdmin::execQuery(std::string Sql, MYSQL_RES *&pResult)
{
	if (nullptr == conn)
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
int32_t DbAdmin::execSql(std::string Sql)
{
	if (nullptr == conn)
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

int32_t DbAdmin::stmtExecQuery(std::string Sql, std::vector<std::string> Vec, 
				uint32_t uCount, std::vector<std::vector<std::string>> &Res)
{
	//异常判断
	uint32_t param_count = Vec.size();
	if (0 == Sql.size() || 0 == uCount 
					|| 0 == param_count || nullptr == conn)
	{
		return RET::FAIL;
	}

	//初始化预处理环境句柄  MYSQL_STMT *stmt
	MYSQL_STMT *stmt = mysql_stmt_init(conn);
	if (nullptr == stmt)
	{
		return RET::FAIL;
	}

	//向预处理环境句柄stmt 中添加sql( 带有占位符)
	if (mysql_stmt_prepare(stmt, Sql.c_str(), Sql.size()))
	{
		mysql_stmt_close(stmt);
		return RET::FAIL;
	}

	//获取sql语言中占位符的个数
	if (param_count != mysql_stmt_param_count(stmt))
	{
		mysql_stmt_close(stmt);
		return RET::FAIL;
	}

	//初始化bind数组
	MYSQL_BIND bind[param_count];
	memset(bind, 0, sizeof(bind));
	for (uint32_t uIndex = 0; uIndex < param_count; uIndex++)
	{
		bind[uIndex].buffer_type = MYSQL_TYPE_STRING;
		bind[uIndex].buffer = (char*)(Vec[uIndex].c_str());
		bind[uIndex].is_null = nullptr;
		bind[uIndex].buffer_length = Vec[uIndex].size();
	}
	
	//bind buffers
	if (mysql_stmt_bind_param(stmt, bind))
	{
		mysql_stmt_close(stmt);
		return RET::FAIL;
	}

	//绑定result
	char tmp[uCount][1024];
	MYSQL_BIND result[uCount];
	memset(result, 0, sizeof(result));
	for (uint32_t ui = 0; ui < uCount; ui++)
	{
		result[ui].buffer_type = MYSQL_TYPE_STRING;
		result[ui].buffer = tmp[ui];
		result[ui].buffer_length = sizeof(tmp);
	}

	if (mysql_stmt_bind_result(stmt, result))
	{
		mysql_stmt_close(stmt);
		return RET::FAIL;
	}

	//执行语句
	if (mysql_stmt_execute(stmt))
	{
		mysql_stmt_close(stmt);
		return RET::FAIL;
	}

	//取出查询结果
	if (mysql_stmt_store_result(stmt))
	{
		mysql_stmt_close(stmt);
		return RET::FAIL;
	}

	while (0 == mysql_stmt_fetch(stmt))
	{
		std::vector<std::string> res;
		for (uint32_t uIndex = 0; uIndex < uCount; uIndex++)
		{
			std::string bak = tmp[uIndex];
			res.push_back(bak);
		}

		Res.push_back(res);
	}

	//释放内存，关闭句柄
	mysql_stmt_close(stmt);
	return RET::SUC;
}

int32_t DbAdmin::stmtExecSql(std::string Sql, std::vector<std::string> Vec)
{
	uint32_t param_count = Vec.size();
	if (0 == param_count || nullptr == conn)
	{
		return RET::FAIL;
	}

	//初始化预处理环境句柄MYSQL_STMT *stmt
	MYSQL_STMT *stmt = mysql_stmt_init(conn);
	if (nullptr == stmt)
	{
		return RET::FAIL;
	}

	//向预处理环境句柄stmt中添加sql(带有占位符)
	if (mysql_stmt_prepare(stmt, Sql.c_str(), Sql.size()))
	{
		mysql_stmt_close(stmt);
		return RET::FAIL;
	}

	//获取sql语言中占位符的个数
	if (param_count != mysql_stmt_param_count(stmt))
	{
		mysql_stmt_close(stmt);
		return RET::FAIL;
	}

	//初始化bind数组
	MYSQL_BIND bind[param_count];
	for (uint32_t uIndex = 0; uIndex < param_count; uIndex++)
	{
		bind[uIndex].buffer_type = MYSQL_TYPE_STRING;
		bind[uIndex].buffer = (char*)(Vec[uIndex].c_str());
		bind[uIndex].is_null = nullptr;
		unsigned long Strlen = Vec[uIndex].size();
		bind[uIndex].length = &Strlen;
	}
	
	//bind buffers
	if (mysql_stmt_bind_param(stmt, bind))
	{
		mysql_stmt_close(stmt);
		return RET::FAIL;
	}

	//执行语句
	if (mysql_stmt_execute(stmt))
	{
		mysql_stmt_close(stmt);
		return RET::FAIL;
	}

	mysql_stmt_close(stmt);
	return RET::SUC;
}
