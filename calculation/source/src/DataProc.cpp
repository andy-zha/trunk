#include "DataProc.h"
#include <string.h>

DataProc::DataProc()
{}

DataProc::~DataProc()
{}

//float DataProc::GetKappa(uint32_t **Matrix, uint32_t uLenOne, uint32_t uLenTwo)
float DataProc::GetKappa(std::vector<std::vector<uint32_t>> Matrix)
{
	uint32_t uLenOne = Matrix.size();
    //异常判断
	if (0 == uLenOne)
	{
		return 0.0;
	}

    //计算p总
	uint32_t uTotals = 0;
	std::vector<uint32_t> TotalsOne(uLenOne, 0);
	std::vector<uint32_t> TotalsTwo(uLenOne, 0);
	for (uint32_t uIndexOne = 0; uIndexOne < uLenOne; uIndexOne++)
	{
		uint32_t uLenTwo = Matrix[uIndexOne].size();
		if (0 == uLenTwo || uLenOne != uLenTwo)
		{
			return 0.0;
		}

	    for (uint32_t uIndexTwo = 0; uIndexTwo < uLenTwo; uIndexTwo++)
		{
		    //所有项之和(p总;a0,a1...;b0,b1...)
			uTotals += Matrix[uIndexOne][uIndexTwo];
			TotalsOne[uIndexOne] += Matrix[uIndexOne][uIndexTwo];
            TotalsTwo[uIndexTwo] += Matrix[uIndexOne][uIndexTwo];
		}
	}

	//计算po/pe
	uint32_t uDiagonalTotals = 0;
	uint32_t uProductTotals = 0;
	for (uint32_t uIndex = 0; uIndex < uLenOne; uIndex++)
	{
	    //求对角线和
	    uDiagonalTotals += Matrix[uIndex][uIndex];
		uProductTotals += TotalsOne[uIndex] * TotalsTwo[uIndex];
	}
	float po = (float)uDiagonalTotals / uTotals;
	float pe = (float)uProductTotals / (uTotals * uTotals);

	//计算kappa
	float kappa = (po - pe) / (1 - pe);
	return kappa;
}

//计算算数平均值
float DataProc::GetAirthMeticMean(std::vector<NS_DATAPROC::DataStatistics> vec)
{
	uint32_t Sum = 0.0;
	uint32_t Total = 0;

	std::vector<NS_DATAPROC::DataStatistics>::iterator iter = vec.begin();
	for (; iter != vec.end(); iter++)
	{
		Sum += iter->num * iter->total;
		Total += iter->total;
	}

	return (float)Sum / Total;
}
