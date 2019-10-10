#ifndef _CFGID_H_
#define _CFGID_H_

#include <iostream> 

#define CFG_MAX_DEPTH 5 

namespace NS_CONFIG
{
	typedef enum CFGID
	{
		EM_CFGID_START = -1,                //枚举保护
		EM_CFGID_SERVER_IP = 0,             //服务端ip
		EM_CFGID_SERVER_PORT,               //服务端port
		EM_CFGID_QUEUE_MAX_SIZE,            //缓冲区最大长度
		EM_CFGID_SITE_MAX_NUM,              //站点最大数
		EM_CFGID_FILTER_CFG_FILE,           //参数过滤配置路径
		EM_CFGID_REX_TOTAL,                 //正则表达式总个数
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
	//服务端ip
	{NS_CONFIG::EM_CFGID_SERVER_IP, "../etc/ml.json", {"PROCESSOR", "SERVER_IP", (const char *)0}},
	//服务端port
	{NS_CONFIG::EM_CFGID_SERVER_PORT, "../etc/ml.json", {"PROCESSOR", "SERVER_PORT", (const char *)0}},
	//缓冲区最大长度
	{NS_CONFIG::EM_CFGID_QUEUE_MAX_SIZE, "../etc/ml.json", {"PROCESSOR", "QUEUE_MAX_SIZE", (const char *)0}},

	//IPDRuleMgr
	//站点最大值
	{NS_CONFIG::EM_CFGID_SITE_MAX_NUM, "../etc/ml.json", {"IPDRULEMGR", "SITE_MAX_NUM", (const char *)0}},

	//COMMON
	//参数过滤规则文件路径
	{NS_CONFIG::EM_CFGID_FILTER_CFG_FILE, "../etc/ml.json", {"COMMON", "FILTER_CFG_FILE", (const char *)0}},

	//pattern.json
	//正则总个数
	{NS_CONFIG::EM_CFGID_REX_TOTAL, "../etc/pattern.json", {"PATTERNMGR", "TOTAL", (const char*)0}},
};

#endif 
