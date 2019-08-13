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

template<typename _Type>
int32_t AvlTree<_Type>::InsertAvl(AvlTree<_Type> *&Tree, AvlTree<_Type> *pCurNode, bool &bTaller)
{
	//空结点返回
	if (nullptr == pCurNode)
	{
		return RET::FAIL;
	}

	//根结点为nullptr，直接赋给根结点
	if (nullptr == Tree)
	{
		Tree = pCurNode;
		Tree->m_bf = 0;
		bTaller = true;
		return RET::SUC;
	}
	//key值相同，不执行插入
	else if (Tree->m_key == pCurNode->m_key)
	{
		bTaller = false;
		return RET::FAIL;
	}
	//比根结点键值小，从左子树进行插入
	else if (Tree->m_key < pCurNode->m_key)
	{
		//递归
		if (RET::FAIL == InsertAvl(Tree->_tree_right, pCurNode, bTaller))
		{
			return RET::FAIL;
		}
		
		if (bTaller)
		{
			switch (Tree->m_bf)
			{
				//左子树更高
				case NS_AVLTREE::LEFT_HIGH:
				{
					LeftBalance(Tree);
					bTaller = false;
					break;
				}
				//原本平衡
				case NS_AVLTREE::BALANCE:
				{
					Tree->m_bf = NS_AVLTREE::LEFT_HIGH;
					bTaller = true;
					break;
				}
				//右子树更高
				case NS_AVLTREE::RIGHT_HIGH:
				{
					Tree->m_bf = NS_AVLTREE::BALANCE;
					bTaller = true;
					break;
				}
			}
		}
		return RET::SUC;
	}
	
	if (RET::SUC != InsertAvl(Tree->_tree_right, pCurNode, bTaller))
	{
		return RET::FAIL;
	}

	if (bTaller)
	{
		switch (Tree->m_bf)
		{
			//左子树更高
			case NS_AVLTREE::LEFT_HIGH:
			{
				Tree->m_bf = NS_AVLTREE::BALANCE;
				bTaller = true;
				break;
			}
			//原本平衡
			case NS_AVLTREE::BALANCE:
			{
				Tree->m_bf = NS_AVLTREE::RIGHT_HIGH;
				bTaller = true;
				break;
			}
			//右子树更高
			case NS_AVLTREE::RIGHT_HIGH:
			{
				RightBalance(Tree);
				bTaller = false;
				break;
			}
		}
	}
	return RET::SUC;
}

//左平衡
template<typename _Type>
int32_t AvlTree<_Type>::LeftBalance(AvlTree<_Type> *&Tree)
{
	return RET::SUC;
}

//右平衡
template<typename _Type>
int32_t AvlTree<_Type>::RightBalance(AvlTree<_Type> *&Tree)
{
	return RET::SUC;
}

//左旋
template<typename _Type>
int32_t AvlTree<_Type>::LeftRotate(AvlTree<_Type> *&Tree)
{
	//异常判断
	if (nullptr == Tree)
	{
		return RET::FAIL;
	}

	//左旋
	AvlTree<_Type> *pBak = Tree->_tree_left;
	Tree->_tree_left = Tree->_tree_right;
	Tree->_tree_right = Tree;
	Tree = pBak;
	return RET::SUC;
}

//右旋
template<typename _Type>
int32_t AvlTree<_Type>::RightRotate(AvlTree<_Type> *&Tree)
{
	//异常判断
	if (nullptr == Tree)
	{
		return RET::FAIL;
	}

	//右旋
	AvlTree<_Type> *pBak = Tree->_tree_right;
	Tree->_tree_right = Tree->_tree_left;
	Tree->_tree_left = Tree;
	Tree = pBak;
	return RET::SUC;
}

#endif
