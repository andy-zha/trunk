#ifndef _PATTERNMGR_H_
#define _PATTERNMGR_H_

#include "RetCodeDefine.h"
#include "MemoryDefine.h"
#include "Config.h"
#include <map>
#include <pcre.h>

namespace NS_PATTERNMGR
{
	class Pattern
	{
		public:
			/**
			 * @brief 构造函数
			 */
			Pattern()
			{}

			/**
			 * @brief 析构函数
			 */
			~Pattern()
			{}

		public:
			std::string m_name;                      //名称
			std::string m_pattern;                   //模式
			pcre *m_reg;
			pcre_extra *m_extra;
			int32_t m_captures;			
	};
}

/** 模式管理器类 **/
class PatternMgr
{
	public:
		/**
		 * @brief 析构函数
		 */
		~PatternMgr();

		/**
		 * @brief 去除复制构造函数与拷贝函数
		 */
		PatternMgr(PatternMgr const &) = delete;
		void operator=(PatternMgr const &) = delete;

		/**
		 * @brief 获取单例
		 *
		 * @return 单例
		 */
		static PatternMgr &getInstance()
		{
			static PatternMgr _instance;
			return _instance;
		}

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
		 * @brief 模式管理器初始化
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t init();

	private:
		/**
		 * @brief 构造函数
		 */
		PatternMgr();

		/**
		 * @brief 正则入库
		 *
		 * @prame name 正则名称; pattern 正则; uPatId 正则主键
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t writeToDb(std::string name, std::string pattern, uint16_t &uPatId);

	private:
		/**
		 * @brief 模式总个数
		 */
		int32_t m_rexTotal;

		/**
		 * @brief 模式表
		 */
		std::map<uint16_t, NS_PATTERNMGR::Pattern> m_pattern;

		/**
		 * @brief 
		 */
		bool m_jitenabled;

};

#endif  
