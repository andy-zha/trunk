#include "Accepter.h"
#include "Processor.h"

#ifdef _GTEST_
#include "gtest/gtest.h"
#endif

//主程序入口
int32_t main(int32_t argc, char **argv)
{
#ifndef _GTEST_
	//主处理初始化
	if (RET::SUC != Processor::getInstance().init())
	{
		std::cout<<"Ml_Engine: Accepter Init Failed!"<<std::endl;
		return RET::FAIL;
	}

	//接收器初始化
	if (RET::SUC != Accepter::getInstance().init())
	{
		std::cout<<"Ml_Engine: Accepter Init Failed!"<<std::endl;
		return RET::FAIL;
	}

	//处理器启动
	if (RET::SUC != Processor::getInstance().start())
	{
		std::cout<<"Ml_Engine: Accepter Start Failed!"<<std::endl;
		return RET::FAIL;
	}

	//接收器启动
	if (RET::SUC != Accepter::getInstance().start())
	{
		std::cout<<"Ml_Engine: Accepter Start Failed!"<<std::endl;
		return RET::FAIL;
	}

	Accepter::getInstance().threadJoin();
	Processor::getInstance().threadJoin();

	return RET::SUC;

#else
	printf("Running main() from %s\n", __FILE__);
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#endif

}
