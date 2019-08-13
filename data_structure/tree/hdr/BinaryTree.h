#ifndef _BINARY_TREE_H_
#define _BINARY_TREE_H_

#include "RetCodeDefine.h"
#include "MemoryDefine.h"

//树结点定义
template<typename _Type>
class BinaryTree
{
	public:
		/**
		 * @brief 构造函数
		 */
		BinaryTree();

		/**
	     * @brief 析构函数
		 */
		~BinaryTree();

		/**
 		 * @brief 插入左子结点(root结点插入)
 		 *
 		 * @prame pNewNode 新节点
 		 *
 		 * @return RET::SUC 成功; RET::FAIL失败
 		 */
		int32_t InsertLeftNode(BinaryTree<_Type> *pNewNode);

		/**
 		 * @brief 插入左子结点
 		 *
 		 * @prame pParentNode 双亲结点; pNewNode 新节点
 		 *
 		 * @return RET::SUC 成功; RET::FAIL失败
 		 */
		int32_t InsertLeftNode(BinaryTree<_Type> *pParentNode, BinaryTree<_Type> *pNewNode);

		/**
 		 * @brief 插入右子结点(root结点插入)
 		 *
 		 * @prame pNewNode 新节点
 		 *
 		 * @return RET::SUC 成功; RET::FAIL失败
 		 */
		int32_t InsertRightNode(BinaryTree<_Type> *pNewNode);

		/**
 		 * @brief 插入右子结点
 		 *
 		 * @prame pParentNode 双亲结点; pNewNode 新节点
 		 *
 		 * @return RET::SUC 成功; RET::FAIL失败
 		 */
		int32_t InsertRightNode(BinaryTree<_Type> *pParentNode, BinaryTree<_Type> *pNewNode);

		/**
		 * @brief 分层遍历
		 *
		 * @prame pNode 根结点
		 *
		 * @return RET::SUC 遍历成功; RET::FAIL 遍历失败
		 */
		int32_t OrderTraver(BinaryTree<_Type> *pNode);

		/**
		 * @brief 前序遍历
		 *
		 * @prame pNode 根结点
		 *
		 * @return RET::SUC 遍历成功; RET::FAIL 遍历失败
		 */
		int32_t PreOrderTraver(BinaryTree<_Type> *pNode);

		/**
		 * @brief 中序遍历
		 *
		 * @prame pNode 根结点
		 *
		 * @return RET::SUC 遍历成功; RET::FAIL 遍历失败
		 */
		int32_t InOrderTraver(BinaryTree<_Type> *pNode);

		/**
		 * @brief 后序遍历
		 *
		 * @prame pNode 根结点
		 *
		 * @return RET::SUC 遍历成功; RET::FAIL 遍历失败
		 */
		int32_t OnOrderTraver(BinaryTree<_Type> *pNode);

	public:
		/**
		 * @brief 结点标记
		 */
		uint32_t uIndex;

		/**
		 * @brief 数据体
		 */
		_Type m_tree_data;

		/**
		 * @brief 左指针
		 */
		BinaryTree<_Type> *_tree_left;

		/**
		 * @brief 右指针
		 */
		BinaryTree<_Type> *_tree_right;
};

#include "BinaryTree.tcc"
#endif
