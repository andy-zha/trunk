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
