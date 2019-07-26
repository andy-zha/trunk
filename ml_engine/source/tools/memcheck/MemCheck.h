#ifndef _MEMCHECK_H_
#define _MEMCHECK_H_

#include "RetCodeDefine.h"
#include <execinfo.h>
#include <cxxabi.h>
#include <unistd.h>
#include <string.h>
#include <map>
#include <list>

namespace NS_MEMCHECK
{
	class MemNode
	{
		public:
			/**
			 * @brief 构造函数
			 */
			MemNode()
			{}

			/**
			 * @brief 析构函数
			 */
			~MemNode()
			{}

		public:
			void *addr;              //地址信息
			void **Buf;              //堆栈信息
			int32_t size;            //堆栈层数
	};

	class MemSlot
	{
		public:
			/**
			 * @brief 构造函数
			 */
			MemSlot()
			{}

			/**
			 * @brief 析构函数
			 */
			~MemSlot()
			{}

		public:
			std::list<NS_MEMCHECK::MemNode> m_Memlist;
	};

	//哈希大小
	static const uint32_t HASHSIZE = 17777;
}

class MemCheck
{
	public:
		/**
		 * @brief 构造函数
		 */
		MemCheck();

		/**
		 * @brief 析构函数
		 */
		~MemCheck();

		/**
		 * @brief 获取单例
		 *
		 * @return 单例
		 */
		static MemCheck &GetInstance()
		{
			static MemCheck Instance;
			return Instance;
		}

		/**
 		 * @brief memcheck初始化
 		 *
 		 * @return RET::SUC 失败; RET::FAIL 失败
 		 */
		int32_t Init();

		/**
		 * @brief 内存分配
		 *
		 * @prame pAddr 地址信息
		 */
		void New(void *pAddr);

		/**
		 * @brief 内存释放
		 * 
		 * @prame pAddr 释放地址
		 */
		void Delete(void *pAddr);

		/**
		 * @brief 写日志
		 * 
		 * @prame RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t WriteLog();

	private:
		/**
		 * @brief 堆栈信息处理
		 *
		 * @prame buf 堆栈信息; size 堆栈层数
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t Translation(void *buf[128], int32_t size, FILE *fd);

	private:
		/**
 		 * @brief 哈希表1
 		 */
		std::map<uint32_t, NS_MEMCHECK::MemSlot> m_AddrMapOne;

		/**
 		 * @brief 哈希表2
 		 */
		std::map<uint32_t, NS_MEMCHECK::MemSlot> m_AddrMapTwo;

		/**
		 * @brief 结点信息转换败次数
		 */
		uint32_t m_TranslationFailed;
};

#endif
