#include "StrProc.h"

StrProc::StrProc()
{}

StrProc::~StrProc()
{}

//获取字符串匹配度
float StrProc::GetStrMatchDegree(std::string strone, std::string strtwo)
{
	//获取字符串长度
	uint32_t uSizeOne = strone.size();
	uint32_t uSizeTwo = strtwo.size();

	//其中一字符串长度为0另一字符串长度不为0则匹配度为0
	if ((0 == uSizeOne && 0 != uSizeTwo)
			|| (0 != uSizeOne && 0 == uSizeTwo))
	{
		return 0;
	}

	//两字符串长度都为0，匹配度为1
	if (0 == uSizeOne && 0 == uSizeTwo)
	{
		return 1;
	}

	//初始化统计数组
	uint32_t uIndex = 0;
	uint32_t Times[uSizeOne + 1][uSizeTwo + 1];
	for (uIndex = 0; uIndex < uSizeOne; uIndex++)
	{
		Times[0][uIndex] = uIndex;
	}
	for (uIndex = 0; uIndex < uSizeTwo; uIndex++)
	{
		Times[uIndex][0] = uIndex;
	}

	for (uint32_t uOne = 1; uOne <= uSizeOne; uOne++)
	{
		for (uint32_t uTwo = 1; uTwo <= uSizeTwo; uTwo++)
		{
			//计算三种可能性次数
			uint32_t TimeOne = Times[uOne - 1][uTwo] + 1;
			uint32_t TimeTwo = Times[uTwo][uOne - 1] + 1;
			uint32_t TimeThree = 0;
			if (strone[uOne] == strtwo[uTwo])
			{
				TimeThree = Times[uOne - 1][uTwo - 1];
			}
			else
			{
				TimeThree = Times[uOne - 1][uTwo - 1] + 1;  
			}

			//选择次数最小值
			uint32_t uTimes = TimeOne;
			if (uTimes > TimeTwo)
			{
				uTimes = TimeTwo;
			}
			if (uTimes > TimeThree)
			{
				uTimes = TimeThree;
			}
			Times[uOne][uTwo] = uTimes;
		}
	}

	//取字符串最大长度值且给出匹配度
	uint32_t MaxSize = uSizeOne;
	if (MaxSize < uSizeTwo) {
		MaxSize = uSizeTwo;
	}
	float Degree = 1 - (float)Times[uSizeOne][uSizeTwo] / MaxSize;
	return Degree;
}
