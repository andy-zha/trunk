#include "HttpParser.h"

http_parser_settings HttpParser::Settings;

//构造函数
HttpParser::HttpParser()
{
}

//析构函数
HttpParser::~HttpParser()
{}

//解析器初始化接口
int32_t HttpParser::Init()
{
	//初始化解析回调函数
	Settings.on_url = &HttpParser::OnUrl;
	Settings.on_header_field = &HttpParser::OnHeaderField;
	Settings.on_header_value = &HttpParser::OnHeaderValue;
	Settings.on_body = &HttpParser::OnBody;

	return RET::SUC;
}

//解析器入口
int32_t HttpParser::Start(InputPacket *pInputPkt)
{
	//异常判断
	if (NULL == pInputPkt || NULL == pInputPkt->pStr
					|| 0 == pInputPkt->uLength)
	{
		return RET::FAIL;
	}

	http_parser_init(&_parser, HTTP_REQUEST);
	//执行解析及回调
	size_t parsed = http_parser_execute(&_parser, &Settings, 
					pInputPkt->pStr, pInputPkt->uLength);
	//解析字节数不等于data总长度返回失败
	if (parsed != pInputPkt->uLength)
	{
		return RET::FAIL;
	}

	return RET::SUC;
}

//url解析回调
int32_t HttpParser::OnUrl(http_parser *_parse, const char* at, size_t uLength)
{
	char Method[10];
	if (NS_HTTPPASER::EM_METHOD_START < _parse->method 
					&& NS_HTTPPASER::EM_METHOD_END > _parse->method)
	{
		_MEM_CPY_(Method, const_cast<char*>(NS_HTTPPASER::HttpMethod[_parse->method]), 10);
		std::cout<<"method:"<<Method<<std::endl;
	}

	char *pUrl = (char*)_MEM_NEW_(uLength + 1);
	_MEM_CPY_(pUrl, const_cast<char*>(at), uLength);
	_MEM_ZERO_(pUrl, uLength + 1, uLength);

	std::cout<<"Url:"<<pUrl<<std::endl;
	_MEM_DEL_(pUrl);

	return RET::SUC;
}

//header解析回调(field)
int32_t HttpParser::OnHeaderField(http_parser *_parse, const char* at, size_t uLength)
{
	char *pField = (char*)_MEM_NEW_(uLength + 1);
	_MEM_CPY_(pField, const_cast<char*>(at), uLength);
	_MEM_ZERO_(pField, uLength + 1, uLength);

	std::cout<<"Field:"<<pField<<std::endl;
	_MEM_DEL_(pField);

	return RET::SUC;
}

//header解析回调(value)
int32_t HttpParser::OnHeaderValue(http_parser *_parse, const char* at, size_t uLength)
{
	char *pValue = (char*)_MEM_NEW_(uLength + 1);
	_MEM_CPY_(pValue, const_cast<char*>(at), uLength);
	_MEM_ZERO_(pValue, uLength + 1, uLength);

	std::cout<<"Value:"<<pValue<<std::endl;
	_MEM_DEL_(pValue);

	return RET::SUC;
}

//body解析回调
int32_t HttpParser::OnBody(http_parser *_parse, const char* at, size_t uLength)
{
	char *pValue = (char*)_MEM_NEW_(uLength + 1);
	_MEM_CPY_(pValue, const_cast<char*>(at), uLength);
	_MEM_ZERO_(pValue, uLength + 1, uLength);

	std::cout<<"Body:"<<pValue<<std::endl;
	_MEM_DEL_(pValue);

	return RET::SUC;
}
