#include "Processor.h"

#ifdef _GTEST_
#include <gtest/gtest.h>
#endif

int32_t main(int32_t argc, char **argv)
{
#ifndef _GTEST_

	//主处理初始化
	if (RET::SUC != Processor::GetInstance().Init())
	{
		std::cout<<"WebSocket: Processor Init Failed!"<<std::endl;
		return RET::FAIL;
	}

	//主处理启动
	if (RET::SUC != Processor::GetInstance().Start())
	{
		std::cout<<"WebSocket: Processor Start Failed!"<<std::endl;
		return RET::FAIL;
	}
	return RET::SUC;

#else

	printf("Running main() from %s\n", __FILE__);
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();

#endif
}
