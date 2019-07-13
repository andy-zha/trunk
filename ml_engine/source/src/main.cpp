#include "Processor.h"

int32_t main(int32_t argc, char **argv)
{
	if (RET::SUC != Processor::GetInstance().Init())
	{
		std::cout<<"Ml_Engine: Processor Init Failed!"<<std::endl;
	}

	Processor::GetInstance().Process();

	return RET::SUC;
}
