#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include "DList.h"
#include "MemoryDefine.h"
#include "RetCodeDefine.h"
#include <pthread.h>
#include <stdlib.h>

/** 槽节点类 **/
template<typename T>
class HashSlot
{
	public:
		/**
		 * @brief 构造函数
		 */
		HashSlot()
		{
			pthread_mutex_init(&mutex, nullptr);
		}

		/**
		 * @brief 析构函数
		 */
		~HashSlot()
		{
			pthread_mutex_destroy(&mutex);
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
		 * @brief 尝试上锁
		 */
		int32_t tryLock()
		{
			return pthread_mutex_trylock(&mutex);
		}

		/**
		 * @brief 上锁
		 */
		void lock()
		{
			pthread_mutex_lock(&mutex);
		}

		/**
		 * @brief 去锁
		 */
		void unLock()
		{
			pthread_mutex_unlock(&mutex);
		}

	public:
		/**
		 * @brief 业务链  
		 */
		DList<T> m_hashList;

		/**
		 * @brief 互斥锁
		 */
		pthread_mutex_t mutex;
};

/** 哈希表操作类 **/
template<typename T>
class HashTable
{
	public:
		/**
		 * @brief 构造函数
		 */
		HashTable()
		{
			m_hashSlot = nullptr;
		}

		/**
		 * @brief 析构函数
		 */
		~HashTable()
		{
			if (nullptr != m_hashSlot) {
				delete m_hashSlot;
				m_hashSlot = nullptr;
			}
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
		 * @brief 哈希表初始化
		 *
		 * @prame uHashSize 哈希表大小
		 *
		 * return RET::SUC 成功/RET::FAIL 失败
		 */
		int32_t hashTableInit(uint32_t uSize)
		{
			try	{
				m_hashSlot = new HashSlot<T> [uSize];
			} catch(std::bad_alloc) {
				return RET::FAIL;
			}

			m_hashSize = uSize;
			return RET::SUC;
		}

		/**
		 * @brief 查找槽节点
		 *
		 * @prame uHashKey 哈希值；pSlot 槽节点指针
		 *
		 * return RET::SUC 成功/RET::FAIL 失败
		 */
		HashSlot<T>* findHashSlot(uint32_t uKey)
		{
			//判断key值是否越界
			if (uKey >= m_hashSize || nullptr == m_hashSlot)
			{
				return nullptr;
			}

			return &m_hashSlot[uKey];
		}

	private:
		/**
		 * @brief 哈希槽
		 */
		HashSlot<T> *m_hashSlot;
		
		/**
		 * @brief 哈希表大小存储
		 */
		uint32_t m_hashSize;
};

#endif
