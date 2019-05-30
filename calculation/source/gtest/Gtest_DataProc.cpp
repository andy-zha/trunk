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
