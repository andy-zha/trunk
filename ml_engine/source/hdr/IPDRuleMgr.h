#ifndef _IPDRULEMGR_H_
#define _IPDRULEMGR_H_

#include "MemoryDefine.h"
#include "RetCodeDefine.h"
#include "DbAdmin.h"
#include "Timer.h"
#include <vector>
#include <list>
#include <string>

namespace NS_IPDRULE
{
	//站点结点
	class SiteSlot
	{
		public:
			/**
			 * @brief 构造函数
			 */
			SiteSlot()
			{
				m_SiteId = 0;
				m_Port = 0;
				m_SiteKey = 0;
			}

			/**
			 * @brief 析构函数
			 */
			~SiteSlot()
			{}

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

		public:
			uint16_t m_SiteId;                                   //数据库站点id
			uint16_t m_Port;                                     //站点port
			uint64_t m_SiteKey;                                  //生成的站点键值
			std::string m_Ip;                                    //站点ip
			std::string m_Domain;                                //站点域名
	};

	//业务结点
	class BrSlot
	{
		public:
			/**
			 * @brief 构造函数
			 */
			BrSlot()
			{
				m_SiteKey = 0;
				m_Cycle = 0;
				m_LearnStatus = -1;
				m_Port = 0;
			}

			/**
			 * @brief 析构函数
			 */
			~BrSlot()
			{}

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

		public:
			uint8_t m_SiteKey;									//站点键值
			uint8_t m_Cycle;									//学习周期
			int8_t m_LearnStatus;								//学习状态
			uint16_t m_BusinessId;                              //数据库业务id
			uint16_t m_Port;                                    //注册port
			std::string m_Ip;                                   //注册ip
			std::string m_Domain;                               //注册域名
			std::list<std::string> m_TrustIpList;               //可信任ip列表
			std::list<std::string> m_UnTrustIpList;             //不可信任ip列表
			std::list<std::string> m_NotLearnUrlList;           //不学习url列表
			std::vector<NS_IPDRULE::SiteSlot> m_SiteTable;		//站点表	
	};
}

class IPDRuleMgr
{
	public:
		/**
		 * @brief 析构函数
		 */
		~IPDRuleMgr();

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
		 * @brief 获取单例
		 *
		 * @return 返回单例
		 */
		static IPDRuleMgr GetInstance()
		{
			static IPDRuleMgr _Instance;
			return _Instance;
		}

		/**
		 * @brief 初始化
		 *
		 * return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t Init();

		/**
		 * @brief 处理接口
		 */
		void Process();

#ifndef _GTEST_	
	private:
#endif
		/**
		 * @brief 构造函数
		 */
		IPDRuleMgr();

		/**
		 * @brief 查询业务函数
		 *
		 * @prame iter 当前业务槽
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t QueryBusiness(std::vector<NS_IPDRULE::BrSlot>::iterator iter);

		/**
		 * @brief 处理不学习url
		 *
		 * @prame pResult 查询结果; iter 当前业务槽
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t ProcessNotLearnUrl(MYSQL_RES *pResult, std::vector<NS_IPDRULE::BrSlot>::iterator iter);

		/**
		 * @brief 删除Url相关数据
		 *
		 * @prame BusinessId 业务Id; Url Url内容
		 */
		void DeleteUrlData(std::string BusinessId, std::string Url);

		/**
		 * @brief 删除Url表内Url记录
		 *
		 * @prame BusinessId 业务Id; SiteId 站点Id; UrlId UrlId 
		 */
		void DeleteUrl(std::string BusinessId, std::string SiteId, std::string UrlId);	

		/**
		 * @brief 删除Args表
		 *
		 * @prame BusinessId 业务Id; SiteId 站点Id; UrlId UrlId 
		 */
		void DropArgsTable(std::string BusinessId, std::string SiteId, std::string UrlId);	

	private:
		/**
		 * @brief 业务链
		 */
		std::vector<NS_IPDRULE::BrSlot> m_BusinessList;           

		/**
		 * @brief 数据库操作对象
		 */
		DbAdmin m_DbAdmin;

		/**
 		 * @brief 加载时间间隔
 		 */
		uint32_t m_LoadThreshold;
		
		/**
 		 * @brief 数据库连接失败次数(日志信息) 
 		 */
		uint32_t m_ConnectDbFailed;

		/**
 		 * @brief 数据库去连接失败次数(日志信息)
 		 */
		uint32_t m_CloseDbFailed;
	
		/**
 		 * @brief 执行数据库查询失败次数(日志信息)
 		 */
		uint32_t m_ExecQueryFailed;

		/**
 		 * @brief 上次活跃时间
 		 */
		uint64_t m_LastActiveTime;
};

#endif
