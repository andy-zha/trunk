#ifndef _CONSOLEMGR_H_
#define _CONSOLEMGR_H_ 

#include "RetCodeDefine.h"
#include "MemoryDefine.h"
#include "DbAdmin.h"

class ConsoleMgr
{
	public:
		/**
		 * @brief 析构函数
		 */
		~ConsoleMgr();

		/**
		 * @brief 去除拷贝函数
		 */
		ConsoleMgr(ConsoleMgr const&) = delete;

		/**
		 * @brief 去除重载赋值
		 */
		void operator=(ConsoleMgr const&) = delete;
		
		/**
		 * @brief 获取单例
		 *
		 * @return 返回单例
		 */
		static ConsoleMgr &GetInstance()
		{
			static ConsoleMgr Instance;
			return Instance;
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
		 * @brief 初始化接口
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t Init();

		/**
 	     * @brief 开始接口
 	     *
 	     * @return RET::SUC 成功; RET::FAIL 失败
 	     */
		int32_t Start();

	private:
		/**
		 * @brief 构造函数
		 */
		ConsoleMgr();

		/**
 	     * @brief Memcheck 工具入口 
 	     */
		void MemCheckConsole();
	
	private:		
		/**
		 * @brief 数据库操作对象
		 */
		DbAdmin m_db;
};

#endif
