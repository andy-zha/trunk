#ifndef _HASHTABLEMGR_H_
#define _HASHTABLEMGR_H_

#include "HashNode.h"
#include "HashTable.h"

#define HASH_MAX_NUM 128

/** 哈希管理器命名空间 **/
namespace NS_HASHTABLEMGR
{
	//哈希业务id注册
	typedef enum HASH_INDEX
	{
		EM_HASH_INDEX_START = -1,                                 //哈希业务下标起始保护接口
		EM_HASH_LEARNER,                                          //学习缓存哈希表大小
		EM_HASH_INDEX_END                                         //哈希业务下标结尾保护接口
	}EM_HASH_INDEX;
}

/** 哈希管理器类 **/
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

		/**
		 * @brief 哈希业务注册接口
		 *
		 * @prame uHashIndex 业务下标; uHashSize 哈希大小
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t registerFunc(uint32_t uHashIndex, uint32_t uHashSize);

		/**
		 * @brief 查找哈希槽结点
		 *
		 * @prame uHashIndex 哈希业务下标; uHashKey 哈希值
		 *
		 * @return 哈希槽结点指针
		 */
		HashSlot<HashNode> *findHashSlot(uint32_t uHashIndex, uint32_t uHashKey);

		/**
		 * @brief 查找哈希链
		 *
		 * @prame pSlot 槽结点地址; pList 哈希链
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t findHashList(HashSlot<HashNode> *pSlot, DList<HashNode> *&pList);

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

		/**
		 * @brief 哈希size表
		 */
		uint32_t m_hashSize[HASH_MAX_NUM];

		/**
		 * @brief 哈希表
		 */
		HashTable<HashNodeBase*> m_hashTable;

};

#endif
