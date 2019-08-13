#ifndef _BINARY_TREE_TCC
#define _BINARY_TREE_TCC

#include "BinaryTree.h"

//构造函数
template<typename _Type>
BinaryTree<_Type>::BinaryTree():uIndex(0), _tree_left(nullptr), _tree_right(nullptr)
{
}

//析构函数
template<typename _Type>
BinaryTree<_Type>::~BinaryTree()
{
}

//插入左节点
template<typename _Type>
int32_t BinaryTree<_Type>::InsertLeftNode(BinaryTree<_Type> *pNewNode)
{
	if (nullptr == pNewNode)
	{
		return RET::FAIL;
	}

	this->_tree_left = pNewNode;

	return RET::SUC;
}

//插入左节点
template<typename _Type>
int32_t BinaryTree<_Type>::InsertLeftNode(BinaryTree<_Type> *pParentNode, 
				BinaryTree<_Type> *pNewNode)
{
	if (nullptr == pParentNode || nullptr == pNewNode)
	{
		return RET::FAIL;
	}

	pParentNode->_tree_left = pNewNode;

	return RET::SUC;
}

//插入右节点
template<typename _Type>
int32_t BinaryTree<_Type>::InsertRightNode(BinaryTree<_Type> *pNewNode)
{
	if (nullptr == pNewNode)
	{
		return RET::FAIL;
	}

	this->_tree_right = pNewNode;

	return RET::SUC;
}

//插入右节点
template<typename _Type>
int32_t BinaryTree<_Type>::InsertRightNode(BinaryTree<_Type> *pParentNode, 
				BinaryTree<_Type> *pNewNode)
{
	if (nullptr == pParentNode || nullptr == pNewNode)
	{
		return RET::FAIL;
	}

	pParentNode->_tree_right = pNewNode;

	return RET::SUC;
}

//分层遍历
template<typename _Type>
int32_t BinaryTree<_Type>::OrderTraver(BinaryTree<_Type> *pNode)
{
	if (nullptr == pNode) 
	{
		return RET::FAIL;
	}

	return RET::SUC;
}

//前序遍历
template<typename _Type>
int32_t BinaryTree<_Type>::PreOrderTraver(BinaryTree<_Type> *pNode)
{
	if (nullptr == pNode) 
	{
		return RET::FAIL;
	}

	std::cout<<"pNode->uIndex:"<<pNode->uIndex<<std::endl;
	PreOrderTraver(pNode->_tree_left);
	PreOrderTraver(pNode->_tree_right);

	return RET::SUC;
}

//中序遍历
template<typename _Type>
int32_t BinaryTree<_Type>::InOrderTraver(BinaryTree<_Type> *pNode)
{
	if (nullptr == pNode) 
	{
		return RET::FAIL;
	}

	InOrderTraver(pNode->_tree_left);
	std::cout<<"pNode->uIndex:"<<pNode->uIndex<<std::endl;
	InOrderTraver(pNode->_tree_right);

	return RET::SUC;
}

//后序遍历
template<typename _Type>
int32_t BinaryTree<_Type>::OnOrderTraver(BinaryTree<_Type> *pNode)
{
	if (nullptr == pNode) 
	{
		return RET::FAIL;
	}

	OnOrderTraver(pNode->_tree_left);
	OnOrderTraver(pNode->_tree_right);
	std::cout<<"pNode->uIndex:"<<pNode->uIndex<<std::endl;

	return RET::SUC;
}

#endif 
