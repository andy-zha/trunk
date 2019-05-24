#include "DataProc.h"

DataProc::DataProc()
{}

DataProc::~DataProc()
{}

float DataProc::GetKappa(uint32_t **Matrix, uint32_t uLenOne, uint32_t uLenTwo)
{
    //异常判断
	if (0 == uLenOne || 0 == uLenTwo 
			|| uLenOne == uLenTwo)
	{
		return 0.0;
	}

    //计算p总
    uint32_t uIndexOne = 0;
	uint32_t uTotals = 0;
	uint32_t *TotalsOne = new uint32_t [uLenOne];
	uint32_t *TotalsTwo = new uint32_t [uLenTwo];
	for (uIndexOne = 0; uIndexOne < uLenOne; uIndexOne++)
	{
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
	for (uIndexOne = 0; uIndexOne < uLenOne; uIndexOne++)
	{
	    //求对角线和
	    uDiagonalTotals += Matrix[uIndexOne][uIndexOne];
		uProductTotals += TotalsOne[uIndexOne] * TotalsTwo[uIndexOne];
	}
	float po = (float)uDiagonalTotals / uTotals;
	float pe = (float)uProductTotals / uTotals * uTotals;

    //释放指针
    delete [] TotalsOne;
	delete [] TotalsTwo;

	//计算kappa
	float kappa = (po - pe) / (1 - pe);
	return kappa;
}
