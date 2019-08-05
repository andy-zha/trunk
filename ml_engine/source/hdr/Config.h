#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "RetCodeDefine.h"
#include "MemoryDefine.h"
#include "yajl/yajl_tree.h"
#include <fstream>

#define CFG_MAX_DEPTH 5 

namespace NS_CONFIG
{
	typedef enum CFGID
	{
		EM_CFGID_START = -1,                //枚举保护
		EM_CFGID_SERVER_IP = 0,             //服务端ip
		EM_CFGID_SERVER_PORT,               //服务端port
		EM_CFGID_SITE_MAX_NUM,              //站点最大数
		EM_CFGID_END                        //枚举保护
	}EM_CFGID;
	
	typedef struct CfgInfo
	{
		NS_CONFIG::EM_CFGID CfgId;                      //配置id
		const char* filename;                           //配置文件名
		const char* cfgpath[CFG_MAX_DEPTH];             //配置路径 
	}CfgInfo;
}

//配置信息数组
static const NS_CONFIG::CfgInfo g_stCfgInfo[] = {
	//Processor
	{NS_CONFIG::EM_CFGID_SERVER_IP, "../etc/ml.json", {"PROCESSOR", "SERVER_IP", (const char *)0}},
	{NS_CONFIG::EM_CFGID_SERVER_PORT, "../etc/ml.json", {"PROCESSOR", "SERVER_PORT", (const char *)0}},

	//IPDRuleMgr
	{NS_CONFIG::EM_CFGID_SITE_MAX_NUM, "../etc/ml.json", {"IPDRULEMGR", "SITE_MAX_NUM", (const char *)0}},
};

class Config
{
	public:
		/**
		 * @brief 构造函数
		 */
		Config();
		
		/**
		 * @brief 析构函数
		 */
		~Config();

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
		 * @brief 获取配置值(int32_t类型)
		 *
		 * @prame CfgId 配置id; iValue 配置值
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		static int32_t GetCfg(NS_CONFIG::EM_CFGID CfgId, int32_t &iValue);

		/**
		 * @brief 获取配置值(string型)
		 *
		 * @prame CfgId 配置id; Value 配置值
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		static int32_t GetCfg(NS_CONFIG::EM_CFGID CfgId, std::string &Value);

		/**
		 * @brief 获取文件流
		 *
		 * @prame filename 指定文件名
		 *
		 * @return 文件流
		 */
		static std::string GetStringFromFile(std::string filename);
};

#endif
