#include "Processor.h"

//主程序入口
int32_t main(int32_t argc, char **argv)
{
	//主处理初始化
	if (RET::SUC != Processor::GetInstance().Init())
	{
		std::cout<<"Ml_Engine: Processor Init Failed!"<<std::endl;
		return RET::FAIL;
	}

	Processor::GetInstance().Process();

	return RET::SUC;
}
