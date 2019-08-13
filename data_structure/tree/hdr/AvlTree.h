#ifndef _AVL_TREE_H_
#define _AVL_TREE_H_

#include "RetCodeDefine.h"
#include "MemoryDefine.h"

template<typename _Type>
class AvlTree
{
	public:
		/**
		 * @brief 构造函数
		 */
		AvlTree();
		
		/**
		 * @brief 析构函数
		 */
		~AvlTree();

	private:
		/**
		 * @brief 左旋
		 *
		 * @prame Tree 树根结点
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败 
		 */
		int32_t LeftRotate(AvlTree<_Type> *Tree);

		/**
		 * @brief 右旋
		 *
		 * @prame Tree 树根结点
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败 
		 */
		int32_t RightRotate(AvlTree<_Type> *Tree);

	public:
		/**
		 * @brief 数据体
		 */
		_Type m_tree_data;
		
		/**
		 * @brief 平衡因子
		 */
		int32_t m_bf;

		/**
		 * @brief 左指针
		 */
		AvlTree<_Type> *_tree_left;

		/**
		 * @brief 右指针
		 */
		AvlTree<_Type> *_tree_right;

};

#include "AvlTree.tcc"

#endif
