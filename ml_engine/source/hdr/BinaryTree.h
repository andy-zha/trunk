#ifndef _BINARYTREE_H_
#define _BINARYTREE_H_

#include "RetCodeDefine.h"
#include "MemoryDefine.h"

template<typename _Tree>
class BinaryTree
{
	public:
		/**
 		 * @brief 构造函数
 		 */
		BinaryTree():m_tree_left(nullptr), m_tree_right(nullptr)
		{}

		/**
		 * @brief 析构函数
		 */
		~BinaryTree()
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
		 * @brief 根结点插入左结点
		 *
		 * @prame pNode 结点
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t InsertLeftNode(BinaryTree<_Tree> *pNode)
		{
			//异常判断
			if (nullptr == pNode) {
				return RET::FAIL;
			}

			this->m_tree_left = pNode;
			return RET::SUC;
		}

		/**
		 * @brief 根结点插入右结点
		 *
		 * @prame pNode 结点
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t InsertRightNode(BinaryTree<_Tree> *pNode)
		{
			//异常判断
			if (nullptr == pNode) {
				return RET::FAIL;
			}

			this->m_tree_right = pNode;
			return RET::SUC;
		}

		/**
		 * @brief 从当前结点插入左结点
		 *
		 * @prame pCurNode 当前结点; pNewNode 新结点
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t InsertLeftNode(BinaryTree<_Tree> *pCurNode, BinaryTree<_Tree> *pNewNode)
		{
			//异常判断
			if (nullptr == pCurNode || nullptr == pNewNode) 
			{
				return RET::FAIL;
			}

			pCurNode->m_tree_left = pNewNode;
			return RET::SUC;
		}

		/**
		 * @brief 从当前结点插入右结点
		 *
		 * @prame pCurNode 当前结点; pNewNode 新结点
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t InsertRightNode(BinaryTree<_Tree> *pCurNode, BinaryTree<_Tree> *pNewNode)
		{
			//异常判断
			if (nullptr == pCurNode || nullptr == pNewNode) 
			{
				return RET::FAIL;
			}

			pCurNode->m_tree_right = pNewNode;
			return RET::SUC;
		}

	public:
		/**
		 * @brief 结点内容
		 */
		_Tree m_tree_data;

		/**
 		 * @brief 结点左指针
 		 */
		BinaryTree<_Tree> *m_tree_left;

		/**
 		 * @brief 结点右指针
 		 */
		BinaryTree<_Tree> *m_tree_right;
};

#endif
