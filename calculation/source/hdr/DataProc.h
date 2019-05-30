#ifndef _DATAPROC_H_
#define _DATAPROC_H_
#include "asmlib.h"
#include <vector>

namespace NS_DATAPROC
{
	class DataStatistics
	{
		public:
			/**
			 * @brief 构造函数
			 */
			DataStatistics()
			{}

			/**
			 * @brief 析构函数
			 */
			~DataStatistics()
			{}

		public:
			uint32_t num;			//数值
			uint32_t total;			//数值出现的次数
	};
}

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
		//static float GetKappa(uint32_t **Matrix, uint32_t uLenOne, uint32_t uLenTwo);
		static float GetKappa(std::vector<std::vector<uint32_t>> Matrix);

		/**
		 * @brief 计算算数平均值
		 *
		 * @prame vec数据统计结果
		 *
		 * @return 算数平均值
		 */
		static float GetAirthMeticMean(std::vector<NS_DATAPROC::DataStatistics> vec);

};

#endif
