#include "SuperMatch.h"

/** 构造函数 **/
SuperMatch::SuperMatch()
{}

/** 析构函数 **/
SuperMatch::~SuperMatch()
{}

/** 构建状态机 **/
int32_t SuperMatch::Build(std::vector<std::string> pattern)
{
	//异常判断
	if (pattern.empty()) {
		return RET::FAIL;
	}

	//开始构建状态机
	std::vector<std::string>::iterator iter = pattern.begin();
	for (; iter != pattern.end(); iter++)
	{
		bool bFlag = false;
		BinaryTree<NS_SUPERMATCH::Pattern> *pCurNode = nullptr;
		for (uint32_t uIndex = 0; uIndex < (*iter).size(); uIndex++)
		{
			//建立左子树
			if (bFlag) {
				BinaryTree<NS_SUPERMATCH::Pattern> *pNewNode = nullptr;
				try {
					pNewNode = new BinaryTree<NS_SUPERMATCH::Pattern>;
				}catch(std::bad_alloc) {
					return RET::FAIL;
				}

				pNewNode->m_tree_data.m_ch = (*iter)[uIndex];
				pNewNode->m_tree_data.m_total = 0;
				if (RET::SUC != m_PatternTree.InsertLeftNode(pCurNode, pNewNode))
				{
					return RET::FAIL;
				}

				pCurNode = pNewNode;
				continue;
			}

			//向右查找结点，查不到添加右结点，并从当前结点开始建立左子树
			if (RET::SUC != FindPattern((*iter)[uIndex], pCurNode))
			{
				bFlag = true;
				BinaryTree<NS_SUPERMATCH::Pattern> *pNode = nullptr;
				try	{
					pNode = new BinaryTree<NS_SUPERMATCH::Pattern>;
				}catch(std::bad_alloc) {
					return RET::FAIL;
				}

				pNode->m_tree_data.m_ch = (*iter)[uIndex];
				pNode->m_tree_data.m_total = 0;
				if (RET::SUC != m_PatternTree.InsertRightNode(pCurNode, pNode))
				{
					return RET::FAIL;
				}

				pCurNode = pNode;
				continue;
			}

			pCurNode = pCurNode->m_tree_left;
		}
	}

	return RET::SUC;
}

/** 查找字符模式结点 **/
int32_t SuperMatch::FindPattern(char ch, BinaryTree<NS_SUPERMATCH::Pattern> *&pCurNode)
{
	//向右进行遍历
	BinaryTree<NS_SUPERMATCH::Pattern> *pNode = nullptr;
	if (nullptr == pCurNode) {
		pNode = &m_PatternTree;
	}else {
		pNode = pCurNode;
	}

	while (nullptr != pNode) {
		//字符相同,返回成功
		pCurNode = pNode;
		if (pNode->m_tree_data.m_ch == ch) {
			pNode->m_tree_data.m_total++;
			return RET::SUC;
		}

		pNode = pNode->m_tree_right;
	}

	return RET::FAIL;
}

/** 模式完全匹配 **/
int32_t SuperMatch::MatchPattern(std::string str)
{
	std::string::iterator iter = str.begin();
	BinaryTree<NS_SUPERMATCH::Pattern> *pCurNode = nullptr;
	for (; iter < str.end(); iter++) {
		//当前结点右子树查找
		if (RET::SUC != FindPattern(*iter, pCurNode))
		{
			return RET::FAIL;
		}

		//当前字符查找到，指针指向左子树
		pCurNode = pCurNode->m_tree_left;
	}

	//依然存在左子树，则完全匹配失败
	if (nullptr != pCurNode) {
		return RET::FAIL;
	}

	return RET::SUC;
}

/** 模式子串匹配 **/
int32_t SuperMatch::SubMatchPattern(std::string str)
{
	std::string::iterator iter = str.begin();
	BinaryTree<NS_SUPERMATCH::Pattern> *pCurNode = nullptr;
	for (; iter < str.end(); iter++) {
		//当前结点右子树查找
		if (RET::SUC != FindPattern(*iter, pCurNode))
		{
			return RET::FAIL;
		}

		//当前字符查找到，指针指向左子树
		pCurNode = pCurNode->m_tree_left;
	}

	return RET::SUC;
}

/** 模式模糊匹配(*) **/
int32_t SuperMatch::FuzzyMatchPattern(std::string str)
{
	std::string::iterator iter = str.begin();
	BinaryTree<NS_SUPERMATCH::Pattern> *pCurNode = nullptr;
	for (; iter < str.end(); iter++) {
		//当前结点右子树查找
		if (RET::SUC != FindPattern(*iter, pCurNode))
		{
			if ('*' == pCurNode->m_tree_data.m_ch)
			{
				return RET::SUC;
			}

			return RET::FAIL;
		}

		//当前字符查找到，指针指向左子树
		pCurNode = pCurNode->m_tree_left;
	}

	//依然存在左子树，则完全匹配失败
	if (nullptr != pCurNode) {
		return RET::FAIL;
	}

	return RET::SUC;
}

/** 查找字符模式结点(不区分大小写) **/
int32_t SuperMatch::SoftFindPattern(char ch, BinaryTree<NS_SUPERMATCH::Pattern> *&pCurNode)
{
	//向右进行遍历
	BinaryTree<NS_SUPERMATCH::Pattern> *pNode = nullptr;
	if (nullptr == pCurNode) {
		pNode = &m_PatternTree;
	}else {
		pNode = pCurNode;
	}

	while (nullptr != pNode) {
		//字符相同,返回成功
		pCurNode = pNode;
		if (tolower(pNode->m_tree_data.m_ch) == tolower(ch))
		{
			pNode->m_tree_data.m_total++;
			return RET::SUC;
		}

		pNode = pNode->m_tree_right;
	}

	return RET::FAIL;
}

/** 模式完全匹配 **/
int32_t SuperMatch::SoftMatch(std::string str)
{
	std::string::iterator iter = str.begin();
	BinaryTree<NS_SUPERMATCH::Pattern> *pCurNode = nullptr;
	for (; iter < str.end(); iter++) {
		//当前结点右子树查找
		if (RET::SUC != SoftFindPattern(*iter, pCurNode))
		{
			return RET::FAIL;
		}

		//当前字符查找到指针指向左子树
		pCurNode = pCurNode->m_tree_left;
	}

	//依然存在左子树，则完全匹配失败
	if (nullptr != pCurNode) {
		return RET::FAIL;
	}

	return RET::SUC;
}

/** 模式子串匹配 **/
int32_t SuperMatch::SoftSubMatch(std::string str)
{
	std::string::iterator iter = str.begin();
	BinaryTree<NS_SUPERMATCH::Pattern> *pCurNode = nullptr;
	for (; iter < str.end(); iter++) {
		//当前结点右子树查找
		if (RET::SUC != SoftFindPattern(*iter, pCurNode))
		{
			return RET::FAIL;
		}

		//当前字符查找到指针指向左子树
		pCurNode = pCurNode->m_tree_left;
	}

	return RET::SUC;
}

/** 模式模糊匹配(*) **/
int32_t SuperMatch::SoftFuzzyMatch(std::string str)
{
	std::string::iterator iter = str.begin();
	BinaryTree<NS_SUPERMATCH::Pattern> *pCurNode = nullptr;
	for (; iter < str.end(); iter++) {
		//当前结点右子树查找
		if (RET::SUC != SoftFindPattern(*iter, pCurNode))
		{
			if ('*' == pCurNode->m_tree_data.m_ch)
			{
				return RET::SUC;
			}

			return RET::FAIL;
		}

		//当前字符查找到指针指向左子树
		pCurNode = pCurNode->m_tree_left;
	}

	//依然存在左子树，则完全匹配失败
	if (nullptr != pCurNode) {
		return RET::FAIL;
	}

	return RET::SUC;
}

int32_t SuperMatch::Sort2Swap()
{}

/** 销毁状态机 **/
int32_t SuperMatch::Destroy()
{
	return RET::SUC;
}
