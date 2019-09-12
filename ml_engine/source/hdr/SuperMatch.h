#ifndef _SUPERMATCH_H_
#define _SUPERMATCH_H_

#include "MemoryDefine.h"
#include "RetCodeDefine.h"
#include "BinaryTree.h"
#include <vector>
#include <string>

namespace NS_SUPERMATCH
{
	class Pattern
	{
		public:
			/**
			 * @brief 构造函数
			 */
			Pattern()
			{
				m_total = 0;
			}

			/**
			 * @brief 析构函数
			 */
			~Pattern()
			{}

		public:
			char m_ch;                  //字符
			uint32_t m_total;           //字符被查找次数
	};
}

class SuperMatch
{
	public:
		/**
		 * @brief 构造函数
		 */
		SuperMatch();

		/**
		 * @brief 析构函数
		 */
		~SuperMatch();

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
		 * @brief 构建状态机
		 *
		 * @prame pattern 所有模式串
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t Build(std::vector<std::string> pattern);

		/**
		 * @brief 查找字符模式 
		 *
		 * @prame ch 字符; pCurNode 当前结点
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t FindPattern(char ch, BinaryTree<NS_SUPERMATCH::Pattern> *&pCurNode);

		/**
		 * @brief 模式完全匹配
		 *
		 * @prame str 字符串
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t MatchPattern(std::string str);

		/**
		 * @brief 模式子串匹配
		 *
		 * @prame str 字符串
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t SubMatchPattern(std::string str);

		/**
		 * @brief 模式模糊匹配
		 *
		 * @prame str 字符串
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t FuzzyMatchPattern(std::string str);

		/**
		 * @brief 查找字符模式(不区分大小写) 
		 *
		 * @prame ch 字符; pCurNode 当前结点
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t SoftFindPattern(char ch, BinaryTree<NS_SUPERMATCH::Pattern> *&pCurNode);

		/**
		 * @brief 模式完全匹配(不区分大小写)
		 *
		 * @prame str 字符串
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t SoftMatch(std::string str);

		/**
		 * @brief 模式子串匹配(不区分大小写)
		 *
		 * @prame str 字符串
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t SoftSubMatch(std::string str);

		/**
		 * @brief 模式模糊匹配(不区分大小写)
		 *
		 * @prame str 字符串
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t SoftFuzzyMatch(std::string str);

		/**
 		 * @brief 排序整合
 		 *
 		 * @return RET::SUC 成功; RET::FAIL 失败
 		 */
		int32_t Sort2Swap();

		/**
 		 * @brief 销毁状态机
 		 *
 		 * @return RET::SUC 成功; RET::FAIL 失败
 		 */
		int32_t Destroy();

	private:
		/**
		 * @brief 模式树
		 */
		BinaryTree<NS_SUPERMATCH::Pattern> m_PatternTree;
 
};

#endif 
