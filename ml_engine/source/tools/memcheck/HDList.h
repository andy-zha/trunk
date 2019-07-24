#ifndef _DLIST_H_
#define _DLIST_H_

#include "RetCodeDefine.h"

template<typename Type>
class HDList
{
	public:
		/*
		 * @breif 构造函数
		 */
		HDList():_dlist_prev(this),_dlist_next(this),_dlist_len(0)
		{
		}

		/*
		 ** @breif 构造函数
		 */
		~HDList()
		{
		}

		/*
		 ** @breif 弹出节点
		 */
		int32_t PopNode(HDList<Type> *pCurnode)
		{
			if (0 >= _dlist_len || NULL == pCurnode)
			{
				return RET::FAIL;
			}
			pCurnode->_dlist_prev->_dlist_next = pCurnode->_dlist_next;
			pCurnode->_dlist_next->_dlist_prev = pCurnode->_dlist_prev;

			pCurnode->_dlist_prev = pCurnode;
			pCurnode->_dlist_next = pCurnode;

			_dlist_len--;
			return RET::SUC;
		}

		/*
		 ** @breif 前向添加新的节点
		 */
		int32_t PrevAddNode(HDList<Type> *pNewnode, HDList<Type> *pCurnode)
		{
			if (0 > _dlist_len || NULL == pNewnode || NULL == pCurnode)
			{
				return RET::FAIL;
			}

			pCurnode->_dlist_prev->_dlist_next = pNewnode;
			pNewnode->_dlist_prev = pCurnode->_dlist_prev;
			pCurnode->_dlist_prev = pNewnode;
			pNewnode->_dlist_next = pCurnode;

			_dlist_len++;
			return RET::SUC;
		}

		/*
		 ** @breif 后向添加新的节点
		 */
		int32_t NextAddNode(HDList<Type> *pNewnode, HDList<Type> *pCurnode)
		{
			if (0 > _dlist_len || NULL == pNewnode || NULL == pCurnode)
			{
				return RET::FAIL;
			}

			pCurnode->_dlist_next->_dlist_prev = pNewnode;
			pNewnode->_dlist_next = pCurnode->_dlist_next;
			pCurnode->_dlist_next = pNewnode;
			pNewnode->_dlist_prev = pCurnode;

			_dlist_len++;
			return RET::SUC;
		}

		/*
		 ** @breif 头节点后添加新的节点
		 */
		int32_t HeadAddNode(HDList<Type> *pNewnode)
		{
			if (NULL == pNewnode) 
			{
				return RET::FAIL;
			}

			if (RET::SUC != NextAddNode(pNewnode,this))
			{
				return RET::FAIL;
			}

			return RET::SUC;
		}

		/*
		 ** @breif 尾节点后添加新的节点
		 */
		int32_t TailAddNode(HDList<Type> *pNewnode)
		{
			if (NULL == pNewnode){
				return RET::FAIL;
			}

			if (RET::SUC != PrevAddNode(pNewnode,this))
			{
				return RET::FAIL;
			}

			return RET::SUC;
		}

		/*
		 ** @breif 后序遍历所有节点
		 */
		int32_t ForwardTraver(HDList<Type> *&pCurnode)
		{
			if(NULL == pCurnode)
			{
				pCurnode = _dlist_next;
			}
			else
			{
				pCurnode = pCurnode->_dlist_next;
			}

			return pCurnode != this ? RET::SUC : RET::FAIL; 
		}

		/*
		 ** @breif 前向遍历所有节点
		 */
		int32_t BackwardTraver(HDList<Type> *&pCurnode)
		{
			if(NULL == pCurnode)
			{
				pCurnode = _dlist_prev;
			}
			else
			{
				pCurnode = pCurnode->_dlist_prev;
			}

			return pCurnode != this ? RET::SUC : RET::FAIL; 
		}

		/*
		 ** @breif 删除节点
		 */
		int32_t RemoveNode(HDList<Type> *pCurnode)
		{
			if (RET::SUC == PopNode(pCurnode))
			{
				if (NULL != pCurnode)
				{
					delete pCurnode;
					pCurnode = NULL;
				}
				return RET::SUC;
			}

			return RET::FAIL;
		}

		/*
		 ** @brief 销毁链表
		 */
		int32_t DestroyList()
		{
			HDList<Type> *pNode = NULL;
			while (RET::SUC == ForwardTraver(pNode))
			{
				if (NULL != pNode && RET::SUC != PopNode(pNode))
				{
					return RET::FAIL;
				}

				if (NULL != pNode)
				{
					delete pNode;
					pNode = NULL;
				}
			}

			return RET::SUC;
		}

		/*
		 ** @breif 获取链表长度
		 */
		int32_t GetLen() const
		{
			return _dlist_len;
		}

	public:
		/*
		 ** @breif 公有数据成员
		 */
		Type m_dlist_data;

	private:
		/*
		 ** @breif 前向指针
		 */
		HDList<Type> * _dlist_prev;

		/*
		 ** @breif 后向指针
		 */
		HDList<Type> * _dlist_next;

		/*
		 ** @breif 双向链表长度
		 */
		int32_t _dlist_len;
};

#endif
