#ifndef _HASHTABLEMGR_H_
#define _HASHTABLEMGR_H_

#include "HashTable.h"

class HashTableMgr
{
	public:
		/**
		 * @brief 析构函数
		 */
		~HashTableMgr();

		/**
		 * @brief 去除拷贝函数
		 */
		HashTableMgr(HashTableMgr const&) = delete;

		/**
		 * @brief 去除重载赋值
		 */
		void operator=(HashTableMgr const&) = delete;
		
		/**
		 * @brief 获取单例
		 *
		 * @return 返回单例
		 */
		static HashTableMgr &getInstance()
		{
			static HashTableMgr Instance;
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
		 * @brief 重载 内存释放
		 *
		 * @prame p 释放地址
		 */
		static void operator delete(void *p)
		{
			_MEM_DEL_(p);
		}

		/**
		 * @brief 初始化接口
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t init();

	private:
		/**
		 * @brief 构造函数
		 */
		HashTableMgr();

	private:
		/**
		 * @brief 哈希业务总个数
		 */
		uint32_t m_hashNum;

};

#endif
