#include "StrProc.h"

//构造函数
StrProc::StrProc()
{}

//析构函数
StrProc::~StrProc()
{}

//url编码接口
std::string StrProc::UrlEncode(const std::string &src)
{
	//异常判断
	if (0 == src.size())
	{
		return nullptr;
	}

	//编码
	std::string buf;
	std::string dst;
	for (uint32_t uIndex = 0; uIndex < src.size(); uIndex++)
	{
		if (isalnum(src[uIndex]))
		{
			buf[0] = src[uIndex];
		}
		else if(isspace(src[uIndex]))
		{
			buf[0] = '+';
		}
		else
		{
			buf[0] = '%';
			buf[1] = toHex((char)src[uIndex] >> 4);
			buf[2] = toHex((char)src[uIndex] % 16);
		}
		dst += buf;
	}

	return dst;
}

std::string StrProc::UrlDecode(const std::string &src)
{
	if (0 == src.size())
	{
		return nullptr;
	}

	const char * pSrc = (const char*)src.c_str();
	const uint32_t uSrcLen = src.length();
	const char * const pSrcEnd = pSrc + uSrcLen;
	const char * const pLastDec = pSrcEnd - 2;
	
	char decone;
	char dectwo;
	char * const pStart = new char [uSrcLen];
	char * pEnd = pStart; 
	while (pSrc < pLastDec)
	{
		if (*pEnd == '%')
		{
			if (-1 != (decone = Hex2Dec[*++pSrc])
							&& (-1 != (dectwo = Hex2Dec[*++pSrc])))
			{
				*pEnd++ = (decone << 4) + dectwo;
				++pSrc;
				continue;
			}
		}
		*pEnd++ = *pSrc++;
	}

	while (pSrc < pSrcEnd)
	{
		*pEnd++ = *pSrc++;
	}

	std::string dst = std::string(pStart, pEnd);
	delete [] pStart;

	return dst;
}

char StrProc::toHex(char ch)
{
	return ch > 9 ? ch + 55 : ch + 48;
}

char StrProc::Hex2Dec[256] = 
{
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	0, 1, 2, 3, 4, 5, 6, 7, 
	8, 9, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, 
	(char)-1, 10, 11, 12, 13, 14, 15, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, 10, 11, 12, 13, 14, 15, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1, (char)-1,
};
