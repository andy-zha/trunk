#include "gtest/gtest.h"
#include "BinaryTree.h"

namespace
{
	class TestBinaryTree : public testing::Test
	{
		void SetUp() override
		{}

		void TearDown() override
		{} 
	};
}

class TreeNode
{
	public:
		TreeNode()
		{}

		~TreeNode()
		{}

	public:
		std::string TreeValue;
};

TEST_F(TestBinaryTree, TEST_BINARY_TREE)
{
	//初始化根结点
	BinaryTree<TreeNode> m_BinaryTree;
	m_BinaryTree.uIndex = 1;
	m_BinaryTree.m_tree_data.TreeValue = "root";

	//根结点插入左结点
	BinaryTree<TreeNode> *pNodeOne = new BinaryTree<TreeNode>;
	pNodeOne->uIndex = 2;
	pNodeOne->m_tree_data.TreeValue = "root_left";
	ASSERT_EQ(RET::SUC, m_BinaryTree.InsertLeftNode(pNodeOne));
	ASSERT_EQ(2, m_BinaryTree._tree_left->uIndex);
	ASSERT_STREQ(std::string("root_left").c_str(), 
					m_BinaryTree._tree_left->m_tree_data.TreeValue.c_str());

	//根结点插入右结点
	BinaryTree<TreeNode> *pNodeTwo = new BinaryTree<TreeNode>;
	pNodeTwo->uIndex = 3;
	pNodeTwo->m_tree_data.TreeValue = "root_right";
	ASSERT_EQ(RET::SUC, m_BinaryTree.InsertRightNode(pNodeTwo));
	ASSERT_EQ(3, m_BinaryTree._tree_right->uIndex);
	ASSERT_STREQ(std::string("root_right").c_str(), 
					m_BinaryTree._tree_right->m_tree_data.TreeValue.c_str());

	//根左节点插入左节点
	BinaryTree<TreeNode> *pNodeThree = new BinaryTree<TreeNode>;
	pNodeThree->uIndex = 4;
	pNodeThree->m_tree_data.TreeValue = "root_left_left";
	ASSERT_EQ(RET::SUC, m_BinaryTree.InsertLeftNode(pNodeOne, pNodeThree));

	//根左节点插入右节点
	BinaryTree<TreeNode> *pNodeFour = new BinaryTree<TreeNode>;
	pNodeFour->uIndex = 5;
	pNodeFour->m_tree_data.TreeValue = "root_left_right";
	ASSERT_EQ(RET::SUC, m_BinaryTree.InsertRightNode(pNodeOne, pNodeFour));

	//前序遍历
	ASSERT_EQ(RET::SUC, m_BinaryTree.PreOrderTraver(&m_BinaryTree));
	//中序遍历
	ASSERT_EQ(RET::SUC, m_BinaryTree.InOrderTraver(&m_BinaryTree));
	//后序遍历
	ASSERT_EQ(RET::SUC, m_BinaryTree.OnOrderTraver(&m_BinaryTree));
}
