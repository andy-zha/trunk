#include "ConsoleMgr.h"

int32_t main(int32_t args, char **argv)
{
	if (RET::SUC != ConsoleMgr::GetInstance().Init())
	{
		std::cout<<"Console Init Failed!"<<std::endl;
		return RET::FAIL;
	}

	if (RET::SUC != ConsoleMgr::GetInstance().Start())
	{
		std::cout<<"Console Start Failed!"<<std::endl;
		return RET::FAIL;
	}

	return RET::SUC;
}
