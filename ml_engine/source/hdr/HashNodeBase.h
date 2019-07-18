#ifndef _HASHNODEBASE_H_
#define _HASHNODEBASE_H_

#include "MemoryDefine.h"

class HashNodeBase
{
	public:
		/**
		 * @brief 构造函数
		 */
		HashNodeBase()
		{}

		/**
		 * @brief 析构函数
		 */
		virtual ~HashNodeBase()
		{}

	public:
		/**
		 * @brief 上次活跃时间
		 */
		m_lastactivetime;
};

#endif
