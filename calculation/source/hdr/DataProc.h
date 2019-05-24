#ifndef _DATAPROC_H_
#define _DATAPROC_H_
#include "asmlib.h"

//数据处理模块
class DataProc
{
    public:
		/**
		 * @brief 构造函数
		 */
		DataProc();

		/**
		 * @brief 析构函数
		 */
		~DataProc();

		/**
		 * @brief 获取kappa系数(一维长度和二维长度必须相等，不等系数为0)
		 *
		 * @prame Matrix 数据矩阵(二维数组);uLenOne 矩阵一维长度;uLenTwo 矩阵二维长度
		 *
		 * @return kappa系数
		 */
		static float GetKappa(uint32_t **Matrix, uint32_t uLenOne, uint32_t uLenTwo);
};

#endif
