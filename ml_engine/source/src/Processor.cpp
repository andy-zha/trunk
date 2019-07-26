#include "Processor.h"

//构造函数
Processor::Processor()
{}

//析构函数
Processor::~Processor()
{}

//初始化接口
int32_t Processor::Init()
{
	//解析器管理器初始化
	if (RET::SUC != ParserMgr::GetInstance().Init())
	{
		std::cout<<"Ml_Engine: ParserMgr Init Failed!"<<std::endl;
		return RET::FAIL;
	}

	if (RET::SUC != _Parser.Init())
	{
		return RET::FAIL;
	}

	return RET::SUC;
}

//主处理接口
void Processor::Process(InputPacket *pInputPkt)
{
	//加载业务规则
	IPDRuleMgr::GetInstance().Process();

#ifdef _MEMCHECK_
//	MemCheck::GetInstance().WriteLog();
#endif	

	if (nullptr == pInputPkt)
	{
		return;
	}

	if (RET::SUC != _Parser.Start(pInputPkt))
	{
		m_ParserFailed++;
	}

	//释放包体内存
	if (nullptr != pInputPkt)
	{
		delete pInputPkt;
		pInputPkt = nullptr;
	}
}
