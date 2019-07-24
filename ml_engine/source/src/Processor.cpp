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

//test
void test(InputPacket *&pkt)
{
	std::cout<<"ClientPort: "<<pkt->m_ClientPort<<std::endl;
	std::cout<<"ClientIp: "<<pkt->m_ClientIp<<std::endl;
}

//主处理接口
void Processor::Process(InputPacket *pInputPkt)
{
	//加载业务规则
	IPDRuleMgr::GetInstance().Process();

	if (NULL == pInputPkt)
	{
		return;
	}

	if (RET::SUC != _Parser.Start(pInputPkt))
	{
		m_ParserFailed++;
	}

	test(pInputPkt);

	//释放包体内存
	if (NULL != pInputPkt)
	{
		delete pInputPkt;
		pInputPkt = NULL;
	}
}
