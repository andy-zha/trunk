#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include "RetCodeDefine.h"
#include "MemoryDefine.h"
#include "DList.h"
#include <pthread.h>

template<typename Type>
class HashSlot
{
	public:
		/*
		 * @brief 构造函数
		 */
		HashSlot()
		{
			pthread_mutex_init(&_mutex, NULL);
		}

		/*
		 * @brief 析构函数
		 */
		~HashSlot()
		{
			pthread_mutex_destroy(&_mutex);
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

		/*
		 * @brief 加锁
		 */
		void Lock()
		{
			pthread_mutex_lock(&_mutex);
		}

		/*
		 * @brief 去锁
		 */
		void UnLock()
		{
			pthread_mutex_unlock(_mutex);
		}

	public:
		/*
		 * @brief 业务链
		 */
		DList<Type> m_Hash_List;

		/*
		 * @brief 互斥锁
		 */
		pthread_mutex_t _mutex;
};

class HashTable
{
	public:
		/**
		 * @brief 构造函数
		 */
		HashTable()
		{}

		/**
		 * @brief 析构函数
		 */
		~HashTable()
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
	
		/**
		 * @brief 哈希表初始化
		 *
		 * @prame uNum 哈希业务总数 uHashSize 哈希大小数组
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t HashTableInit(uint32_t uNum, uHashSize[])
		{
			try
			{
				_pSlot = new HashSlot<Type>* [uNum];
			}
			catch(std::bad_alloc)
			{
				return RET::FAIL;
			}

			for (uint32_t uIndex = 0; uIndex < uNum; uIndex++)
			{
				try
				{
					_pSlot[uIndex] = new HashSlot<Type> [uHashSize[uIndex]];
				}
				catch(std::bad_alloc)
				{
					return RET::FAIL;
				}
			}

			return RET::SUC;
		}

		/**
		 * @brief 获取哈希槽地址
		 *
		 * @prame uIndex 哈希业务下标
		 *
		 * @return 槽结点地址
		 */
		HashSlot<Type> *FindHashTableList(uint32_t uIndex)
		{
			if (NULL != HashSlot[uIndex])
			{
				return HashSlot[uIndex];
			}

			return NULL;
		}

	public:
		/**
		 * @brief 哈希槽
		 */
		HashSlot<Type> **_pSlot;
};

#endif
