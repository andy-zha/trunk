#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "RetCodeDefine.h"
#include "MemoryDefine.h"
#include "yajl/yajl_tree.h"
#include "CfgId.h"
#include <fstream>


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
		static int32_t getCfg(NS_CONFIG::EM_CFGID CfgId, int32_t &iValue);

		/**
		 * @brief 获取配置值(string型)
		 *
		 * @prame CfgId 配置id; Value 配置值
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		static int32_t getCfg(NS_CONFIG::EM_CFGID CfgId, std::string &Value);

		/**
		 * @brief 获取配置值(string型)
		 *
		 * @prame CfgId 配置id; Value 配置值
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		static int32_t getCfg(std::string CfgFile, const char **CfgPath, std::string &Value);

		/**
		 * @brief 获取配置值(string型)
		 *
		 * @prame CfgId 配置id; Value 配置值
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		static int32_t getCfg(std::string CfgFile, const char **CfgPath, int32_t &iValue);

		/**
		 * @brief 获取文件流
		 *
		 * @prame filename 指定文件名
		 *
		 * @return 文件流
		 */
		static std::string getStringFromFile(std::string filename);
};

#endif
