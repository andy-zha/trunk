#include "Processor.h"
#include "ParserMgr.h"

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

	return RET::SUC;
}

//test
void test(InputPacket *&pkt)
{
	std::cout<<"SrcMac: "<<pkt->SrcMac<<std::endl;
	std::cout<<"DstMac: "<<pkt->DstMac<<std::endl;
}

//主处理接口
void Processor::Process()
{
	while (true)
	{
		//构建包体
		InputPacket *InputPkt = NULL;
		try
		{
			InputPkt = new InputPacket();
		}
		catch(std::bad_alloc)
		{
			return;
		}
		
		//循环解析
		while (0 <= InputPkt->uPayload)
		{
			ParserBase *_Parser = ParserMgr::GetInstance().GetParser(InputPkt->uType);
			if (NULL == _Parser)
			{
				break;
			}

			if (RET::SUC != _Parser->Start(InputPkt))
			{
				break;
			}
		}

		test(InputPkt);

		//释放包体内存
		if (NULL != InputPkt)
		{
			delete InputPkt;
			InputPkt = NULL;
		}
	}
}
