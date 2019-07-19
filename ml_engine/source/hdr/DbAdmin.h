#ifndef _DBADMIN_H_
#define _DBADMIN_H_

#include "MemoryDefine.h"
#include "RetCodeDefine.h"
#include <mysql/mysql.h> 

namespace NS_DBADMIN
{
	static const char * HOST = "127.0.0.1";                    //本地地址
	static const char * USER = "root";                         //用户名
	static const char * PASSWD = "andy123321";                 //密码
	static const char * DBNAME = "ml_engine";                  //数据库名
}

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
		 * @brief 重载 内存分配
		 *
		 * @prame size 内存大小
		 *
		 * @return 内存地址
		 */
		static void * operator new(size_t size)
		{
			void *p = (void*)_MEM_NEW_(size);
			return p;
		}

		/**
		 * @brief 重载 内存释放
		 *
		 * @prame p 释放地址
		 */
		static void operator delete(void *p)
		{
			_MEM_DEL_(p);
		}

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

	private:
		/**
 		 * @brief 数据库操作对象
 		 */
		MYSQL *conn;
};

#endif
