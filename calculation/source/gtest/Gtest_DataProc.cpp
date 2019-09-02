#include "gtest/gtest.h"
#include "DataProc.h"

namespace {
    class Test_DataProc : public testing::Test
	{
	    void SetUp() override{};

		void TearDown() override{};
	};
}

TEST_F(Test_DataProc, GTest_GetKappa)
{
	std::vector<std::vector<uint32_t>> Metrix;	
	std::vector<uint32_t> vecone;
	vecone.push_back(10);
	vecone.push_back(5);
	vecone.push_back(5);
	Metrix.push_back(vecone);
	std::vector<uint32_t> vectwo;
	vectwo.push_back(2);
	vectwo.push_back(35);
	vectwo.push_back(2);
	Metrix.push_back(vectwo);
	std::vector<uint32_t> vecthree;
	vecthree.push_back(8);
	vecthree.push_back(5);
	vecthree.push_back(15);
	Metrix.push_back(vecthree);

	ASSERT_FLOAT_EQ(0.51042104, DataProc::GetKappa(Metrix));
}

TEST_F(Test_DataProc, GTest_GetAirthMeticMean)
{
	std::vector<NS_DATAPROC::DataStatistics> vec;
	for (uint32_t uindex = 0; uindex < 5; uindex++)
	{
		NS_DATAPROC::DataStatistics data;
		data.num = uindex + 3;
		data.total = uindex + 5;
		vec.push_back(data);
	}

	ASSERT_FLOAT_EQ(5.28571428, DataProc::GetAirthMeticMean(vec));
}

TEST_F(Test_DataProc, GTest_Sort)
{
	//测试冒泡排序
	std::vector<int32_t> vSrc;
	vSrc.push_back(123);
	vSrc.push_back(-123);
	vSrc.push_back(24);
	vSrc.push_back(0);
	vSrc.push_back(89);
	vSrc.push_back(21);
	ASSERT_EQ(RET::SUC, DataProc::BubbleSort(vSrc));
	ASSERT_EQ(123, vSrc[5]);
	ASSERT_EQ(89, vSrc[4]);
	ASSERT_EQ(24, vSrc[3]);
	ASSERT_EQ(21, vSrc[2]);
	ASSERT_EQ(0, vSrc[1]);
	ASSERT_EQ(-123, vSrc[0]);

	//测试选择排序
	vSrc.clear();
	vSrc.push_back(128);
	vSrc.push_back(-124);
	vSrc.push_back(28);
	vSrc.push_back(0);
	vSrc.push_back(81);
	vSrc.push_back(22);
	ASSERT_EQ(RET::SUC, DataProc::SelectSort(vSrc));
	ASSERT_EQ(128, vSrc[5]);
	ASSERT_EQ(81, vSrc[4]);
	ASSERT_EQ(28, vSrc[3]);
	ASSERT_EQ(22, vSrc[2]);
	ASSERT_EQ(0, vSrc[1]);
	ASSERT_EQ(-124, vSrc[0]);

	//测试选择排序
	vSrc.clear();
	vSrc.push_back(129);
	vSrc.push_back(-124);
	vSrc.push_back(23);
	vSrc.push_back(0);
	vSrc.push_back(98);
	vSrc.push_back(13);
	ASSERT_EQ(RET::SUC, DataProc::InsertSort(vSrc));
	ASSERT_EQ(129, vSrc[5]);
	ASSERT_EQ(98, vSrc[4]);
	ASSERT_EQ(23, vSrc[3]);
	ASSERT_EQ(13, vSrc[2]);
	ASSERT_EQ(0, vSrc[1]);
	ASSERT_EQ(-124, vSrc[0]);
}
