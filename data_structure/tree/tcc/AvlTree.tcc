#ifndef _AVL_TREE_TCC
#define _AVL_TREE_TCC

//构造函数
template<typename _Type>
AvlTree<_Type>::AvlTree() : m_bf(0), _tree_left(nullptr), _tree_right(nullptr)
{}

//析构函数
template<typename _Type>
AvlTree<_Type>::~AvlTree()
{}

//左旋
template<typename _Type>
int32_t AvlTree<_Type>::LeftRotate(AvlTree<_Type> *Tree)
{
	if (nullptr == Tree)
	{
		return RET::FAIL;
	}

	AvlTree<_Type> *pBak = Tree->_tree_left;
	Tree->_tree_left = Tree->_tree_right;
	Tree->_tree_right = Tree;
	Tree = pBak;
	return RET::SUC;
}

//右旋
template<typename _Type>
int32_t AvlTree<_Type>::RightRotate(AvlTree<_Type> *Tree)
{
	if (nullptr == Tree)
	{
		return RET::FAIL;
	}

	AvlTree<_Type> *pBak = Tree->_tree_right;
	Tree->_tree_right = Tree->_tree_left;
	Tree->_tree_left = Tree;
	Tree = pBak;
	return RET::SUC;
}

#endif
