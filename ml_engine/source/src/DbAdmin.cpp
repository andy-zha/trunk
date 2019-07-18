#include "DbAdmin.h"

//构造函数
DbAdmin::DbAdmin()
{}

//析构函数
DbAdmin::~DbAdmin()
{}

//连接接口
int32_t DbAdmin::Connect()
{}

//断开连接接口
int32_t DbAdmin::Close()
{}

//执行查询语句接口
int32_t DbAdmin::ExecQuery(std::string Sql, MYSQL_RES *pResult)
{}

//执行语句接口
int32_t DbAdmin::ExecSql(std::string Sql)
{}
