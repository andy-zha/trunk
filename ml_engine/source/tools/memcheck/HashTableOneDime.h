#ifndef _HASHTABLEONEDIME_H_
#define _HASHTABLEONEDIME_H_

#include "RetCodeDefine.h"
#include "HDList.h"
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
			pthread_mutex_unlock(&_mutex);
		}

	public:
		/*
		 * @brief 业务链
		 */
		HDList<Type> m_Hash_List;

		/*
		 * @brief 互斥锁
		 */
		pthread_mutex_t _mutex;
};

template<typename Type>
class HashTableOneDime
{
	public:
		/**
		 * @brief 构造函数
		 */
		HashTableOneDime()
		{}

		/**
		 * @brief 析构函数
		 */
		~HashTableOneDime()
		{}

		/**
		 * @brief 哈希表初始化
		 *
		 * @prame uNum 哈希业务总数 uHashSize 哈希大小数组
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t HashTableInit(uint32_t uNum)
		{
			try
			{
				_pSlot = new HashSlot<Type> [uNum];
			}
			catch(std::bad_alloc)
			{
				return RET::FAIL;
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
		HashSlot<Type> *FindHashSlot(uint32_t uIndex)
		{
			return &_pSlot[uIndex];
		}

	public:
		/**
		 * @brief 哈希槽
		 */
		HashSlot<Type> *_pSlot;
};

#endif
