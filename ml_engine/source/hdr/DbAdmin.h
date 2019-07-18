#ifndef _DBADMIN_H_
#define _DBADMIN_H_

#include "MemoryDefine.h"
#include "RetCodeDefine.h"
#include <mysql/mysql.h> 

class DbAdmin
{
	public:
		/**
		 * @brief
		 */
		DbAdmin();

		/**
		 * @brief
		 */
		~DbAdmin();

		/**
		 * @brief 连接接口
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t Connect();

		/**
		 * @brief 断开连接接口
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t Close();

		/**
		 * @brief 执行查询接口
		 *
		 * @prame Sql 执行语句; pResult 查询结果
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t ExecQuery(std::string Sql, MYSQL_RES *pResult);

		/**
		 * @brief 执行接口
		 *
		 * @prame Sql 执行语句
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t ExecSql(std::string Sql);
};

#endif
