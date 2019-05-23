#ifndef _STRPROC_H_
#define _STRPROC_H_

#include "asmlib.h"

class StrProc
{
    public:
		/**
		 * @brief 构造函数
		 */
		StrProc();

		/**
		 * @brief 析构函数
		 */
		~StrProc();

		/**
		 * @brief 获取两字符串匹配度
		 *
		 * @prame strone strtwo 字符串
		 *
		 * @return 匹配度
		 */
		static float GetStrMatchDegree(std::string strone, std::string strtwo);
};

#endif
