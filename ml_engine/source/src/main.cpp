#include "Processor.h"
#include "Acceptor.h"

#ifdef _GTEST_
#include "gtest/gtest.h"
#endif

//主程序入口
int32_t main(int32_t argc, char **argv)
{
#ifndef _GTEST
	//接收器初始化
	if (RET::SUC != Acceptor::GetInstance().Init())
	{
		std::cout<<"Ml_Engine: Acceptor Init Failed!"<<std::endl;
		return RET::FAIL;
	}

	//主处理初始化
	if (RET::SUC != Processor::GetInstance().Init())
	{
		std::cout<<"Ml_Engine: Processor Init Failed!"<<std::endl;
		return RET::FAIL;
	}

	//接收器启动
	if (RET::SUC != Acceptor::GetInstance().Start())
	{
		std::cout<<"Ml_Engine: Acceptor Start Failed!"<<std::endl;
		return RET::FAIL;
	}

	return RET::SUC;

#else
	printf("Running main() from %s\n", __FILE__);
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#endif
}
