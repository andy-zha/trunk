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
	
	typedef enum BUSINESSFEILD
	{
		EM_BUSINESS_FEILD_START = -1,                          //枚举保护
		EM_BUSINESS_ID,                                        //业务ID
		EM_BUSINESS_NAME,                                      //业务名称
		EM_BUSINESS_IP,                                        //业务IP
		EM_BUSINESS_PORT,                                      //业务port   
		EM_BUSINESS_DOMAIN,                                    //业务域名
		EM_BUSINESS_SITEKEY,                                   //站点键值
		EM_BUSINESS_CYCLE,                                     //学习周期
		EM_BUSINESS_AUTOCHECK,                                 //自动检测开关
		EM_BUSINESS_AUTODELETE,                                //自动删除开关
		EM_BUSINESS_AD_TIME,                                   //自动删除的时间阈值
		EM_BUSINESS_AD_REQNUM,                                 //自动删除的请求了阈值
		EM_BUSINESS_AUDIT_AC,                                  //告警频率异常的动作
		EM_BUSINESS_DEDIT_AC,                                  //阻断频率异常的动作
		EM_BUSINESS_LEARNSTATUS,                               //业务学习状态
		EM_BUSINESS_CHECKSTATUS,                               //业务检测状态
		EM_BUSINESS_FEILD_END                                  //枚举保护
	}EM_BUSINESSFEILD;
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
		int32_t ExecQuery(std::string Sql, MYSQL_RES *&pResult);

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
