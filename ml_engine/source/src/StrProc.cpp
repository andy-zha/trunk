#include "StrProc.h"

char StrProc::HEX2DEC[256] = {
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,  
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,  
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,  
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,
	0,  1,  2,  3,   4,  5,  6,  7,   
	8,  9, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, 10, 11, 12,  13, 14, 15, (char)-1,  
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,  
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, 10, 11, 12,  13, 14, 15, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,  
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,  
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,  
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,  
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,  
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,  
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,  
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,  
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,  
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,
	(char)-1, (char)-1, (char)-1, (char)-1,  (char)-1, (char)-1, (char)-1, (char)-1,
};

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
	//异常判断
	if (0 == src.size())
	{
		return nullptr;
	}

	const unsigned char * pSrc = (const unsigned char *)src.c_str();
	const uint32_t uSrclen = src.length();
	const unsigned char * const pSrcEnd = pSrc + uSrclen;
	// last decodable '%'
	const unsigned char * const pSrcLastDec = pSrcEnd - 2;

	char * const pStart = new char[uSrclen];
	char * pEnd = pStart;

	while (pSrc < pSrcLastDec) 
	{
		if (*pSrc == '%') 
		{
			char dec1;
			char dec2;
			if (-1 != (dec1 = StrProc::HEX2DEC[*(pSrc + 1)])
                        && -1 != (dec2 = StrProc::HEX2DEC[*(pSrc + 2)])) 
			{
				*pEnd++ = (dec1 << 4) + dec2;
				pSrc += 3;
				continue;
			}
		}
		*pEnd++ = *pSrc++;
	}
        
	// the last 2- chars
	while (pSrc < pSrcEnd) 
	{
		*pEnd++ = *pSrc++;
	}
        
	std::string sResult(pStart, pEnd);
	delete [] pStart;
	return sResult;
}

char StrProc::toHex(char ch)
{
	return ch > 9 ? ch + 55 : ch + 48;
}
