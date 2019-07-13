#include "ParserMgr.h"

ParserMgr::ParserMgr()
{}

ParserMgr::~ParserMgr()
{}

//管理器初始化
int32_t ParserMgr::Init()
{
	try
	{
		_Parser = new ParserBase * [NS_PROTOCOL::EM_PROTOCOL_END];
	}
	catch(std::bad_alloc)
	{
		return RET::FAIL;
	}

	//Eth2解析器构造及初始化调用
	_Parser[NS_PROTOCOL::EM_ETH2_PROTOCOL] = new Eth2Parser();
	if (RET::SUC != _Parser[NS_PROTOCOL::EM_ETH2_PROTOCOL]->Init())
	{
		return RET::FAIL;
	}

	return RET::SUC;
}

//获取解析器地址
ParserBase *ParserMgr::GetParser(uint8_t uProType)
{
	//异常判断
	if (uProType >= NS_PROTOCOL::EM_PROTOCOL_END
		|| uProType <= NS_PROTOCOL::EM_PROTOCOL_START)
	{
		return NULL;
	}	

	return _Parser[uProType];
}
